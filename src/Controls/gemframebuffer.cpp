////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// tigital AT mac DOT com
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2005-2006 James Tittle II, tigital At mac DoT com
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "gemframebuffer.h"
#include <string.h>


CPPEXTERN_NEW_WITH_TWO_ARGS(gemframebuffer, t_symbol *, A_DEFSYMBOL, t_symbol *, A_DEFSYMBOL)

/////////////////////////////////////////////////////////
//
// gemframebuffer
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemframebuffer :: gemframebuffer()
  : m_haveinit(false), m_wantinit(false), m_frameBufferIndex(0), m_depthBufferIndex(0),
    m_offScreenID(0), m_texTarget(GL_TEXTURE_2D), m_texunit(0),
    m_width(256), m_height(256),
m_mode(0), m_internalformat(GL_RGB8), m_format(GL_RGB), m_type(GL_UNSIGNED_BYTE)
	
{
  // create an outlet to send out texture info:
  //  - ID
  //  - width & height
  //  - format/type (ie. GL_TEXTURE_RECTANGLE or GL_TEXTURE_2D)
  //  - anything else?
  m_outTexInfo = outlet_new(this->x_obj, 0);

  m_FBOcolor[0] = 0.f;
  m_FBOcolor[1] = 0.f;
  m_FBOcolor[2] = 0.f;
  m_FBOcolor[3] = 0.f;
	
	m_perspect[0] = -1.f;
 	m_perspect[1] = 1.f;	
	m_perspect[2] = -1.f;
	m_perspect[3] = 1.f;	
	m_perspect[4] = 1.f;
	m_perspect[5] = 20.f;	
}

gemframebuffer :: gemframebuffer(t_symbol *format, t_symbol *type)
  : m_haveinit(false), m_wantinit(false), m_frameBufferIndex(0), m_depthBufferIndex(0),
    m_offScreenID(0), m_texTarget(GL_TEXTURE_2D), m_texunit(0),
    m_width(256), m_height(256),
    m_mode(0), m_internalformat(GL_RGB8), m_format(GL_RGB), m_type(GL_UNSIGNED_BYTE)
{
  // create an outlet to send out texture info:
  //  - ID
  //  - width & height
  //  - format/type (ie. GL_TEXTURE_RECTANGLE or GL_TEXTURE_2D)
  //  - anything else?
  m_outTexInfo = outlet_new(this->x_obj, 0);
  
  m_FBOcolor[0] = 0.f;
  m_FBOcolor[1] = 0.f;
  m_FBOcolor[2] = 0.f;
  m_FBOcolor[3] = 0.f;

	m_perspect[0] = -1.f;
 	m_perspect[1] = 1.f;	
	m_perspect[2] = -1.f;
	m_perspect[3] = 1.f;	
	m_perspect[4] = 1.f;
	m_perspect[5] = 20.f;	
	
	formatMess(format->s_name);
  typeMess(type->s_name);
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemframebuffer :: ~gemframebuffer()
{
  destroyFBO();
  outlet_free(m_outTexInfo);
}

////////////////////////////////////////////////////////
// extension check
//
/////////////////////////////////////////////////////////
bool gemframebuffer :: isRunnable() {
  if(!GLEW_VERSION_1_3) {
    error("openGL version 1.3 needed");
    return false;
  }

  if(GLEW_EXT_framebuffer_object)
    return true;

  error("openGL framebuffer extension is not supported by this system");

  return false;
}


////////////////////////////////////////////////////////
// renderGL
//
/////////////////////////////////////////////////////////
void gemframebuffer :: render(GemState *state)
{
  if(!m_width || !m_height) {
    error("width and height must be present!");
  }
 
  glActiveTexture(GL_TEXTURE0_ARB + m_texunit);

  if (m_wantinit)
    initFBO();
  
  // store the window viewport dimensions so we can reset them,
  // and set the viewport to the dimensions of our texture
  glGetIntegerv(GL_VIEWPORT, m_vp);
  glGetFloatv( GL_COLOR_CLEAR_VALUE, m_color );
	
  glBindTexture( m_texTarget, 0 );
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameBufferIndex);
  // Bind the texture to the frame buffer.
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                            m_texTarget, m_offScreenID, 0);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                               GL_RENDERBUFFER_EXT, m_depthBufferIndex);
  
  // debug yellow color
  // glClearColor( 1,1,0,0);
  glClearColor( m_FBOcolor[0], m_FBOcolor[1], m_FBOcolor[2], m_FBOcolor[3] );
  
  // Clear the buffers and reset the model view matrix.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  // We need a one-to-one mapping of pixels to texels in order to
  // ensure every element of our texture is processed. By setting our
  // viewport to the dimensions of our destination texture and drawing
  // a screen-sized quad (see below), we ensure that every pixel of our
  // texel is generated and processed in the fragment program.		
  glViewport(0,0, m_width, m_height);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
	glFrustum( m_perspect[0],  m_perspect[1],  m_perspect[2],  m_perspect[3], m_perspect[4], m_perspect[5]);
	glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

