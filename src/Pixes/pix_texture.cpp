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

#include "pix_texture.h"

#include "Base/GemMan.h"
#include "Base/GemPixUtil.h"
#include <string.h>

#ifdef GL_TEXTURE_RECTANGLE_EXT
//#undef GL_TEXTURE_RECTANGLE_EXT
#endif
CPPEXTERN_NEW(pix_texture)

static inline int powerOfTwo(int value){
  int x = 1;
  while(x < value) x <<= 1;
  return(x);  
}

/////////////////////////////////////////////////////////
//
// pix_texture
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_texture :: pix_texture()
  : m_textureOnOff(1), 
    m_textureQuality(GL_LINEAR), m_repeat(GL_REPEAT), m_rebuildList(0), m_textureObj(0),m_textureType( GL_TEXTURE_2D )
     
{
  m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = -1;
  m_buffer.xsize = m_buffer.ysize = m_buffer.csize = -1;
  m_buffer.data = NULL;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_texture :: ~pix_texture()
{ }

/////////////////////////////////////////////////////////
// setUpTextureState
//
/////////////////////////////////////////////////////////
void pix_texture :: setUpTextureState() {
//#ifdef GL_TEXTURE_RECTANGLE_EXT
if (m_mode && GemMan::texture_rectangle_supported){
  if ( m_textureType ==  GL_TEXTURE_RECTANGLE_EXT)				//tigital
    glTexParameterf(m_textureType, GL_TEXTURE_PRIORITY, 0.0f);
    post("pix_texture: using rectangle texture");
    }
//#endif
#ifdef GL_UNPACK_CLIENT_STORAGE_APPLE
  if (GemMan::client_storage_supported){
    glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, GL_TRUE);
    post("pix_texture: using client storage");}
  else
#endif // CLIENT_STORAGE
    glPixelStoref(GL_UNPACK_ALIGNMENT, 1);

  glTexParameterf(m_textureType, GL_TEXTURE_MIN_FILTER, m_textureQuality);
  glTexParameterf(m_textureType, GL_TEXTURE_MAG_FILTER, m_textureQuality);
  glTexParameterf(m_textureType, GL_TEXTURE_WRAP_S, m_repeat);
  glTexParameterf(m_textureType, GL_TEXTURE_WRAP_T, m_repeat);


//#ifdef GL_TEXTURE_RECTANGLE_EXT
if (m_mode)
  if ( m_textureType !=  GL_TEXTURE_RECTANGLE_EXT)
//#endif
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  // note:  On MacOS X, pix_texture is used for power of two/normalised textures, so 
  //		texture_rectangle can't be used (otherwise, we'd just see one pixel!)
}

