///////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    Copyright (c) 2003 Daniel Heckenberg
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_snap2tex.h"

#include "Gem/Manager.h"
#include "Utils/Functions.h"

CPPEXTERN_NEW_WITH_GIMME(pix_snap2tex);

/////////////////////////////////////////////////////////
//
// pix_snap2tex
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

pix_snap2tex :: pix_snap2tex(int argc, t_atom *argv)
  :
  m_textureObj(0), m_textureType(GL_TEXTURE_2D),
  m_didTexture(false), m_init(false),

  m_textureOnOff(true),
  m_textureQuality(GL_LINEAR), m_repeat(GL_REPEAT),
  m_rectangle(0), m_canRectangle(0),
  m_texUnit(0),
  m_x(0), m_y(0), m_width(-1), m_height(-1),
  m_texWidth(-1), m_texHeight(-1),
  m_xRatio(1.), m_yRatio(1.),
  m_oldTexCoords(NULL), m_oldNumCoords(0), m_oldTexture(0),
  m_outTexInfo(NULL)
{
  switch(argc) {
  case 4:
    m_x = atom_getint(&argv[0]);
    m_y = atom_getint(&argv[1]);
    m_width = atom_getint(&argv[2]);
    m_height = atom_getint(&argv[3]);
    break;
  case 2:
    m_x = m_y = 0;
    m_width = atom_getint(&argv[0]);
    m_height = atom_getint(&argv[1]);
    break;
  /* coverity[unterminated_default] */
  default:
    error("needs 0, 2, or 4 values");
  case 0:
    m_x = m_y = 0;
    m_width = m_height = -1;
    break;
  }

  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("pos"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"),
            gensym("size"));

  // create an outlet to send texture info
  m_outTexInfo = outlet_new(this->x_obj, 0);
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_snap2tex :: ~pix_snap2tex(void)
{
}

////////////////////////////////////////////////////////
// setUpTextureState
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: setUpTextureState(void)
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexParameterf(m_textureType, GL_TEXTURE_WRAP_S, m_repeat);
  glTexParameterf(m_textureType, GL_TEXTURE_WRAP_T, m_repeat);
  glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, m_textureQuality);
  glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, m_textureQuality);
#if 0
  if (m_rectangle && m_textureType !=  GL_TEXTURE_RECTANGLE_EXT) {
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  }
#endif
}


/////////////////////////////////////////////////////////
// extension checks
//
/////////////////////////////////////////////////////////
bool pix_snap2tex :: isRunnable(void)
{
  if(!GLEW_VERSION_1_1 && !GLEW_EXT_texture_object) {
    error("your system lacks texture support");
    return false;
  }

  /* check rectangle possibilities */
  m_canRectangle=GL_TEXTURE_2D;
  if(GLEW_ARB_texture_rectangle) {
    m_canRectangle=GL_TEXTURE_RECTANGLE_ARB;
  } else if (GLEW_EXT_texture_rectangle) {
    m_canRectangle=GL_TEXTURE_RECTANGLE_EXT;
  }

  return true;
}


void pix_snap2tex :: setTexCoords(float x, float y)
{
  m_xRatio = x;
  m_yRatio = y;

  m_coords[0].s = 0.f;
  m_coords[0].t = 0.f;

  m_coords[1].s = x;
  m_coords[1].t = 0.f;

  m_coords[2].s = x;
  m_coords[2].t = y;

  m_coords[3].s = 0.f;
  m_coords[3].t = y;
}

