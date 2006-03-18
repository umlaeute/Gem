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

#include "Base/GemMan.h"
#include "Base/GemPixUtil.h"

CPPEXTERN_NEW_WITH_ONE_ARG(pix_multitexture, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// pix_multitexture
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_multitexture :: pix_multitexture(t_floatarg reqTexUnits)
  : m_reqTexUnits((GLint)reqTexUnits), m_max(0), m_textureType(0), m_mode(1),
    m_xRatio(1.f), m_yRatio(1.f), upsidedown(false),
    m_oldTexCoords(NULL), m_oldNumCoords(0), m_oldTexture(0)
{
#ifndef GL_TEXTURE0_ARB
  post("[pix_multitexture]: GEM has been compiled without ARB-multitexture support");
#endif
  if (m_reqTexUnits==0) {
    throw (GemException("[pix_multitexture]: Please specify more than 0 texture units"));
  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_multitexture :: ~pix_multitexture()
{ }

/////////////////////////////////////////////////////////
// setTexCoords
// upsidedown is derived from the imageStruct.upsidedown
// use this when loading images...
//
/////////////////////////////////////////////////////////
inline void setTexCoords(TexCoord *coords, float xRatio, float yRatio, GLboolean upsidedown=false){
  if(!upsidedown){
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
#ifdef GL_TEXTURE0_ARB
        m_oldTexCoords=state->texCoords;
	m_oldNumCoords=state->numTexCoords;
	m_oldTexture  =state->texture;

	if ( !m_mode )
		m_textureType = GL_TEXTURE_2D;
	else
		m_textureType = GL_TEXTURE_RECTANGLE_EXT;
	
	setTexCoords(m_coords, m_xRatio, m_yRatio, upsidedown);
	state->texCoords = m_coords;
	state->numTexCoords = 4;
	
	for ( int i=0; i< m_reqTexUnits; i++ )
	{
		glActiveTextureARB( GL_TEXTURE0_ARB + i );
		glEnable( m_textureType );
		glBindTexture( m_textureType, m_texID[i] );
	}
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
#endif
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_multitexture :: postrender(GemState *state)
{
#ifdef GL_TEXTURE0_ARB
  state->texCoords   = m_oldTexCoords;
  state->numTexCoords= m_oldNumCoords;
  state->texture     = m_oldTexture;

  for ( int i = m_reqTexUnits; i>0; i--)
  {
    glActiveTextureARB( GL_TEXTURE0_ARB + i);
    glDisable( m_textureType );
  }
  glActiveTextureARB( GL_TEXTURE0_ARB );
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
#endif
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_multitexture :: startRendering()
{
#ifdef GL_MAX_TEXTURE_UNITS_ARB
  glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &m_max );
  post("[%s]: MAX_TEXTURE_UNITS for current context = %d", m_objectname->s_name, m_max);
#endif
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_multitexture :: stopRendering()
{

}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void pix_multitexture :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_multitexture::texUnitMessCallback,
                  gensym("texUnit"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_multitexture::modeCallback,
		gensym("mode"), A_FLOAT, A_NULL);
}
void pix_multitexture :: texUnitMessCallback(void *data, float n, float texture)
{
  GetMyClass(data)->m_texID[(int)n] = (GLint)texture;
}
void pix_multitexture :: modeCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->m_mode=((int)quality);
}
