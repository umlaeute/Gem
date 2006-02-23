////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2003 Daniel Heckenberg
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_snap2tex.h"

#include "Base/GemMan.h"
#include "Base/GemFuncUtil.h"

CPPEXTERN_NEW_WITH_GIMME(pix_snap2tex)

/////////////////////////////////////////////////////////
//
// pix_snap2tex
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

pix_snap2tex :: pix_snap2tex(int argc, t_atom *argv)
  : m_textureOnOff(1), m_textureQuality(GL_LINEAR),
    m_textureType(GL_TEXTURE_2D), m_repeat(GL_REPEAT),
    m_texWidth(-1), m_texHeight(-1),
    m_oldWidth(-1), m_oldHeight(-1),
	m_textureObj(0)
{
  if (argc == 4)
    {
      m_x = (int)atom_getfloat(&argv[0]);
      m_y = (int)atom_getfloat(&argv[1]);
      m_width = (int)atom_getfloat(&argv[2]);
      m_height = (int)atom_getfloat(&argv[3]);
    }
  else if (argc == 2)
    {
      m_x = m_y = 0;
      m_width = (int)atom_getfloat(&argv[0]);
      m_height = (int)atom_getfloat(&argv[1]);
    }
  else if (argc == 0)
    {
      m_x = m_y = 0;
      m_width = m_height = -1;
    }
  else
    {
      error("GEM: pix_snap2tex: needs 0, 2, or 4 values");
      m_x = m_y = 0;
      m_width = m_height = 128;
    }
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vert_pos"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vert_size"));
  
  // create an outlet to send texture info
  m_outTexInfo = outlet_new(this->x_obj, 0);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_snap2tex :: ~pix_snap2tex()
{
}

/////////////////////////////////////////////////////////
// setUpTextureState
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: setUpTextureState()
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexParameterf(m_textureType, GL_TEXTURE_WRAP_S, m_repeat);
  glTexParameterf(m_textureType, GL_TEXTURE_WRAP_T, m_repeat);
  glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, m_textureQuality);
  glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, m_textureQuality);
#ifdef GL_TEXTURE_RECTANGLE_EXT
  if (m_mode)
    if ( m_textureType !=  GL_TEXTURE_RECTANGLE_EXT)
#endif //GL_TEXTURE_RECTANGLE_EXT
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

/////////////////////////////////////////////////////////
// snapMess
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: snapMess()
{
  if ( !GemMan::windowExists() ) return;

  int width =(m_width <0)?GemMan::m_width :m_width;
  int height=(m_height<0)?GemMan::m_height:m_height;

  if (width <= 0 || height <= 0)
    {
      error("GEM: pix_snap2tex: Illegal size");
      return;
    }

  glEnable(m_textureType);

#ifdef GL_VERSION_1_1
  glBindTexture(m_textureType, m_textureObj);
#elif GL_EXT_texture_object
  glBindTextureEXT(m_textureType, m_textureObj);
#else
  // can we build a display list?
  int creatingDispList = 0;
  glNewList(m_textureObj, GL_COMPILE_AND_EXECUTE);
  creatingDispList = 1;
  setUpTextureState();
#endif    

  // if the size changed, then reset the texture
  int x_2 = powerOfTwo(width);
  int y_2 = powerOfTwo(height);

  if (width != m_oldWidth || height != m_oldHeight) 
    {
      m_oldWidth = width;
      m_oldHeight = height;

      float m_xRatio = (float)width / (float)x_2;
      float m_yRatio = (float)height / (float)y_2;
		
      m_coords[0].s = 0.f;
      m_coords[0].t = 0.f;
		
      m_coords[1].s = m_xRatio;
      m_coords[1].t = 0.f;
		
      m_coords[2].s = m_xRatio;
      m_coords[2].t = m_yRatio;
		
      m_coords[3].s = 0.f;
      m_coords[3].t = m_yRatio;
//    }

/*  if (x_2 != m_texWidth || y_2 != m_texHeight)	{ */
    m_texWidth = x_2;
    m_texHeight = y_2;
		
    glCopyTexImage2D(	m_textureType, 0,
			GL_RGB,
			m_x, m_y,
			m_texWidth, m_texHeight, 
			0);

  } else
  m_texHeight = m_height;
  m_texWidth = m_width;
  
    {
      glCopyTexSubImage2D(m_textureType, 0,
			  0, 0,
			  m_x, m_y,		// position
			  m_texWidth,
			  m_texHeight);		
    }

//post("pix_snap2tex: m_x %d m_y %d m_texWidth %d m_texHeight %d",m_x,m_y,m_texWidth,m_texHeight);


#ifdef GL_VERSION_1_1
#elif GL_EXT_texture_object
#else
  if (creatingDispList)
    {
      glEndList();
    }
#endif

  glDisable(m_textureType);

}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: render(GemState *state)
{
  if (!m_textureOnOff) return;

  state->texture = 1;
  state->texCoords = m_coords;
  state->numTexCoords = 4;

  glEnable(m_textureType);

#ifdef GL_VERSION_1_1
  glBindTexture(m_textureType, m_textureObj);
#elif GL_EXT_texture_object
  glBindTextureEXT(m_textureType, m_textureObj);
#else
  glCallList(m_textureObj)
#endif    

  t_atom ap[4];
  SETFLOAT(ap, (t_float)m_textureObj);
  SETFLOAT(ap+1, m_texWidth);
  SETFLOAT(ap+2, m_texHeight);
  SETFLOAT(ap+3, m_textureType);
  // send texture info to outlet
  outlet_list(m_outTexInfo, 0, 4, ap);
}