/////////////////////////////////////////////////////////
// snapMess
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: snapMess(void)
{
  if(getState()==INIT) {
    verbose(0, "not initialized yet with a valid context");
    return;
  }
  if(!GLEW_VERSION_1_1 && !GLEW_EXT_texture_object) {
    return;
  }

  int width  = m_width;
  int height = m_height;

  GemMan::getDimen(((m_width >0)?NULL:&width ),
                   ((m_height>0)?NULL:&height));

  if (width <= 0 || height <= 0) {
    error("Illegal size %dx%d", width, height);
    return;
  }

  if(GLEW_VERSION_1_3) {
    glActiveTexture(GL_TEXTURE0_ARB + m_texUnit);
  }

  m_textureType = (m_rectangle?m_canRectangle:GL_TEXTURE_2D);

  glEnable(m_textureType);

  if(GLEW_VERSION_1_1) {
    glBindTexture(m_textureType, m_textureObj);
  } else {
    glBindTextureEXT(m_textureType, m_textureObj);
  }

  if (m_init) {
    m_init=false;
    // if the size changed, then reset the texture
    if(GL_TEXTURE_2D == m_textureType) {
      int x_2 = powerOfTwo(width);
      int y_2 = powerOfTwo(height);
      m_texWidth = x_2;
      m_texHeight = y_2;
      setTexCoords((float)width / (float)x_2, (float)height / (float)y_2);

      glCopyTexImage2D( m_textureType, 0,
#ifdef __EMSCRIPTEN__
                        GL_RGB,
#else
                        GL_RGBA16,
#endif
                        m_x, m_y,
                        m_texWidth, m_texHeight,
                        0);
    } else {
      setTexCoords(width, height);
      m_texWidth  = width;
      m_texHeight = height;
      glCopyTexImage2D( m_textureType, 0,
#ifdef __EMSCRIPTEN__
                        GL_RGB,
#else
                        GL_RGBA16,
#endif
                        m_x, m_y,
                        m_texWidth, m_texHeight,
                        0);
    }
  } else {
    m_texHeight = height;
    m_texWidth = width;
  }

  glCopyTexSubImage2D(m_textureType, 0,
                      0, 0,
                      m_x, m_y,         // position
                      m_texWidth, m_texHeight);

  glDisable(m_textureType);

  if(GLEW_VERSION_1_3) {
    glActiveTexture(GL_TEXTURE0_ARB);
  }
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: render(GemState *state)
{
  m_didTexture=false;
  state->get(GemState::_GL_TEX_COORDS, m_oldTexCoords);
  state->get(GemState::_GL_TEX_NUMCOORDS, m_oldNumCoords);
  state->get(GemState::_GL_TEX_TYPE, m_oldTexture);

  if (!m_textureOnOff) {
    return;
  }

  state->set(GemState::_GL_TEX_COORDS, static_cast<TexCoord*>(m_coords));
  state->set(GemState::_GL_TEX_NUMCOORDS, 4);
  state->set(GemState::_GL_TEX_TYPE, 1);


  if(GLEW_VERSION_1_3) {
    glActiveTexture(GL_TEXTURE0_ARB + m_texUnit);  //needed?
  }

  glEnable(m_textureType);

  if(GLEW_VERSION_1_1) {
    glBindTexture(m_textureType, m_textureObj);
  } else {
    glBindTextureEXT(m_textureType, m_textureObj);
  }

  m_didTexture=true;

  t_atom ap[5];
  SETFLOAT(ap, (t_float)m_textureObj);
  SETFLOAT(ap+1, m_xRatio);
  SETFLOAT(ap+2, m_yRatio);
  SETFLOAT(ap+3, m_textureType);
  SETFLOAT(ap+4, static_cast<t_float>(0));// upsidedown
  // send texture info to outlet
  outlet_list(m_outTexInfo, 0, 5, ap);
}


/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: postrender(GemState *state)
{
  state->set(GemState::_GL_TEX_COORDS, m_oldTexCoords);
  state->set(GemState::_GL_TEX_NUMCOORDS, m_oldNumCoords);
  state->set(GemState::_GL_TEX_TYPE, m_oldTexture);

  if (m_didTexture) {
    if(GLEW_VERSION_1_3) {
      glActiveTexture(GL_TEXTURE0_ARB + m_texUnit);  //needed?
    }
    glDisable(m_textureType);
    // to avoid matrix stack confusion, we reset the upstream texunit to 0
    if(GLEW_VERSION_1_3) {
      glActiveTexture(GL_TEXTURE0_ARB);
    }
  }
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: startRendering(void)
{
  if(GLEW_VERSION_1_1) {
    glGenTextures(1, &m_textureObj);

    if(GLEW_VERSION_1_3) {
      glActiveTexture(GL_TEXTURE0_ARB + m_texUnit);
    }

    m_textureType = (m_rectangle?m_canRectangle:GL_TEXTURE_2D);

    glBindTexture(m_textureType, m_textureObj);
    setUpTextureState();
  } else {
    glGenTexturesEXT(1, &m_textureObj);
    glBindTextureEXT(m_textureType, m_textureObj);
    setUpTextureState();
  }
  if(GLEW_VERSION_1_3) {
    glActiveTexture(GL_TEXTURE0_ARB);
  }

  m_texWidth = m_texHeight = -1;
  m_init=true;

  if (!m_textureObj)    {
    error("Unable to allocate texture object");
    return;
  }
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: stopRendering(void)
{
  if(m_textureObj) {
    if(GLEW_VERSION_1_1) {
      glDeleteTextures(1, &m_textureObj);
    } else {
      glDeleteTexturesEXT(1, &m_textureObj);
    }
  }
  m_textureObj = 0;
  m_texWidth = m_texHeight = -1;
}


/////////////////////////////////////////////////////////
// sizeMess
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: sizeMess(int width, int height)
{
  m_width = width;
  m_height = height;
  setModified();
}

/////////////////////////////////////////////////////////
// posMess
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: posMess(int x, int y)
{
  m_x = x;
  m_y = y;
  setModified();
}

/////////////////////////////////////////////////////////
// textureOnOff
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: textureOnOff(bool on)
{
  m_textureOnOff = on;
  setModified();
}

////////////////////////////////////////////////////////
// textureOnOff
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: rectangleMess(int mode)
{
  m_rectangle = mode;
  setModified();
}
void pix_snap2tex :: texUnitMess(int unit)
{
  m_texUnit = unit;
  setModified();
}


/////////////////////////////////////////////////////////
// textureQuality
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: textureQuality(int type)
{
  if (type) {
    m_textureQuality = GL_LINEAR;
  } else {
    m_textureQuality = GL_NEAREST;
  }
  if (m_textureObj) {
    if(GLEW_VERSION_1_3) {
      glActiveTexture(GL_TEXTURE0_ARB + m_texUnit);
    }
    if(GLEW_VERSION_1_1) {
      glBindTexture(m_textureType, m_textureObj);
      glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, m_textureQuality);
      glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, m_textureQuality);
    } else if (GLEW_EXT_texture_object) {
      glBindTextureEXT(m_textureType, m_textureObj);
      glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, m_textureQuality);
      glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, m_textureQuality);
    }
    if(GLEW_VERSION_1_3) {
      glActiveTexture(GL_TEXTURE0_ARB);
    }
  }
  setModified();
}

