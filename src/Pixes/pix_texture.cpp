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
//    Copyright (c) 2002-2003 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_texture.h"

#include "Base/GemMan.h"
#include "Base/GemPixUtil.h"
#include <string.h>

#ifdef debug
# undef debug
#endif

//#define DEBUG_ME

#ifdef DEBUG_ME
# define debug post
#else
# define debug
#endif

CPPEXTERN_NEW(pix_texture)

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
    m_textureQuality(GL_LINEAR), m_repeat(GL_REPEAT),
    m_rebuildList(0), m_textureObj(0),m_textureType( GL_TEXTURE_2D ),
    m_mode(0),
    m_clientStorage(0), //have to do this due to texture corruption issues
    m_yuv(1)
{
  m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = -1;
  m_buffer.xsize = m_buffer.ysize = m_buffer.csize = -1;
  m_buffer.data = NULL;
  
  #if defined(GL_TEXTURE_RECTANGLE_EXT) 
  //|| defined(GL_NV_TEXTURE_RECTANGLE)
  m_mode = 1;  //default to the fastest mode for systems that support it
  #endif
  
  // create an outlet to send texture ID
  m_outTexID = outlet_new(this->x_obj, &s_float);
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
#ifdef GL_TEXTURE_RECTANGLE_EXT
  if (m_mode && GemMan::texture_rectangle_supported){
    if ( m_textureType ==  GL_TEXTURE_RECTANGLE_EXT)
      glTexParameterf(m_textureType, GL_TEXTURE_PRIORITY, 0.0f);
    // JMZ: disabled the following, as rectangle-textures are clamped anyhow
    // JMZ: and normalized ones, lose their setting 
	//  m_repeat = GL_CLAMP_TO_EDGE;
      debug("pix_texture: using rectangle texture");
  }
#endif // GL_TEXTURE_RECTANGLE_EXT

#ifdef GL_UNPACK_CLIENT_STORAGE_APPLE
  if (GemMan::client_storage_supported && m_clientStorage){
    glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, GL_TRUE);
    debug("pix_texture: using client storage");
  }

  else {
    glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, GL_FALSE);
    glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
    debug("pix_texture: not using client storage");
  }

#else
  
  glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
  
#endif // CLIENT_STORAGE
    

  glTexParameterf(m_textureType, GL_TEXTURE_MIN_FILTER, m_textureQuality);
  glTexParameterf(m_textureType, GL_TEXTURE_MAG_FILTER, m_textureQuality);
  glTexParameterf(m_textureType, GL_TEXTURE_WRAP_S, m_repeat);
  glTexParameterf(m_textureType, GL_TEXTURE_WRAP_T, m_repeat);
  

#ifdef GL_TEXTURE_RECTANGLE_EXT
  if (m_mode)
    if ( m_textureType !=  GL_TEXTURE_RECTANGLE_EXT)
#endif //GL_TEXTURE_RECTANGLE_EXT
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

