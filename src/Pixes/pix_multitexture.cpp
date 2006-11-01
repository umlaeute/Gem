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

#include <stdio.h>
#ifdef __WIN32__
# include <io.h>
# include <windows.h>
# define snprintf _snprintf
#endif

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
  : m_inlet(NULL),
    m_reqTexUnits((GLint)reqTexUnits), m_max(0), m_textureType(GL_TEXTURE_2D), m_mode(0),
    m_xRatio(1.f), m_yRatio(1.f), upsidedown(false), m_texSizeX(0), m_texSizeY(0),
    m_oldTexCoords(NULL), m_oldNumCoords(0), m_oldTexture(0)
{
#if !defined(GL_VERSION_1_3) && !defined(GL_ARB_multitexture)
  post("[pix_multitexture]: GEM has been compiled without ARB-multitexture support");
#endif
  if (m_reqTexUnits<=0) {
    throw (GemException("[pix_multitexture]: Please specify more than 0 texture units"));
  }

#if defined(GL_TEXTURE_RECTANGLE_EXT) 
  //|| defined(GL_NV_TEXTURE_RECTANGLE)
  m_mode = 1;  //default to the fastest mode for systems that support it
  m_textureType = GL_TEXTURE_RECTANGLE_EXT;
#endif
  
  m_inlet=new t_inlet*[m_reqTexUnits]; 
  char tempVt[5];
  for(int i=0;i<m_reqTexUnits; i++){
    snprintf(tempVt, 5, "#%d\0", i);
    m_inlet[i]=inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym(tempVt));
  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_multitexture :: ~pix_multitexture()
{ 
  if(m_inlet){
    for(int i=0;i<m_reqTexUnits; i++){
      inlet_free(m_inlet[i]);
    }
    delete[]m_inlet;
  }
}

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
	int x_2, y_2;
	
	m_oldTexCoords=state->texCoords;
	m_oldNumCoords=state->numTexCoords;
	m_oldTexture  =state->texture;
	
	state->multiTexUnits = m_reqTexUnits;
	if (m_textureType == GL_TEXTURE_2D)
	{
//	  x_2 = powerOfTwo(m_texSizeX);
//	  y_2 = powerOfTwo(m_texSizeY);
//	  normalized = ((m_imagebuf.xsize==x_2) && (m_imagebuf.ysize==y_2));
//	  setTexCoords(m_coords, 1.f, 1.f, true);
	  m_xRatio = 1.0;
	  m_yRatio = 1.0;
	  state->texture = 1;
	}else{
	  m_xRatio = m_texSizeX;
	  m_yRatio = m_texSizeY;
	  state->texture = 2;
	}
	setTexCoords(m_coords, m_xRatio, m_yRatio, true);
	state->texCoords = m_coords;
	state->numTexCoords = 4;
	
	for ( int i=0; i< m_reqTexUnits; i++ )
	{
#if defined(GL_VERSION_1_3)
        glActiveTexture( GL_TEXTURE0 + i );
#elif defined(GL_ARB_multitexture) && !defined(GL_VERSION_1_3)
		glActiveTextureARB( GL_TEXTURE0_ARB + i );
#endif
		glEnable( m_textureType );
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
  state->texCoords   = m_oldTexCoords;
  state->numTexCoords= m_oldNumCoords;
  state->texture     = m_oldTexture;
  
#if defined(GL_VERSION_1_3)
  for ( int i = m_reqTexUnits; i>0; i--)
  {
    glActiveTexture( GL_TEXTURE0 + i);
    glDisable( m_textureType );
  }
  glActiveTexture( GL_TEXTURE0 );
#elif defined(GL_ARB_multitexture) && !defined(GL_VERSION_1_3)
  for ( int i = m_reqTexUnits; i>0; i--)
  {
    glActiveTextureARB( GL_TEXTURE0_ARB + i);
    glDisable( m_textureType );
  }
  glActiveTextureARB( GL_TEXTURE0_ARB );
#endif

/*  test code to auto-generate texture coords:  doesn't work :-(
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
*/
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
  class_addmethod(classPtr, (t_method)&pix_multitexture::dimenMessCallback,
                  gensym("dimen"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_multitexture::modeCallback,
		gensym("mode"), A_FLOAT, A_NULL);
  // generic inlets for texUnit
  class_addanything(classPtr, (t_method)&pix_multitexture::parmCallback);
}
void pix_multitexture :: texUnitMessCallback(void *data, float n, float texID)
{
  GetMyClass(data)->m_texID[(int)n] = (GLint)texID;
}

void pix_multitexture :: dimenMessCallback(void *data, float sizeX, float sizeY)
{
  GetMyClass(data)->m_texSizeX = (int)sizeX;
  GetMyClass(data)->m_texSizeY = (int)sizeY;
}

void pix_multitexture :: modeCallback(void *data, t_floatarg textype)
{
  GetMyClass(data)->m_mode=((int)textype);
  if (textype)
  {
//    GetMyClass(data)->m_oldType = GetMyClass(data)->m_textureType;
    GetMyClass(data)->m_textureType = GL_TEXTURE_RECTANGLE_EXT;
    post("[%s]:  using mode 1:GL_TEXTURE_RECTANGLE_EXT", GetMyClass(data)->m_objectname->s_name);
  }else{
    GetMyClass(data)->m_textureType = GL_TEXTURE_2D;
	post("[%s]:  using mode 0:GL_TEXTURE_2D", GetMyClass(data)->m_objectname->s_name);
  }
}

void pix_multitexture :: parmCallback(void *data, t_symbol*s, int argc, t_atom*argv){
  if(argc>0&&argv->a_type==A_FLOAT&&('#'==*s->s_name)){
    int i = atoi(s->s_name+1);
    GetMyClass(data)->m_texID[i]=(GLint)atom_getint(argv);
  } else {
     GetMyClass(data)->PDerror("invalid texUnit specified!");
  }
}
