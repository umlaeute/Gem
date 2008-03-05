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
//    Copyright (c) 2002-2006 James Tittle & Chris Clepper
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
    m_didTexture(false), m_rebuildList(0), 
    m_textureObj(0), 
    m_extTextureObj(0), m_extWidth(1.), m_extHeight(1.), m_extType(GL_TEXTURE_2D),
    m_extUpsidedown(false),
    m_realTextureObj(0),
    m_oldTexCoords(NULL), m_oldNumCoords(0), m_oldTexture(0), 
    m_textureType( GL_TEXTURE_2D ),
    m_rectangle(0), m_env(GL_MODULATE),
    m_clientStorage(0), //have to do this due to texture corruption issues
    m_yuv(1),
    m_texunit(0)
{
  m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = -1;
  m_buffer.xsize = m_buffer.ysize = m_buffer.csize = -1;
  m_buffer.data = NULL;
  
  //rectangle textures by default only for OSX since there are too many busted drivers in use on Windows and Linux
#ifdef __APPLE__
  m_rectangle = 1;  //default to the fastest mode for systems that support it
  m_textureType = GL_TEXTURE_RECTANGLE_ARB;
#endif

  // create an inlet to receive external texture IDs
  m_inTexID  = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("extTexture"));

  // create an outlet to send texture ID
  m_outTexID = outlet_new(this->x_obj, &s_float);
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_texture :: ~pix_texture()
{
  if(m_inTexID) inlet_free (m_inTexID);
  if(m_outTexID)outlet_free(m_outTexID);

  m_inTexID=NULL;
  m_outTexID=NULL;
}

////////////////////////////////////////////////////////
// setUpTextureState
//
/////////////////////////////////////////////////////////
void pix_texture :: setUpTextureState() {
  if (m_rectangle && GemMan::texture_rectangle_supported){
    if ( m_textureType ==  GL_TEXTURE_RECTANGLE_ARB || m_textureType == GL_TEXTURE_RECTANGLE_EXT)
      {
        glTexParameterf(m_textureType, GL_TEXTURE_PRIORITY, 0.0f);
        // JMZ: disabled the following, as rectangle-textures are clamped anyhow
        // JMZ: and normalized ones, lose their setting
        // TIGITAL: this is necessary on osx, at least with non-powerof2 textures!
        //			otherwise, weird texturing occurs (looks similar to pix_refraction)
        // NPOT: GL_CLAMP, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
        // POT:  above plus GL_REPEAT, GL_MIRRORED_REPEAT
        m_repeat = GL_CLAMP_TO_EDGE;
        debug("using rectangle texture");
      }
  }


  if (GemMan::client_storage_supported){
    if(m_clientStorage){
      glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, GL_TRUE);
      debug("using client storage");
    } else {
      glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, GL_FALSE);
      debug("not using client storage");
    }
  } else   
    glPixelStoref(GL_UNPACK_ALIGNMENT, 1);

  glTexParameterf(m_textureType, GL_TEXTURE_MIN_FILTER, m_textureQuality);
  glTexParameterf(m_textureType, GL_TEXTURE_MAG_FILTER, m_textureQuality);
  glTexParameterf(m_textureType, GL_TEXTURE_WRAP_S, m_repeat);
  glTexParameterf(m_textureType, GL_TEXTURE_WRAP_T, m_repeat);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_env);
}

////////////////////////////////////////////////////////
// setTexCoords
//
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


////////////////////////////////////////////////////////
// extension check
//
/////////////////////////////////////////////////////////
bool pix_texture :: isRunnable(void) {
  /* for simplicity's sake, i have dropped support for very old openGL-versions */
  if(GLEW_VERSION_1_1)
    return true;

  error("need at least openGL-1.1 for texturing! refusing to work");
}