inline int setTexCoords(TexCoord *coords, float xRatio, float yRatio){
#ifndef MACOSX
      coords[0].s = 0.f;
      coords[0].t = 0.f;
      coords[1].s = xRatio;
      coords[1].t = 0.f;
      coords[2].s = xRatio;
      coords[2].t = yRatio;
      coords[3].s = 0.f;
      coords[3].t = yRatio;
#else // MACOSX
      coords[3].s = 0.f;		// switched the order of coords on MACOSX
      coords[3].t = 0.f;		// otherwise we'd be upside down!
      coords[2].s = xRatio;
      coords[2].t = 0.f;
      coords[1].s = xRatio;
      coords[1].t = yRatio;
      coords[0].s = 0.f;
      coords[0].t = yRatio;
#endif // MACOSX
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
#if 1
void pix_texture :: render(GemState *state) {
  if ( !state->image || !m_textureOnOff) return;
  if(!&state->image->image || !state->image->image.data)return;
  state->texture = 1;
  state->texCoords = m_coords;
  state->numTexCoords = 4;

  if (state->image->newimage) m_rebuildList = 1;

  int x_2 = powerOfTwo(state->image->image.xsize);
  int y_2 = powerOfTwo(state->image->image.ysize);

  bool normalized = ((state->image->image.xsize==x_2) && (state->image->image.ysize==y_2));

#ifdef GL_VERSION_1_1
  int texType = m_textureType;

//#ifdef GL_TEXTURE_RECTANGLE_EXT
if (m_mode){
  if (!normalized && GemMan::texture_rectangle_supported ){
    m_textureType = GL_TEXTURE_RECTANGLE_EXT;
    normalized = 0;
  } 
  } else {
//#endif
    m_textureType = GL_TEXTURE_2D;
    normalized = 0;
    }
  if (m_textureType!=texType){
  post("texType != m_textureType");
    stopRendering();startRendering();
  }
  glEnable(m_textureType);
  glBindTexture(m_textureType, m_textureObj);
#elif GL_EXT_texture_object
  glEnable(m_textureType);
  glBindTextureEXT(m_textureType, m_textureObj);
#else
  // can we build a display list?
  int creatingDispList = 0;
  if (!state->inDisplayList && m_rebuildList) {
    glNewList(m_textureObj, GL_COMPILE_AND_EXECUTE);
    creatingDispList = 1;
  }
  setUpTextureState();
#endif

  if (m_rebuildList) {
    if (normalized) {
     // post("normalized");
      m_buffer.xsize = state->image->image.xsize;
      m_buffer.ysize = state->image->image.ysize;
      m_buffer.csize  = state->image->image.csize;
      m_buffer.format = state->image->image.format;
      m_buffer.type   = state->image->image.type;
      m_buffer.reallocate();
      setTexCoords(m_coords, 1.0, 1.0);
      state->texCoords = m_coords;
      state->numTexCoords = 4;
      if (m_buffer.csize != m_dataSize[0] ||
	  m_buffer.xsize != m_dataSize[1] ||
	  m_buffer.ysize != m_dataSize[2]){
	m_dataSize[0] = m_buffer.csize;
	m_dataSize[1] = m_buffer.xsize;
	m_dataSize[2] = m_buffer.ysize;

	// following could be done directly with state->image->image...
	glTexImage2D(m_textureType, 0, 
		     m_buffer.csize,
		     m_buffer.xsize,
		     m_buffer.ysize, 0,
		     m_buffer.format,
		     m_buffer.type,
		     m_buffer.data);
      }
      // okay, load in the actual pixel data
      
      glTexSubImage2D(m_textureType, 0,
		      0, 0,				// position
		      state->image->image.xsize,
		      state->image->image.ysize,
		      state->image->image.format,
		      state->image->image.type,
		      state->image->image.data);
      
    } else { // !normalized
     // post("!normalized");
      float m_xRatio = (float)state->image->image.xsize;
      float m_yRatio = (float)state->image->image.ysize;
      if ( !GemMan::texture_rectangle_supported || !m_mode ) {
     
	m_xRatio /= (float)x_2;
	m_yRatio /= (float)y_2;
	m_buffer.xsize = x_2;
	m_buffer.ysize = y_2;
      } else {
	m_buffer.xsize = state->image->image.xsize;
	m_buffer.ysize = state->image->image.ysize;
      }
      m_buffer.csize  = state->image->image.csize;
      m_buffer.format = state->image->image.format;
      m_buffer.type   = state->image->image.type;
      m_buffer.reallocate();
	//memset(m_buffer.data, 0, m_buffer.xsize*m_buffer.ysize*m_buffer.csize*sizeof(unsigned char));
      setTexCoords(m_coords, m_xRatio, m_yRatio);
      state->texCoords = m_coords;
      state->numTexCoords = 4;

      if (m_buffer.csize != m_dataSize[0] ||
	  m_buffer.xsize != m_dataSize[1] ||
	  m_buffer.ysize != m_dataSize[2]){
            m_dataSize[0] = m_buffer.csize;
            m_dataSize[1] = m_buffer.xsize;
            m_dataSize[2] = m_buffer.ysize;
            
            if (m_buffer.csize == 2 && !m_mode){
                int datasize=m_buffer.xsize*m_buffer.ysize*m_buffer.csize/4;
                unsigned char* dummy=m_buffer.data;
                while(datasize--){
                *dummy++ = 128;
                *dummy++ = 0;
                *dummy++ = 128;
                *dummy++ = 0;
                }
                post("pix_texture: zeroing YUV buffer");
            }

	glTexImage2D(m_textureType, 0,
		     m_buffer.csize,
		     m_buffer.xsize,
		     m_buffer.ysize, 0,
		     m_buffer.format,
		     m_buffer.type,
		     m_buffer.data);
        post("pix_texture: TexImage2D");
      }
      // okay, load in the actual pixel data
      
      glTexSubImage2D(m_textureType, 0,
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
    if (creatingDispList)
      glEndList();
#endif
  }
#ifdef GL_VERSION_1_1
#elif GL_EXT_texture_object
#else
  else glCallList(m_textureObj);
#endif
  m_rebuildList = 0;
}
#endif


/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_texture :: postrender(GemState *state){
  if (!m_textureOnOff) return;
  
  state->texture = 0;
  glDisable(m_textureType);
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_texture :: startRendering()
{
#ifdef GL_VERSION_1_1
  glGenTextures(1, &m_textureObj); // this crashes sometimes!!!! (jmz)
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
  m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = -1;

  if (!m_textureObj)	{
    error("GEM: pix_texture: Unable to allocate texture object");
    return;
  }
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_texture :: stopRendering()
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
void pix_texture :: textureOnOff(int on)
{
  m_textureOnOff = on;
  setModified();
}

/////////////////////////////////////////////////////////
// textureQuality
//
/////////////////////////////////////////////////////////
void pix_texture :: textureQuality(int type)
{
  if (type)
    m_textureQuality = GL_LINEAR;
  else
    m_textureQuality = GL_NEAREST;
  if (m_textureObj) {
#ifdef GL_VERSION_1_1
    glBindTexture(m_textureType, m_textureObj);
    glTexParameterf(m_textureType, GL_TEXTURE_MAG_FILTER, m_textureQuality);
    glTexParameterf(m_textureType, GL_TEXTURE_MIN_FILTER, m_textureQuality);
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
// textureQuality
//
/////////////////////////////////////////////////////////
void pix_texture :: repeatMess(int type)
{
  if (type)
    m_repeat = GL_REPEAT;
  else
    m_repeat = GL_CLAMP_TO_EDGE;
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
void pix_texture :: obj_setupCallback(t_class *classPtr)
{
  class_addfloat(classPtr, (t_method)&pix_texture::floatMessCallback);    
  class_addmethod(classPtr, (t_method)&pix_texture::textureMessCallback,
		  gensym("quality"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_texture::repeatMessCallback,
		  gensym("repeat"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_texture::modeCallback,
		  gensym("mode"), A_FLOAT, A_NULL);
  class_addcreator(_classpix_texture,gensym("pix_texture2"),A_NULL); 
}
void pix_texture :: floatMessCallback(void *data, float n)
{
  GetMyClass(data)->textureOnOff((int)n);
}
void pix_texture :: textureMessCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->textureQuality((int)quality);
}
void pix_texture :: repeatMessCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->repeatMess((int)quality);
}

void pix_texture :: modeCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->m_mode=((int)quality);
}