// upsidedown is derived from the imageStruct.upsidedown
// use this when loading images...
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
void pix_texture :: render(GemState *state) {
  if (!state->image        || !m_textureOnOff          )return;
  if(!&state->image->image || !state->image->image.data)return;
  GLboolean upsidedown = state->image->image.upsidedown;
  state->texCoords = m_coords;
  state->numTexCoords = 4;
  if (state->image->newimage) m_rebuildList = 1;

  m_imagebuf.xsize =state->image->image.xsize;
  m_imagebuf.ysize =state->image->image.ysize;
  m_imagebuf.csize =state->image->image.csize;
  m_imagebuf.format=state->image->image.format;
  m_imagebuf.type  =state->image->image.type;
  m_imagebuf.data  =state->image->image.data;

  int x_2 = powerOfTwo(m_imagebuf.xsize);
  int y_2 = powerOfTwo(m_imagebuf.ysize);

  bool normalized = ((m_imagebuf.xsize==x_2) && (m_imagebuf.ysize==y_2));

  debug("normalized=%d\t%d - %d\t%d - %d", normalized, m_imagebuf.xsize, x_2, m_imagebuf.ysize, y_2);

#ifdef GL_VERSION_1_1
    int texType = m_textureType;


#ifdef GL_TEXTURE_RECTANGLE_EXT
    if (m_mode){
	if (/*!normalized &&*/ GemMan::texture_rectangle_supported ){
	    m_textureType = GL_TEXTURE_RECTANGLE_EXT;
	    debug("pix_texture:  using GL_TEXTURE_RECTANGLE_EXT");
	    normalized = 0;
	}
    } else 
#endif // GL_TEXTURE_RECTANGLE_EXT
    {
	m_textureType = GL_TEXTURE_2D;
	debug("pix_texture:  using GL_TEXTURE_2D");
	normalized = 0;
    }
    if (m_textureType!=texType){
	debug("pix_texture:  texType != m_textureType");
	stopRendering();startRendering();
    }
    
    glEnable(m_textureType);
    glBindTexture(m_textureType, m_textureObj);
    
#ifdef GL_APPLE_texture_range
    if (state->image->newfilm ){
	if ( GemMan::texture_range_supported && GemMan::texture_rectangle_supported && m_mode){
	    glTextureRangeAPPLE( GL_TEXTURE_RECTANGLE_EXT, 
			    m_imagebuf.xsize * m_imagebuf.ysize * m_imagebuf.csize, 
			    m_imagebuf.data );
	    debug("pix_texture:  using glTextureRangeAPPLE()");
	}else{
	    glTextureRangeAPPLE( GL_TEXTURE_RECTANGLE_EXT, 0, NULL );
	}
	glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_STORAGE_HINT_APPLE, GL_STORAGE_SHARED_APPLE );
	// GL_STORAGE_SHARED_APPLE -  AGP texture path
	// GL_STORAGE_CACHED_APPLE - VRAM texture path
	// GL_STORAGE_PRIVATE_APPLE - normal texture path
    }