////////////////////////////////////////////////////////
// postRender
//
/////////////////////////////////////////////////////////
void gemframebuffer :: postrender(GemState *state)
{
  t_float w, h;

  glActiveTexture(GL_TEXTURE0_ARB + m_texunit);

  if(m_texTarget== GL_TEXTURE_2D) {
    w=(t_float)1.f;
    h=(t_float)1.f;
  } else {
    w=(t_float)m_width;
    h=(t_float)m_height;
  }

  // GPGPU CONCEPT 4: Viewport-Sized Quad = Data Stream Generator.
  // In order to execute fragment programs, we need to generate pixels.
  // Drawing a quad the size of our viewport (see above) generates a
  // fragment for every pixel of our destination texture. Each fragment
  // is processed identically by the fragment program. Notice that in
  // the reshape() function, below, we have set the frustum to
  // orthographic, and the frustum dimensions to [-1,1].  Thus, our
  // viewport-sized quad vertices are at [-1,-1], [1,-1], [1,1], and
  // [-1,1]: the corners of the viewport.

  glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
  glBindTexture( m_texTarget, m_offScreenID );

  glMatrixMode(GL_PROJECTION);
	glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  // reset to visible window's clear color
  glClearColor( m_color[0], m_color[1], m_color[2], m_color[3] );
  // reset to original viewport dimensions
  glViewport( m_vp[0], m_vp[1], m_vp[2], m_vp[3] );
  // now that the render is done,

  // send textureID, w, h, textureTarget to outlet
  t_atom ap[5];
  SETFLOAT(ap+0, (t_float)m_offScreenID);
  SETFLOAT(ap+1, w);
  SETFLOAT(ap+2, h);
  SETFLOAT(ap+3, m_texTarget);
  SETFLOAT(ap+4, (t_float)0);
  outlet_list(m_outTexInfo, 0, 5, ap);
}

void gemframebuffer :: printInfo()
{
  if(m_mode)
    post("using rectmode 1:GL_TEXTURE_RECTANGLE");
  else post("using rectmode 0:GL_TEXTURE_2D");

  switch(m_type) {
  case GL_UNSIGNED_BYTE: post("using type: BYTE"); break;
  case GL_FLOAT: post("using type: FLOAT"); break;
  default: post("using type: unknown(%d)", m_type);
  }

  switch(m_format) {
  case GL_YUV422_GEM: post("using color: YUV"); break;
  case GL_RGB: post("using color: RGB"); break;
  case GL_RGBA: post("using color: RGBA"); break;
  case GL_BGRA: post("using color: BGRA"); break;
  }

  post("using texunit: %d", m_texunit);
}