/////////////////////////////////////////////////////////
// repeatMess
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: repeatMess(int type)
{
  if (type) {
    m_repeat = GL_REPEAT;
  } else {
    m_repeat = GL_CLAMP_TO_EDGE;
  }

  if (m_textureObj) {
    if(GLEW_VERSION_1_3) {
      glActiveTexture(GL_TEXTURE0_ARB + m_texUnit);
    }
    if(GLEW_VERSION_1_1) {
      glBindTexture(m_textureType, m_textureObj);
      glTexParameterf(m_textureType, GL_TEXTURE_WRAP_S, m_repeat);
      glTexParameterf(m_textureType, GL_TEXTURE_WRAP_T, m_repeat);
    } else if (GLEW_EXT_texture_object) {
      glBindTextureEXT(m_textureType, m_textureObj);
      glTexParameteri(m_textureType, GL_TEXTURE_WRAP_S, m_repeat);
      glTexParameteri(m_textureType, GL_TEXTURE_WRAP_T, m_repeat);
    }
    if(GLEW_VERSION_1_3) {
      glActiveTexture(GL_TEXTURE0_ARB);
    }
  }
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG0(classPtr, "snap", snapMess);
  CPPEXTERN_MSG0(classPtr, "bang", snapMess);

  CPPEXTERN_MSG1(classPtr, "float", textureOnOff, bool);

  CPPEXTERN_MSG2(classPtr, "size", sizeMess, int, int);
  CPPEXTERN_MSG2(classPtr, "pos", posMess, int, int);

  CPPEXTERN_MSG1(classPtr, "quality", textureQuality, int);
  CPPEXTERN_MSG1(classPtr, "repeat", repeatMess, int);

  CPPEXTERN_MSG1(classPtr, "rectangle", rectangleMess, int);
  CPPEXTERN_MSG1(classPtr, "texunit", texUnitMess, int);
}
