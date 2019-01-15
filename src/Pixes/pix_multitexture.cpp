////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// tigital@mac.com
//
// Implementation file
//
//    Copyright (c) 2005 James Tittle II
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_multitexture.h"

#include "Gem/Manager.h"
#include "Gem/Image.h"
#include "Gem/Exception.h"

#include <stdio.h>
#ifdef _WIN32
# include <io.h>
# include <windows.h>
# define snprintf _snprintf
#endif

CPPEXTERN_NEW_WITH_ONE_ARG(pix_multitexture, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// pix_multitexture
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_multitexture :: pix_multitexture(t_floatarg reqTexUnits)
  : m_inlet(NULL), m_numInlets(0),
    m_reqTexUnits((GLint)reqTexUnits), m_textureType(GL_TEXTURE_2D),
    m_mode(0),
    m_xRatio(1.f), m_yRatio(1.f), upsidedown(false), m_texSizeX(0),
    m_texSizeY(0),
    m_oldTexCoords(NULL), m_oldNumCoords(0), m_oldTexture(0)
{
  if (m_reqTexUnits<=0) {
    throw (GemException("[pix_multitexture]: Please specify more than 0 texture units"));
  }

#ifdef __APPLE__
  m_mode = 1;  //default to the fastest mode for systems that support it
  m_textureType = GL_TEXTURE_RECTANGLE_EXT;
#endif

  for(int i=0; i<MAX_MULTITEX_ID; i++) {
    m_texID[i]=0;
  }

  m_numInlets=m_reqTexUnits;
  m_inlet=new t_inlet*[m_numInlets];
  char tempVt[5];
  for(int i=0; i<m_numInlets; i++) {
    snprintf(tempVt, 5, "#%d", i);
    tempVt[4]=0;
    m_inlet[i]=inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym(tempVt));
  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_multitexture :: ~pix_multitexture()
{
  if(m_inlet) {
    for(int i=0; i<m_numInlets; i++) {
      inlet_free(m_inlet[i]);
    }
    delete[]m_inlet;
  }
}

/////////////////////////////////////////////////////////
// extension checks
//
/////////////////////////////////////////////////////////
bool pix_multitexture :: isRunnable(void)
{
  if(GLEW_VERSION_1_3 && GLEW_ARB_multitexture) {
    GLint numTexUnits=0;
    glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &numTexUnits );
    m_useTexUnits = m_reqTexUnits;
    if(m_useTexUnits>numTexUnits)
      m_useTexUnits=numTexUnits;
    return true;
  }
  m_useTexUnits=0;
  error("your system lacks multitexture support");
  return false;
}

/////////////////////////////////////////////////////////
// setTexCoords
// upsidedown is derived from the imageStruct.upsidedown
// use this when loading images...
//
/////////////////////////////////////////////////////////
inline void setTexCoords(TexCoord *coords, float xRatio, float yRatio,
                         GLboolean upsidedown=false)
{
  if(!upsidedown) {
    coords[0].s = 0.f;
    coords[0].t = 0.f;
    coords[1].s = xRatio;
    coords[1].t = 0.f;
    coords[2].s = xRatio;
    coords[2].t = yRatio;
    coords[3].s = 0.f;
    coords[3].t = yRatio;
  } else {
    coords[3].s = 0.f;
    coords[3].t = 0.f;
    coords[2].s = xRatio;
    coords[2].t = 0.f;
    coords[1].s = xRatio;
    coords[1].t = yRatio;
    coords[0].s = 0.f;
    coords[0].t = yRatio;
  }
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_multitexture :: render(GemState *state)
{
  int textype=0;
  state->get(GemState::_GL_TEX_COORDS, m_oldTexCoords);
  state->get(GemState::_GL_TEX_NUMCOORDS, m_oldNumCoords);
  state->get(GemState::_GL_TEX_TYPE, m_oldTexture);
  state->get(GemState::_GL_TEX_UNITS, m_oldTexUnits);

  if (m_textureType == GL_TEXTURE_2D) {
    m_xRatio = 1.0;
    m_yRatio = 1.0;
    textype = 1;
  } else {
    m_xRatio = m_texSizeX;
    m_yRatio = m_texSizeY;
    textype = 2;
  }

  setTexCoords(m_coords, m_xRatio, m_yRatio, true);

  TexCoord*tc=m_coords;
  state->set(GemState::_GL_TEX_COORDS, tc);
  state->set(GemState::_GL_TEX_NUMCOORDS, 4);
  state->set(GemState::_GL_TEX_TYPE, textype);
  state->set(GemState::_GL_TEX_UNITS, m_useTexUnits);

  for ( int i=0; i< m_useTexUnits; i++ ) {
    if(GLEW_VERSION_1_3) {
      glActiveTexture( GL_TEXTURE0 + i );
    } else {
      glActiveTextureARB( GL_TEXTURE0_ARB + i );
    }

    glEnable( m_textureType );

    if(!m_texID[i])continue;
    glBindTexture( m_textureType, m_texID[i] );
    glTexParameteri( m_textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( m_textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  }
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_multitexture :: postrender(GemState *state)
{
  state->set(GemState::_GL_TEX_COORDS, m_oldTexCoords);
  state->set(GemState::_GL_TEX_NUMCOORDS, m_oldNumCoords);
  state->set(GemState::_GL_TEX_TYPE, m_oldTexture);
  state->set(GemState::_GL_TEX_UNITS, m_oldTexUnits);

  if(GLEW_VERSION_1_3) {
    for ( int i = m_useTexUnits; i>0; i--) {
      glActiveTexture( GL_TEXTURE0 + i);
      glDisable( m_textureType );
    }
    glActiveTexture( GL_TEXTURE0 );
  } else {
    for ( int i = m_useTexUnits; i>0; i--) {
      glActiveTextureARB( GL_TEXTURE0_ARB + i);
      glDisable( m_textureType );
    }
    glActiveTextureARB( GL_TEXTURE0_ARB );
  }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void pix_multitexture :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG2(classPtr, "texunit", texUnitMess, int, int);
  CPPEXTERN_MSG2(classPtr, "dimen",   dimenMess,   int, int);

  CPPEXTERN_MSG1(classPtr, "rectangle",rectangleMess,   bool);
  CPPEXTERN_MSG1(classPtr, "mode",rectangleMess,   bool);

  // generic inlets for texUnit
  class_addanything(classPtr,
                    reinterpret_cast<t_method>(&pix_multitexture::parmCallback));
}
void pix_multitexture :: texUnitMess(int n, int texID)
{
  if(n<0 || n>=MAX_MULTITEX_ID) {
    error("ID %d out of range 0..%d", n, MAX_MULTITEX_ID-1);
    return;
  }
  m_texID[(int)n] = (GLint)texID;
}

void pix_multitexture :: dimenMess(int sizeX, int sizeY)
{
  m_texSizeX = sizeX;
  m_texSizeY = sizeY;
  setModified();
}
void pix_multitexture :: rectangleMess(bool wantrect)
{
  m_mode=((int)wantrect);
  if (wantrect)  {
    m_textureType = GL_TEXTURE_RECTANGLE_EXT;
    verbose(1, "using mode 1:GL_TEXTURE_RECTANGLE_EXT");
  } else {
    m_textureType = GL_TEXTURE_2D;
    verbose(1, "using mode 0:GL_TEXTURE_2D");
  }
  setModified();
}

void pix_multitexture :: parmCallback(void*data, t_symbol*s, int argc,
                                      t_atom*argv)
{
  if(argc>0&&argv->a_type==A_FLOAT&&('#'==*s->s_name)) {
    int i = atoi(s->s_name+1);
    GetMyClass(data)->texUnitMess(i, atom_getint(argv));
  } else {
    GetMyClass(data)->error("invalid texUnit specified! %s[%d]", s->s_name, argc);
  }
}