/////////////////////////////////////////////////////////
// initFBO
//
/////////////////////////////////////////////////////////
void gemframebuffer :: initFBO()
{
  // clean up any existing FBO before creating a new one
  if(m_haveinit)
    destroyFBO();
	
  if ( !m_mode )
    m_texTarget = GL_TEXTURE_2D;
  else
    m_texTarget = GL_TEXTURE_RECTANGLE_EXT;

  // Generate frame buffer object then bind it.
  glGenFramebuffersEXT(1, &m_frameBufferIndex);
  glGenRenderbuffersEXT(1, &m_depthBufferIndex);

  // Create the texture we will be using to render to.
  glGenTextures(1, &m_offScreenID);
  glBindTexture(m_texTarget, m_offScreenID);

  glTexImage2D( m_texTarget, 0, m_internalformat, m_width, m_height, 0, m_format, m_type, NULL );
  // 2.13.2006
  // GL_LINEAR causes fallback to software shader
  // so switching back to GL_NEAREST
  glTexParameteri(m_texTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(m_texTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(m_texTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(m_texTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
  
  // Initialize the render buffer.
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthBufferIndex);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, m_width, m_height);

  // Make sure we have not errors.
  GLenum status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT) ;
  if( status != GL_FRAMEBUFFER_COMPLETE_EXT )
    {
      switch(status) {                                          
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
        error("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT");
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
        error("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT");
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        error("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT");
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        error("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT");
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        error("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT");
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
        error("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT");
        break;
      case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
        error("GL_FRAMEBUFFER_UNSUPPORTED_EXT");
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
        error("GL_INVALID_FRAMEBUFFER_OPERATION_EXT");
        break;
      default:
        error("Unknown ERROR %d", status);
      }
      return;
    }

  // Return out of the frame buffer.
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

  m_haveinit = true;
  m_wantinit = false;

  printInfo();
}

////////////////////////////////////////////////////////
// destroyFBO
//
/////////////////////////////////////////////////////////
void gemframebuffer :: destroyFBO()
{
  if(!GLEW_EXT_framebuffer_object)
    return;
  // Release all resources.
  if(m_depthBufferIndex) glDeleteRenderbuffersEXT(1, &m_depthBufferIndex);
  if(m_frameBufferIndex) glDeleteFramebuffersEXT(1, &m_frameBufferIndex);
  if(m_offScreenID) glDeleteTextures(1, &m_offScreenID);

  m_haveinit = false;
}

////////////////////////////////////////////////////////
// bangMess
//
/////////////////////////////////////////////////////////
void gemframebuffer :: bangMess()
{

}

////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void gemframebuffer :: startRendering()
{
  m_wantinit = true;
}

////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void gemframebuffer :: stopRendering()
{
  destroyFBO();
}
////////////////////////////////////////////////////////
// dimMess
//
/////////////////////////////////////////////////////////
void gemframebuffer :: dimMess(int width, int height)
{
  if (width != m_width || height != m_height)
    {
      m_width = width;
      m_height = height;
      m_wantinit=true;
      setModified();
    }
}

void gemframebuffer :: colorMess(float red, float green, float blue, float alpha)
{
  
  m_FBOcolor[0] = red;
  m_FBOcolor[1] = green;
  m_FBOcolor[2] = blue;
  m_FBOcolor[3] = alpha;
    
  setModified();

  
}

void gemframebuffer :: perspectiveMess(float f_left, float f_right, 
                                       float f_bottom, float f_top,
                                       float f_near, float f_far)
{
	
	m_perspect[0] = f_left;
	m_perspect[1] = f_right;
	m_perspect[2] = f_bottom;
	m_perspect[3] = f_top;
	m_perspect[4] = f_near;
	m_perspect[5] = f_far;
    
	setModified();

}

void gemframebuffer :: formatMess(char* format)
{
  if (!strcmp(format, "YUV"))
    {
      m_format = GL_YUV422_GEM;
#ifdef __APPLE__
      m_type = GL_UNSIGNED_SHORT_8_8_REV_APPLE;
#else
      m_type = GL_UNSIGNED_BYTE;
#endif
      post("format is GL_YUV, %d",m_format);
      return;
    } else
    
    if (!strcmp(format, "RGB")){
      m_internalformat = GL_RGB;
      m_format = GL_RGB;
      post("format is GL_RGB, %d",m_format);

      return;
    } else
    
      if (!strcmp(format, "RGBA")){
        // colorspace will equal RGBA
        post("format is GL_RGBA, %d",m_format);
        m_internalformat = GL_RGBA;
#ifdef __APPLE__
        m_format = GL_BGRA;
#else 
        m_format = GL_RGBA;
#endif
        return;
      } else
    
      if (!strcmp(format, "RGB32")){
        // colorspace will equal RGB32
        post("format is GL_RGB_FLOAT32_ATI, %d",m_format);
        m_internalformat = GL_RGB_FLOAT32_ATI;
#ifdef __APPLE__
        m_format = GL_BGR;
#else 
        m_format = GL_RGB;
#endif
        return;
      } else {
        //default
        post("default format is GL_RGB, %d",m_format);
        m_internalformat = GL_RGB;
        m_format = GL_RGB;
      }
  // changed format, so we need to rebuild the FBO
  m_wantinit=true;
}

void gemframebuffer :: typeMess(char* type)
{
  if (!strcmp(type, "BYTE")){
    m_type = GL_UNSIGNED_BYTE;
    post("type is BYTE, %d",m_type);
    return;
  } 
	else if (!strcmp(type, "INT")){
	  m_type = GL_UNSIGNED_BYTE;
    post("type is INT, %d",m_type);
    return;
  }
	else if (!strcmp(type, "FLOAT")){
	  post("type is GL_FLOAT, %d",m_type);
	  m_type = GL_FLOAT;
	  return;
  } else {
    //default
	  m_type = GL_UNSIGNED_BYTE;
	  post("default type is BYTE, %d",m_type);
  }
  // changed type, so we need to rebuild the FBO
  m_wantinit=true;
}

////////////////////////////////////////////////////////
// static member function
//
////////////////////////////////////////////////////////
void gemframebuffer :: obj_setupCallback(t_class *classPtr)
{
  class_addbang(classPtr, (t_method)&gemframebuffer::bangMessCallback);
  class_addmethod(classPtr, (t_method)&gemframebuffer::modeCallback,
                  gensym("mode"), A_FLOAT, A_NULL);
 class_addmethod(classPtr, (t_method)&gemframebuffer::modeCallback,
                  gensym("rectangle"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemframebuffer::dimMessCallback,
                  gensym("dimen"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemframebuffer::dimMessCallback,
                  gensym("dim"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemframebuffer::formatMessCallback,
                  gensym("format"), A_DEFSYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&gemframebuffer::typeMessCallback,
                  gensym("type"), A_DEFSYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&gemframebuffer::colorMessCallback,
                  gensym("color"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemframebuffer::texunitCallback,
                  gensym("texunit"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemframebuffer::perspectiveMessCallback,
  				 gensym("perspec"), A_GIMME, A_NULL);
}
void gemframebuffer :: bangMessCallback(void *data)
{
  GetMyClass(data)->bangMess();
}
void gemframebuffer :: modeCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->m_mode=((int)quality);
  // changed mode, so we need to rebuild the FBO
  GetMyClass(data)->m_wantinit=true;
}
void gemframebuffer :: dimMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->dimMess((int)width, (int)height);
}
void gemframebuffer :: formatMessCallback (void *data, t_symbol *format)
{
  GetMyClass(data)->formatMess((char*)format->s_name);
}
void gemframebuffer :: typeMessCallback (void *data, t_symbol *type)
{
  GetMyClass(data)->typeMess((char*)type->s_name);
}

void gemframebuffer :: colorMessCallback(void *data, t_floatarg red, t_floatarg green, t_floatarg blue, t_floatarg alpha)
{
  GetMyClass(data)->colorMess(red, green, blue, alpha);
}

void gemframebuffer :: texunitCallback(void *data, t_floatarg unit)
{
  GetMyClass(data)->m_texunit=(GLuint)unit;
}

void gemframebuffer :: perspectiveMessCallback(void *data, t_symbol*s,int argc, t_atom*argv)
{
	t_float f_left, f_right, f_bottom, f_top, f_near, f_far;
	switch(argc){
		case 6:
			f_left=  atom_getfloat(argv);
			f_right=atom_getfloat(argv+1);
			f_bottom= atom_getfloat(argv+2);
			f_top=  atom_getfloat(argv+3);
			f_near=atom_getfloat(argv+4);
			f_far= atom_getfloat(argv+5);
			GetMyClass(data)->perspectiveMess((float)f_left, (float)f_right, 
                         (float)f_bottom, (float)f_top, (float)f_near, (float)f_far);
			break;
		default:
			GetMyClass(data)->error("\"perspec\" expects 6 values for frustum - left, right, bottom, top, near, far");
	}
}