////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_texture :: render(GemState *state) {
  m_didTexture=0;

  m_oldTexCoords=state->texCoords;
  m_oldNumCoords=state->numTexCoords;
  m_oldTexture  =state->texture;

  if(!m_textureOnOff)return;

  GLboolean upsidedown=false;
  GLboolean normalized=true;
  int texType = m_textureType;
  int x_2, y_2;
  GLboolean useExternalTexture=false;
  GLfloat xTex=1., yTex=1.;
  int do_rectangle = (m_rectangle)?GemMan::texture_rectangle_supported:0;

  if (!state->image || !state->image->image.data){
    if(m_extTextureObj>0) {
      useExternalTexture= true;
      m_rebuildList     = false;
      m_textureObj      = m_extTextureObj;
      if(m_extType)m_textureType=m_extType;
      texType=m_textureType;
      upsidedown=m_extUpsidedown;
      m_xRatio=m_extWidth;
      m_yRatio=m_extHeight;
      setTexCoords(m_coords, m_xRatio, m_yRatio, upsidedown);
    } else
      /* neither do we have an image nor an external texture */
      return;
  }

  state->texCoords = m_coords;
  state->numTexCoords = 4;

  if(!useExternalTexture){
    upsidedown = state->image->image.upsidedown;
    if (state->image->newimage) m_rebuildList = 1;

    m_imagebuf.xsize =state->image->image.xsize;
    m_imagebuf.ysize =state->image->image.ysize;
    m_imagebuf.csize =state->image->image.csize;
    m_imagebuf.format=state->image->image.format;
    m_imagebuf.type  =state->image->image.type;
    m_imagebuf.data  =state->image->image.data;
    
    x_2 = powerOfTwo(m_imagebuf.xsize);
    y_2 = powerOfTwo(m_imagebuf.ysize);

    normalized = ((m_imagebuf.xsize==x_2) && (m_imagebuf.ysize==y_2));

    debug("normalized=%d\t%d - %d\t%d - %d", normalized, m_imagebuf.xsize, x_2, m_imagebuf.ysize, y_2);
  }

  if(!useExternalTexture){
    switch(do_rectangle) {
    case 2:
      m_textureType = GL_TEXTURE_RECTANGLE_ARB;
      debug("using mode 1:GL_TEXTURE_RECTANGLE_ARB");
      normalized = 0;
      break;
    case 1:
      m_textureType = GL_TEXTURE_RECTANGLE_EXT;
      debug("using mode 1:GL_TEXTURE_RECTANGLE_EXT");
      normalized = 0;
      break;
    default:
      m_textureType = GL_TEXTURE_2D;
      debug("using mode 0:GL_TEXTURE_2D");
      normalized = 0;
      break;
    }
  }
  if (m_textureType!=texType){
    debug("texType != m_textureType");
    stopRendering();startRendering();
  }   

  if(GLEW_VERSION_1_3) {
    glActiveTexture(GL_TEXTURE0_ARB + m_texunit);
  }
  glEnable(m_textureType);
  glBindTexture(m_textureType, m_textureObj);
  
  state->multiTexUnits = 8;

  if ((!useExternalTexture)&&state->image->newfilm ){
    //  tigital:  shouldn't we also allow TEXTURE_2D here?
    if(NULL!=glTextureRangeAPPLE) {
      if ( GLEW_APPLE_texture_range ){
        if(glTextureRangeAPPLE == NULL) {
          glTextureRangeAPPLE( m_textureType, 
                               m_imagebuf.xsize * m_imagebuf.ysize * m_imagebuf.csize, 
                               m_imagebuf.data );
          debug("using glTextureRangeAPPLE()");
        }else{
          glTextureRangeAPPLE( m_textureType, 0, NULL );
        }
      }
    }

    /* hmm, GL_TEXTURE_STORAGE_HINT_APPLE throws a GL-error on linux (and probably on w32 too)
     * how to do a run-time check for it?
     *
     * according to http://developer.apple.com/documentation/GraphicsImaging/Conceptual/OpenGL-MacProgGuide/opengl_texturedata/chapter_10_section_2.html
     * this seems to be a part of the texture_range extension, so we check for that!
     */
    if(GLEW_APPLE_texture_range)
       glTexParameteri( m_textureType, GL_TEXTURE_STORAGE_HINT_APPLE, GL_STORAGE_SHARED_APPLE );
    // GL_STORAGE_SHARED_APPLE -  AGP texture path
    // GL_STORAGE_CACHED_APPLE - VRAM texture path
    // GL_STORAGE_PRIVATE_APPLE - normal texture path


    if(m_clientStorage) glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, GL_TRUE);
  }

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
      m_xRatio=1.0;
      m_yRatio=1.0;
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
      m_xRatio = (float)m_imagebuf.xsize;
      m_yRatio = (float)m_imagebuf.ysize;
      if ( !do_rectangle ) {
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
     
            
        if (m_buffer.format == GL_YUV422_GEM && !m_rectangle)m_buffer.setBlack();

        //this is for dealing with power of 2 textures which need a buffer that's 2^n
        if ( !do_rectangle ) {            
          glTexImage2D(	m_textureType, 0,
                        //m_buffer.csize,
                        GL_RGBA,
                        m_buffer.xsize,
                        m_buffer.ysize, 0,
                        m_buffer.format,
                        m_buffer.type,
                        m_buffer.data);
     
          debug("TexImage2D non rectangle");
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
            debug("TexImage2D  rectangle");
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
        debug("new film");
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
    
  }
  m_rebuildList = 0;
 
  state->texture = 1;

  // if we are using rectangle textures, this is a way to inform the downstream objects 
  // (this is important for things like [pix_coordinate]
  if(m_textureType==GL_TEXTURE_RECTANGLE_ARB)state->texture=2;

  // if we are using rectangle textures, this is a way to inform the downstream objects 
  // (this is important for things like [pix_coordinate]
  if(m_textureType==GL_TEXTURE_RECTANGLE_EXT)state->texture=2;

  m_didTexture=1;
  
  // send textureID to outlet
  if(m_textureObj){
    t_atom ap[5];
    SETFLOAT(ap, (t_float)m_textureObj);
    SETFLOAT(ap+1, (t_float)m_xRatio);
    SETFLOAT(ap+2, (t_float)m_yRatio);
    SETFLOAT(ap+3, (t_float)m_textureType);
    SETFLOAT(ap+4, (t_float)upsidedown);
    outlet_list(m_outTexID, &s_list, 5, ap);
  }
}

