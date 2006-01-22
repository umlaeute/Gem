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

#ifdef __APPLE__
extern bool HaveValidContext (void);
#endif // __APPLE__

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
  : m_reqTexUnits((GLint)reqTexUnits), m_max(0), m_mode(1)
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
// render
//
/////////////////////////////////////////////////////////
void pix_multitexture :: render(GemState *state)
{
#ifdef GL_TEXTURE0_ARB
	if ( !m_mode )
		m_textureType = GL_TEXTURE_2D;
	else
		m_textureType = GL_TEXTURE_RECTANGLE_EXT;
		
	for ( int i=0; i< m_reqTexUnits; i++ )
	{
		glActiveTextureARB( GL_TEXTURE0_ARB + i );
		glEnable( m_textureType );
		glBindTexture( m_textureType, m_texID[i] );
	}
#endif
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_multitexture :: postrender(GemState *state)
{
#ifdef GL_TEXTURE0_ARB
  state->texture = 0;
  for ( int i = m_reqTexUnits; i>0; i--)
  {
    glActiveTextureARB( GL_TEXTURE0_ARB + i);
    glDisable( m_textureType );
  }
  glActiveTextureARB( GL_TEXTURE0_ARB );
#endif
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_multitexture :: startRendering()
{
#ifdef __APPLE__
  if (!HaveValidContext ()) {
	post("GEM: pix_multitexture - need window/context to start");
	return;
  }
#endif
#ifdef GL_MAX_TEXTURE_UNITS_ARB
  glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &m_max );
  post("[pix_multitexture]: MAX_TEXTURE_UNITS for current context = %d", m_max);
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