#endif // GL_APPLE_texture_range

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
#endif //GL_VERSION_1_1

    if (m_rebuildList) {
	// if YUV is not supported on this platform, we have to convert it to RGB
	//(skip Alpha since it isnt used)
	const bool do_yuv = m_yuv && GemMan::texture_yuv_supported;
	if (!do_yuv && m_imagebuf.format == GL_YUV422_GEM){
	    m_imagebuf.format=GL_RGB;
	    m_imagebuf.csize=3;
	    m_imagebuf.reallocate();
	    m_imagebuf.fromYUV422(state->image->image.data);
	}
	if (normalized) {
	    m_buffer.xsize = m_imagebuf.xsize;
	    m_buffer.ysize = m_imagebuf.ysize;
	    m_buffer.csize  = m_imagebuf.csize;
	    m_buffer.format = m_imagebuf.format;
	    m_buffer.type   = m_imagebuf.type;
	    m_buffer.reallocate();
	    setTexCoords(m_coords, 1.0, 1.0, upsidedown);
	    state->texCoords = m_coords;
	    state->numTexCoords = 4;
	    if (m_buffer.csize != m_dataSize[0] ||
		m_buffer.xsize != m_dataSize[1] ||
		m_buffer.ysize != m_dataSize[2]){
		m_dataSize[0] = m_buffer.csize;
		m_dataSize[1] = m_buffer.xsize;
		m_dataSize[2] = m_buffer.ysize;

	    }
	    //if the texture is a power of two in size then there is no need to subtexture
	    glTexImage2D(m_textureType, 0, 
		     m_imagebuf.csize,
		     m_imagebuf.xsize,
		     m_imagebuf.ysize, 0,
		     m_imagebuf.format,
		     m_imagebuf.type,
		     m_imagebuf.data);
                     
	} else { // !normalized
	    float m_xRatio = (float)m_imagebuf.xsize;
	    float m_yRatio = (float)m_imagebuf.ysize;
	    if ( !GemMan::texture_rectangle_supported || !m_mode ) {
		m_xRatio /= (float)x_2;
		m_yRatio /= (float)y_2;
		m_buffer.xsize = x_2;
		m_buffer.ysize = y_2;
	    } else {
		m_buffer.xsize = m_imagebuf.xsize;
		m_buffer.ysize = m_imagebuf.ysize;
	    }
	    m_buffer.csize  = m_imagebuf.csize;
	    m_buffer.format = m_imagebuf.format;
	    m_buffer.type   = m_imagebuf.type;
	    m_buffer.reallocate();
	    setTexCoords(m_coords, m_xRatio, m_yRatio, upsidedown);
	    state->texCoords = m_coords;
	    state->numTexCoords = 4;

	    if (m_buffer.csize != m_dataSize[0] ||
		m_buffer.xsize != m_dataSize[1] ||
		m_buffer.ysize != m_dataSize[2]){
		m_dataSize[0] = m_buffer.csize;
		m_dataSize[1] = m_buffer.xsize;
		m_dataSize[2] = m_buffer.ysize; 
     
            
		if (m_buffer.format == GL_YUV422_GEM && !m_mode)m_buffer.setBlack();

	    //this is for dealing with power of 2 textures which need a buffer that's 2^n
	    if ( !GemMan::texture_rectangle_supported || !m_mode ) {            
		glTexImage2D(	m_textureType, 0,
				//m_buffer.csize,
				GL_RGBA,
				m_buffer.xsize,
				m_buffer.ysize, 0,
				m_buffer.format,
				m_buffer.type,
				m_buffer.data);
     
		debug("pix_texture: TexImage2D non rectangle");
        }
        else //this deals with rectangle textures that are h*w
        { 
            glTexImage2D(m_textureType, 0,
			 //  m_buffer.csize,
			 GL_RGBA,
			 m_imagebuf.xsize,
			 m_imagebuf.ysize, 0,
			 m_imagebuf.format,
			 m_imagebuf.type,
			 m_imagebuf.data); 
	    debug("pix_texture: TexImage2D  rectangle");
        }
        
      } //end of loop if size has changed
      
      // okay, load in the actual pixel data
      
      //when doing rectangle textures the buffer changes after every film is loaded this call makes sure the 
      //texturing is updated as well to prevent crashes
      if (state->image->newfilm ){
            glTexImage2D(m_textureType, 0,
			 //  m_buffer.csize,  //this is completely wrong btw
			 GL_RGBA, 		//this is the correct internal format for YUV
			 m_imagebuf.xsize,
			 m_imagebuf.ysize, 0,
			 m_imagebuf.format,
			 m_imagebuf.type,
			 m_imagebuf.data);
	    debug("pix_texture: new film");
            state->image->newfilm = 0; //just to be sure
      } 
      glTexSubImage2D(m_textureType, 0,
		      0, 0,				// position
		      m_imagebuf.xsize,
		      m_imagebuf.ysize,
		      m_imagebuf.format,
		      m_imagebuf.type,
		      m_imagebuf.data);
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
  
  state->texture = 1;

#ifdef GL_TEXTURE_RECTANGLE_EXT
  // if we are using rectangle textures, this is a way to inform the downstream objects 
  // (this is important for things like [pix_coordinate]
  if(m_textureType==GL_TEXTURE_RECTANGLE_EXT)state->texture=2;
#endif
  
  // send textureID to outlet
  outlet_float(m_outTexID, (t_float)m_textureObj);
}



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
void pix_texture :: obj_setupCallback(t_class *classPtr)
{
  class_addfloat(classPtr, (t_method)&pix_texture::floatMessCallback);    
  class_addmethod(classPtr, (t_method)&pix_texture::textureMessCallback,
		  gensym("quality"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_texture::repeatMessCallback,
		  gensym("repeat"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_texture::modeCallback,
		  gensym("mode"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_texture::clientStorageCallback,
		  gensym("client_storage"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_texture::yuvCallback,
		  gensym("yuv"), A_FLOAT, A_NULL);
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
  GetMyClass(data)->m_rebuildList=1;

}

void pix_texture :: clientStorageCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->m_clientStorage=((int)quality);
}

void pix_texture :: yuvCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->m_yuv=((int)quality);
}