////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_texture :: postrender(GemState *state){

  state->texCoords   = m_oldTexCoords;
  state->numTexCoords= m_oldNumCoords;
  state->texture     = m_oldTexture;

  if (m_didTexture){
    if(GLEW_VERSION_1_3) {
      glActiveTexture(GL_TEXTURE0_ARB + m_texunit);  //needed?
    }
    glDisable(m_textureType);
  }

}

////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_texture :: startRendering()
{
  glGenTextures(1, &m_realTextureObj); // this crashes sometimes!!!! (jmz)
  if(GLEW_VERSION_1_3) {
    glActiveTexture(GL_TEXTURE0_ARB + m_texunit);
  }
  glBindTexture(m_textureType, m_realTextureObj);
  m_textureObj=m_realTextureObj;
  setUpTextureState();

  m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = -1;

  if (!m_realTextureObj)	{
    error("GEM: pix_texture: Unable to allocate texture object");
    return;
  }
}

////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_texture :: stopRendering()
{
  if(m_realTextureObj) {
    glDeleteTextures(1, &m_realTextureObj);
    
    m_realTextureObj = 0;
    m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = -1;
  }
}

////////////////////////////////////////////////////////
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
    if(GLEW_VERSION_1_3) {
      glActiveTexture(GL_TEXTURE0_ARB + m_texunit);
    }
    glBindTexture(m_textureType, m_textureObj);
    glTexParameterf(m_textureType, GL_TEXTURE_MAG_FILTER, m_textureQuality);
    glTexParameterf(m_textureType, GL_TEXTURE_MIN_FILTER, m_textureQuality);
  }
  setModified();
}

////////////////////////////////////////////////////////
// textureQuality
//
/////////////////////////////////////////////////////////
void pix_texture :: textureRectangle(int rect)
{
  m_rectangle=rect;
  if (m_rectangle)
    post("using mode 1: TEXTURE_RECTANGLE");
  else
    post("using mode 0: TEXTURE_2D");

  m_rebuildList=1;
}