/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: postrender(GemState *state)
{
  if (!m_textureOnOff) return;

  state->texture = 0;
  state->numTexCoords = 0;	
  state->texCoords = NULL;	

  glDisable(m_textureType);
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: startRendering()
{
#ifdef GL_VERSION_1_1
  glGenTextures(1, &m_textureObj);
  glBindTexture(m_textureType, m_textureObj);
  setUpTextureState();
#elif GL_EXT_texture_object
  glGenTexturesEXT(1, &m_textureObj);
  glBindTextureEXT(m_textureType, m_textureObj);    
  setUpTextureState();
#else
  m_textureObj = glGenLists(1);
  m_rebuildList = 1;
#endif
  m_oldWidth = m_oldHeight = m_texWidth = m_texHeight = -1;
  if (!m_textureObj)	{
    error("Gem: pix_snap2tex: Unable to allocate texture object");
    return;
  }

}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: stopRendering()
{
#ifdef GL_VERSION_1_1
  if (m_textureObj) glDeleteTextures(1, &m_textureObj);
#elif GL_EXT_texture_object
  if (m_textureObj) glDeleteTexturesEXT(1, &m_textureObj);
#else
  if (m_textureObj) glDeleteLists(m_textureObj, 1);
#endif
  m_textureObj = 0;
  m_oldWidth = m_oldHeight = m_texWidth = m_texHeight = -1;
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
// cleanImage
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: cleanImage()
{
  // release previous data
  post("pix_snap2tex2tex: clean is unimplemented.");
  setModified();
}

/////////////////////////////////////////////////////////
// textureOnOff
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: textureOnOff(int on)
{
  m_textureOnOff = on;
  setModified();
}

/////////////////////////////////////////////////////////
// textureQuality
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: textureQuality(int type)
{
  if (type)
    m_textureQuality = GL_LINEAR;
  else
    m_textureQuality = GL_NEAREST;
  if (m_textureObj)
    {
#ifdef GL_VERSION_1_1
      glBindTexture(m_textureType, m_textureObj);
      glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, m_textureQuality);
      glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, m_textureQuality);
#elif GL_EXT_texture_object
      glBindTextureEXT(m_textureType, m_textureObj);
      glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, m_textureQuality);
      glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, m_textureQuality);
#else
#endif
    }
  setModified();
}

/////////////////////////////////////////////////////////
// repeatMess
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: repeatMess(int type)
{
  if (type)
    m_repeat = GL_REPEAT;
  else
#ifdef GL_CLAMP_TO_EDGE
    m_repeat = GL_CLAMP_TO_EDGE;
#else
    m_repeat = GL_CLAMP;
#endif

  if (m_textureObj) {
#ifdef GL_VERSION_1_1
    glBindTexture(m_textureType, m_textureObj);
    glTexParameterf(m_textureType, GL_TEXTURE_WRAP_S, m_repeat);
    glTexParameterf(m_textureType, GL_TEXTURE_WRAP_T, m_repeat);
#elif GL_EXT_texture_object
    glBindTextureEXT(m_textureType, m_textureObj);
    glTexParameteri(m_textureType, GL_TEXTURE_WRAP_S, m_repeat);
    glTexParameteri(m_textureType, GL_TEXTURE_WRAP_T, m_repeat);
#else
#endif
  }
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void pix_snap2tex :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_snap2tex::snapMessCallback,
		  gensym("snap"), A_NULL);
  class_addbang(classPtr, (t_method)&pix_snap2tex::snapMessCallback);

  class_addmethod(classPtr, (t_method)&pix_snap2tex::sizeMessCallback,
		  gensym("vert_size"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_snap2tex::posMessCallback,
		  gensym("vert_pos"), A_FLOAT, A_FLOAT, A_NULL);

  class_addfloat(classPtr, (t_method)&pix_snap2tex::floatMessCallback);    
  class_addmethod(classPtr, (t_method)&pix_snap2tex::textureMessCallback,
		  gensym("quality"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_snap2tex::repeatMessCallback,
		  gensym("repeat"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_snap2tex::modeCallback,
		  gensym("mode"), A_FLOAT, A_NULL);
}
void pix_snap2tex :: snapMessCallback(void *data)
{
  GetMyClass(data)->snapMess();
}
void pix_snap2tex :: sizeMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->sizeMess((int)width, (int)height);
}
void pix_snap2tex :: posMessCallback(void *data, t_floatarg x, t_floatarg y)
{
  GetMyClass(data)->posMess((int)x, (int)y);
}
void pix_snap2tex :: floatMessCallback(void *data, float n)
{
  GetMyClass(data)->textureOnOff((int)n);
}
void pix_snap2tex :: textureMessCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->textureQuality((int)quality);
}
void pix_snap2tex :: repeatMessCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->repeatMess((int)quality);
}
void pix_snap2tex :: modeCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->m_mode=((int)quality);
}
