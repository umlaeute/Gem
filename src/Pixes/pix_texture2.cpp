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
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
//  pix_texture2
//
//  2001:forum::für::umläute:2001
//  IOhannes m zmoelnig
//  mailto:zmoelnig@iem.kug.ac.at
//
/////////////////////////////////////////////////////////

#include "pix_texture2.h"
#include "Base/GemMan.h"
#include <string.h>

CPPEXTERN_NEW(pix_texture2)

/////////////////////////////////////////////////////////
//
// pix_texture2
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_texture2 :: pix_texture2()
    	     : m_textureOnOff(1), m_textureQuality(GL_LINEAR),
               m_rebuildList(0), m_textureObj(0)
{
  m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = -1;

  m_buffer.xsize = m_buffer.ysize = m_buffer.csize = -1;
  m_buffer.data = NULL;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_texture2 :: ~pix_texture2()
{
}

/////////////////////////////////////////////////////////
// setUpTextureState
//
/////////////////////////////////////////////////////////
static inline int powerOfTwo(int value)
{
	int x = 1;
	while(x < value) x <<= 1;
	return(x);  
}

void pix_texture2 :: setUpTextureState()
{
#ifdef GL_TEXTURE_RECTANGLE_EXT
    if ( !GemMan::texture_rectangle_supported )				//tigital
    {
        glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    } else {
        glTexParameterf(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_PRIORITY, 0.0);
        if (GemMan::client_storage_supported)
            glPixelStoref(GL_UNPACK_CLIENT_STORAGE_APPLE, 1);
        else
            glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
        glTexParameterf(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
#else
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#endif
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_texture2 :: render(GemState *state)
{
  if ( !state->image || !m_textureOnOff) return;
  state->texture = 1;
  state->texCoords = m_coords;
  state->numTexCoords = 4;

#ifdef GL_VERSION_1_1		//tigital
#ifdef GL_TEXTURE_RECTANGLE_EXT
  if ( GemMan::texture_rectangle_supported ){
    glEnable(GL_TEXTURE_RECTANGLE_EXT);
    glBindTexture(GL_TEXTURE_RECTANGLE_EXT, m_textureObj);
  } else
#endif
    {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, m_textureObj);
    }
#elif GL_EXT_texture_object
  glEnable(GL_TEXTURE_2D);
  glBindTextureEXT(GL_TEXTURE_2D, m_textureObj);
#else
  // can we build a display list?
  int creatingDispList = 0;
  if (state->image->newimage) m_rebuildList = 1;
  if (!state->inDisplayList && m_rebuildList) {
    glNewList(m_textureObj, GL_COMPILE_AND_EXECUTE);
    creatingDispList = 1;
  }
  setUpTextureState();
#endif    

#ifdef GL_VERSION_1_1
  if (state->image->newimage)
#elif GL_EXT_texture_object
  if (state->image->newimage)
#else
  if (m_rebuildList)
#endif
    {
      // if the size changed, then reset the texture
      if (!GemMan::texture_rectangle_supported ) {
	int x_2 = powerOfTwo(state->image->image.xsize);
	int y_2 = powerOfTwo(state->image->image.ysize);
	
	if (x_2 != m_buffer.xsize || y_2 != m_buffer.ysize) {
	  m_buffer.clear();
	  m_buffer.xsize = x_2;
	  m_buffer.ysize = y_2;
	  m_buffer.csize = state->image->image.csize;
	  m_buffer.format = state->image->image.format;
	  m_buffer.type = state->image->image.type;
	  
	  m_buffer.allocate(m_buffer.xsize*m_buffer.ysize*m_buffer.csize*sizeof(unsigned char));
	  memset(m_buffer.data, 0, m_buffer.xsize*m_buffer.ysize*m_buffer.csize*sizeof(unsigned char));
	
	  float m_xRatio = (float)state->image->image.xsize / (float)x_2;
	  float m_yRatio = (float)state->image->image.ysize / (float)y_2;
#ifndef MACOSX		
	  m_coords[0].s = 0.f;
	  m_coords[0].t = 0.f;
	  
	  m_coords[1].s = m_xRatio;
	  m_coords[1].t = 0.f;
	  
	  m_coords[2].s = m_xRatio;
	  m_coords[2].t = m_yRatio;
	  
	  m_coords[3].s = 0.f;
	  m_coords[3].t = m_yRatio;
#else
	  m_coords[3].s = 0.f;		// switched the order of m_coords on MACOSX
	  m_coords[3].t = 0.f;		// otherwise we'd be upside down!
	  
	  m_coords[2].s = m_xRatio;
	  m_coords[2].t = 0.f;
		
	  m_coords[1].s = m_xRatio;
	  m_coords[1].t = m_yRatio;
		
	  m_coords[0].s = 0.f;
	  m_coords[0].t = m_yRatio;
#endif
	}
      } else {				// tigital
	if (state->image->image.xsize != m_buffer.xsize || state->image->image.ysize != m_buffer.ysize) {
	  m_buffer.clear();
	  m_buffer.xsize = state->image->image.xsize;
	  m_buffer.ysize = state->image->image.ysize;
	  m_buffer.csize = state->image->image.csize;
	  m_buffer.format = state->image->image.format;
	  m_buffer.type = state->image->image.type;
          
	  m_buffer.allocate(m_buffer.xsize*m_buffer.ysize*m_buffer.csize*sizeof(unsigned char));
	  memset(m_buffer.data, 0, m_buffer.xsize*m_buffer.ysize*m_buffer.csize*sizeof(unsigned char));
#ifndef MACOSX            
	  m_coords[0].s = 0.f;
	  m_coords[0].t = 0.f;
	  
	  m_coords[1].s = (float)state->image->image.xsize;
	  m_coords[1].t = 0.f;
	  
	  m_coords[2].s = (float)state->image->image.xsize;
	  m_coords[2].t = (float)state->image->image.ysize;
		
	  m_coords[3].s = 0.f;
	  m_coords[3].t = (float)state->image->image.ysize;
#else
	  m_coords[3].s = 0.f;		// switched the order of m_coords on MACOSX
	  m_coords[3].t = 0.f;		// otherwise we'd be upside down!
		
	  m_coords[2].s = (float)state->image->image.xsize;
	  m_coords[2].t = 0.f;
		
	  m_coords[1].s = (float)state->image->image.xsize;
	  m_coords[1].t = (float)state->image->image.ysize;
		
	  m_coords[0].s = 0.f;
	  m_coords[0].t = (float)state->image->image.ysize;
#endif
	}
      }


      if (m_buffer.csize != m_dataSize[0] ||
	  m_buffer.xsize != m_dataSize[1] ||
	  m_buffer.ysize != m_dataSize[2]){
	m_dataSize[0] = m_buffer.csize;
	m_dataSize[1] = m_buffer.xsize;
	m_dataSize[2] = m_buffer.ysize;
#ifdef GL_TEXTURE_RECTANGLE_EXT
	if (GemMan::texture_rectangle_supported)
	  glTexImage2D( GL_TEXTURE_RECTANGLE_EXT, 0,
			m_buffer.csize,
			m_buffer.xsize,
			m_buffer.ysize, 0,
			m_buffer.format,
			m_buffer.type,
			m_buffer.data);
	else
#endif
	  glTexImage2D(GL_TEXTURE_2D,   // target
		       0,               // level
		       m_buffer.csize,  // internalFormat
		       m_buffer.xsize,  // widht
		       m_buffer.ysize,  // height
		       0,               // border
		       m_buffer.format, // format
		       m_buffer.type,   // type
		       m_buffer.data);  // *pixels
      }
      // okay, load in the actual pixel data
#ifdef GL_TEXTURE_RECTANGLE_EXT
      if (GemMan::texture_rectangle_supported)
	glTexSubImage2D(GL_TEXTURE_RECTANGLE_EXT, 0,
			0, 0,				// position
			state->image->image.xsize,
			state->image->image.ysize,
			state->image->image.format,
			state->image->image.type,
			state->image->image.data);
      else
#endif
	{
	  glTexSubImage2D(GL_TEXTURE_2D, 0,
			  0, 0,				// position
			  state->image->image.xsize,
			  state->image->image.ysize,
			  state->image->image.format,
			  state->image->image.type,
			  state->image->image.data);
	}
#ifdef GL_VERSION_1_1
#elif GL_EXT_texture_object
#else
      if (creatingDispList) {
	glEndList();
      }
#endif
    }
#ifdef GL_VERSION_1_1

#elif GL_EXT_texture_object

#else
  else glCallList(m_textureObj);
#endif
  m_rebuildList = 0;
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_texture2 :: postrender(GemState *state)
{
    if (!m_textureOnOff) return;

    state->texture = 0;
#ifdef GL_TEXTURE_RECTANGLE_EXT
    if (!GemMan::texture_rectangle_supported )
        glDisable(GL_TEXTURE_2D);
    else
        glDisable(GL_TEXTURE_RECTANGLE_EXT);
#else
        glDisable(GL_TEXTURE_2D);
#endif
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_texture2 :: startRendering()
{
#ifdef GL_VERSION_1_1
    glGenTextures(1, &m_textureObj);
#ifdef GL_TEXTURE_RECTANGLE_EXT
    if (GemMan::texture_rectangle_supported)
    {
        glBindTexture(GL_TEXTURE_RECTANGLE_EXT, m_textureObj);
	setUpTextureState();
    }else
#endif
      {
    glBindTexture(GL_TEXTURE_2D, m_textureObj);
    setUpTextureState();
    }
#elif GL_EXT_texture_object
    glGenTexturesEXT(1, &m_textureObj);
    glBindTextureEXT(GL_TEXTURE_2D, m_textureObj);    
	setUpTextureState();
#else
    m_textureObj = glGenLists(1);
    m_rebuildList = 1;
#endif
	m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = -1;
	if (!m_textureObj)
	{
		error("GEM: pix_texture2: Unable to allocate texture object");
		return;
	}
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_texture2 :: stopRendering()
{
#ifdef GL_VERSION_1_1
    if (m_textureObj) glDeleteTextures(1, &m_textureObj);
#elif GL_EXT_texture_object
    if (m_textureObj) glDeleteTexturesEXT(1, &m_textureObj);
#else
    if (m_textureObj) glDeleteLists(m_textureObj, 1);
#endif
    m_textureObj = 0;
	m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = -1;
}

/////////////////////////////////////////////////////////
// textureOnOff
//
/////////////////////////////////////////////////////////
void pix_texture2 :: textureOnOff(int on)
{
    m_textureOnOff = on;
    setModified();
}

/////////////////////////////////////////////////////////
// textureQuality
//
/////////////////////////////////////////////////////////
void pix_texture2 :: textureQuality(int type)
{
    if (type)
        m_textureQuality = GL_LINEAR;
    else
        m_textureQuality = GL_NEAREST;
    if (m_textureObj)
    {
#ifdef GL_VERSION_1_1		//tigital
#ifdef GL_TEXTURE_RECTANGLE_EXT
        if (GemMan::texture_rectangle_supported)
        {
            glBindTexture(GL_TEXTURE_RECTANGLE_EXT, m_textureObj);
            glTexParameterf(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, m_textureQuality);
            glTexParameterf(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, m_textureQuality);
        }else
#endif
	  {
            glBindTexture(GL_TEXTURE_2D, m_textureObj);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_textureQuality);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_textureQuality);
        }
#elif GL_EXT_texture_object
        glBindTextureEXT(GL_TEXTURE_2D, m_textureObj);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_textureQuality);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_textureQuality);
#else
#endif
    }
    setModified();
}



/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void pix_texture2 :: obj_setupCallback(t_class *classPtr)
{
  class_addfloat(classPtr, (t_method)&pix_texture2::floatMessCallback);    
  class_addmethod(classPtr, (t_method)&pix_texture2::textureMessCallback,
		  gensym("quality"), A_FLOAT, A_NULL);
}
void pix_texture2 :: floatMessCallback(void *data, float n)
{
  GetMyClass(data)->textureOnOff((int)n);
}
void pix_texture2 :: textureMessCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->textureQuality((int)quality);
}