////////////////////////////////////////////////////////
// texture repeat message
//
/////////////////////////////////////////////////////////
void pix_texture :: repeatMess(int type)
{
  if (type)
    m_repeat = GL_REPEAT;
  else {
    if(GLEW_EXT_texture_edge_clamp)
      m_repeat = GL_CLAMP_TO_EDGE;
    else
      m_repeat = GL_CLAMP;
  }

  if (m_textureObj) {
    if(GLEW_VERSION_1_1) {
      glBindTexture(m_textureType, m_textureObj);
      glTexParameterf(m_textureType, GL_TEXTURE_WRAP_S, m_repeat);
      glTexParameterf(m_textureType, GL_TEXTURE_WRAP_T, m_repeat);
    } else {
      glBindTextureEXT(m_textureType, m_textureObj);
      glTexParameteri(m_textureType, GL_TEXTURE_WRAP_S, m_repeat);
      glTexParameteri(m_textureType, GL_TEXTURE_WRAP_T, m_repeat);
    }
  }
  setModified();
}

////////////////////////////////////////////////////////
// texture environment mode
//
/////////////////////////////////////////////////////////
void pix_texture :: envMess(int num)
{
  switch (num) {
  case 0:
    m_env = GL_REPLACE;
    break;
  case 1:
    m_env = GL_DECAL;
    break;
  case 2:
    m_env = GL_BLEND;
    break;
  case 3:
    m_env = GL_ADD;
    break;
  case 4:
    m_env = GL_COMBINE;
    break;
  default:
    m_env = GL_MODULATE;
  }
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_env);
  setModified();
}

////////////////////////////////////////////////////////
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
  class_addmethod(classPtr, (t_method)&pix_texture::envMessCallback,
                  gensym("env"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_texture::modeCallback,
                  gensym("mode"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_texture::modeCallback,
                  gensym("rectangle"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_texture::clientStorageCallback,
                  gensym("client_storage"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_texture::yuvCallback,
                  gensym("yuv"), A_FLOAT, A_NULL);

  class_addmethod(classPtr, (t_method)&pix_texture::extTextureCallback,
                  gensym("extTexture"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_texture::texunitCallback,
                  gensym("texunit"), A_FLOAT, A_NULL);
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
void pix_texture :: envMessCallback(void *data, t_floatarg num )
{
  GetMyClass(data)->envMess((int) num);
}
void pix_texture :: modeCallback(void *data, t_floatarg rectangle)
{
  GetMyClass(data)->textureRectangle((int)rectangle);
}

void pix_texture :: clientStorageCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->m_clientStorage=((int)quality);
}

void pix_texture :: yuvCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->m_yuv=((int)quality);
}

void pix_texture :: extTextureCallback(void *data, t_symbol*s, int argc, t_atom*argv)
{
  int index=5;
  switch(argc){
  case 5:
    if(A_FLOAT!=argv[4].a_type)break;
    GetMyClass(data)->m_extUpsidedown=atom_getint(argv+4);
  case 4:
    index=4;
    if(A_FLOAT!=argv[3].a_type)break;
    GetMyClass(data)->m_extType=atom_getint(argv+3);
  case 3:
    index=3;
    if(A_FLOAT!=argv[2].a_type)break;
    index=2;
    if(A_FLOAT!=argv[1].a_type)break;
    GetMyClass(data)->m_extWidth =atom_getfloat(argv+1);
    GetMyClass(data)->m_extHeight=atom_getfloat(argv+2);
  case 1:
    index=1;
    if(A_FLOAT!=argv[0].a_type)break;
    GetMyClass(data)->m_extTextureObj=atom_getint(argv+0);
    index=0;
    return;
  default:
    GetMyClass(data)->error("arguments: <texId> [<width> <height> [<type> [<upsidedown>]]]");
    return;
  }
  if(index)
    GetMyClass(data)->error("invalid type of argument #%d", index);


}
void pix_texture :: texunitCallback(void *data, t_floatarg unit)
{
  GetMyClass(data)->m_texunit=(int)unit;
}
