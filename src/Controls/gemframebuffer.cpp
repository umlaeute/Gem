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
    	 : m_init(0), m_frameBufferIndex(0), m_depthBufferIndex(0),
		 m_offScreenID(0), m_texTarget(GL_TEXTURE_2D), m_width(256), m_height(256),
		 m_mode(0), m_internalformat(GL_RGB8), m_format(GL_RGB), m_type(GL_UNSIGNED_BYTE)
{
  // create an outlet to send out texture info:
  //  - ID
  //  - width & height
  //  - format/type (ie. GL_TEXTURE_RECTANGLE or GL_TEXTURE_2D)
  //  - anything else?
  m_outTexInfo = outlet_new(this->x_obj, 0);
}
gemframebuffer :: gemframebuffer(t_symbol *format, t_symbol *type)
    	 : m_init(0), m_frameBufferIndex(0), m_depthBufferIndex(0),
		 m_offScreenID(0), m_texTarget(GL_TEXTURE_2D), m_width(256), m_height(256),
		 m_mode(0), m_internalformat(GL_RGB8), m_format(GL_RGB), m_type(GL_UNSIGNED_BYTE)
{
  // create an outlet to send out texture info:
  //  - ID
  //  - width & height
  //  - format/type (ie. GL_TEXTURE_RECTANGLE or GL_TEXTURE_2D)
  //  - anything else?
  m_outTexInfo = outlet_new(this->x_obj, 0);
  
  formatMess(format->s_name);
  typeMess(type->s_name);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemframebuffer :: ~gemframebuffer()
{
	destroyFBO();
    outlet_free(m_outTexInfo);
}

/////////////////////////////////////////////////////////
// renderGL
//
/////////////////////////////////////////////////////////
void gemframebuffer :: render(GemState *state)
{	
  if (!m_init && m_width && m_height)
    initFBO();
	
  // store the window viewport dimensions so we can reset them,
  // and set the viewport to the dimensions of our texture
  glGetIntegerv(GL_VIEWPORT, m_vp);
  glGetFloatv( GL_COLOR_CLEAR_VALUE, m_color );
  
#ifdef GL_EXT_framebuffer_object
  glBindTexture( m_texTarget, 0 );
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameBufferIndex);
    // Bind the texture to the frame buffer.
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                             m_texTarget, m_offScreenID, 0);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                                GL_RENDERBUFFER_EXT, m_depthBufferIndex);
  
//  glPushMatrix();

  // We need a one-to-one mapping of pixels to texels in order to
  // ensure every element of our texture is processed. By setting our
  // viewport to the dimensions of our destination texture and drawing
  // a screen-sized quad (see below), we ensure that every pixel of our
  // texel is generated and processed in the fragment program.		
  glViewport(0,0, m_width, m_height);

  // debug yellow color
  glClearColor( 1,1,0,0);
/*
  glShadeModel( GL_SMOOTH );
  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );
*/
//  glEnable(GL_DEPTH_TEST);
//  glDepthFunc( GL_LESS );
  
  // Clear the buffers and reset the model view matrix.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  glPushMatrix();
//  glEnable(GL_DEPTH_TEST);
//  glMatrixMode(GL_PROJECTION);
//  glLoadIdentity();
//  gluOrtho2D(-1, 1, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
#endif
}

/////////////////////////////////////////////////////////
// postRender
//
/////////////////////////////////////////////////////////
void gemframebuffer :: postrender(GemState *state)
{
  // GPGPU CONCEPT 4: Viewport-Sized Quad = Data Stream Generator.
  // In order to execute fragment programs, we need to generate pixels.
  // Drawing a quad the size of our viewport (see above) generates a
  // fragment for every pixel of our destination texture. Each fragment
  // is processed identically by the fragment program. Notice that in
  // the reshape() function, below, we have set the frustum to
  // orthographic, and the frustum dimensions to [-1,1].  Thus, our
  // viewport-sized quad vertices are at [-1,-1], [1,-1], [1,1], and
  // [-1,1]: the corners of the viewport.
/*
  glBegin(GL_QUADS);
  {
	glTexCoord2f(0, 0); glVertex3f(-1, -1, -0.5f);
	glTexCoord2f(1, 0); glVertex3f( 1, -1, -0.5f);
	glTexCoord2f(1, 1); glVertex3f( 1,  1, -0.5f);
	glTexCoord2f(0, 1); glVertex3f(-1,  1, -0.5f);
  }
  glEnd();
*/		
#ifdef GL_EXT_framebuffer_object
  glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
  glBindTexture( m_texTarget, m_offScreenID );
//  glDisable(GL_DEPTH_TEST);
#endif
//  glPopMatrix();
  // reset to visible window's clear color
  glClearColor( m_color[0], m_color[1], m_color[2], m_color[3] );
  // reset to original viewport dimensions
  glViewport( m_vp[0], m_vp[1], m_vp[2], m_vp[3] );
  // now that the render is done,
  // send textureID, w, h, textureTarget to outlet
  t_atom ap[4];
  SETFLOAT(ap, (t_float)m_offScreenID);
  SETFLOAT(ap+1, m_width);
  SETFLOAT(ap+2, m_height);
  SETFLOAT(ap+3, m_texTarget);
  outlet_list(m_outTexInfo, 0, 4, ap);
}

/////////////////////////////////////////////////////////
// initFBO
//
/////////////////////////////////////////////////////////
void gemframebuffer :: initFBO()
{
  // clean up any existing FBO before creating a new one
  if(m_init)
	destroyFBO();
	
  if ( !m_mode )
  {
	m_texTarget = GL_TEXTURE_2D;
	post("[%s]:  using mode 0:GL_TEXTURE_2D", m_objectname->s_name);
  }else{
	m_texTarget = GL_TEXTURE_RECTANGLE_EXT;
	post("[%s]:  using mode 1:GL_TEXTURE_RECTANGLE_EXT", m_objectname->s_name);
  }
	
#ifdef GL_EXT_framebuffer_object
  // Generate frame buffer object then bind it.
  glGenFramebuffersEXT(1, &m_frameBufferIndex);
//  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameBufferIndex);
  glGenRenderbuffersEXT(1, &m_depthBufferIndex);

  // Create the texture we will be using to render to.
  glGenTextures(1, &m_offScreenID);
  glBindTexture(m_texTarget, m_offScreenID);

  glTexImage2D( m_texTarget, 0, m_internalformat, m_width, m_height, 0, m_format, m_type, NULL );
//  glTexImage2D(m_texTarget, 0, GL_RGB_FLOAT32_APPLE, m_width, m_height, 0,
//                GL_RGB, GL_FLOAT, NULL);
				//GL_RGB, GL_BYTE, NULL);
				//GL_RGBA, GL_RGBA_FLOAT32_APPLE, NULL);
				//GL_RGB, GL_RGB_FLOAT32_APPLE, NULL); // What's correct for here?
  // 2.13.2006
  // GL_LINEAR causes fallback to software shader
  // so switching back to GL_NEAREST
  glTexParameteri(m_texTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(m_texTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(m_texTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(m_texTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
  
  // Bind the texture to the frame buffer.
//  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
//                             m_texTarget, m_offScreenID, 0);

  // Initialize the render buffer.
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthBufferIndex);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, m_width, m_height);
//  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
//                                GL_RENDERBUFFER_EXT, m_depthBufferIndex);
  // Make sure we have not errors.
  GLenum status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT) ;
  if( status != GL_FRAMEBUFFER_COMPLETE_EXT )
  {
	  switch(status) {                                          
  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		 post("[%s]:  GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT", m_objectname->s_name);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		post("[%s]:  GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT", m_objectname->s_name);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		post("[%s]:  GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT", m_objectname->s_name);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		 post("[%s]:  GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT", m_objectname->s_name);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		post("[%s]:  GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT", m_objectname->s_name);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		 post("[%s]:  GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT", m_objectname->s_name);
    break;
  case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		 post("[%s]:  GL_FRAMEBUFFER_UNSUPPORTED_EXT", m_objectname->s_name);
    break;
  case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
		 post("[%s]:  GL_INVALID_FRAMEBUFFER_OPERATION_EXT", m_objectname->s_name);
    break;
  default:
		 post("[%s]:  Unknown ERROR", m_objectname->s_name);
  }
	return;
  }

  // Return out of the frame buffer.
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#endif
  m_init = 1;
}

/////////////////////////////////////////////////////////
// destroyFBO
//
/////////////////////////////////////////////////////////
void gemframebuffer :: destroyFBO()
{
#ifdef GL_EXT_framebuffer_object
  // Release all resources.
  if(m_depthBufferIndex) glDeleteRenderbuffersEXT(1, &m_depthBufferIndex);
  if(m_frameBufferIndex) glDeleteFramebuffersEXT(1, &m_frameBufferIndex);
  if(m_offScreenID) glDeleteTextures(1, &m_offScreenID);
#endif
  m_init = 0;
}

/////////////////////////////////////////////////////////
// bangMess
//
/////////////////////////////////////////////////////////
void gemframebuffer :: bangMess()
{

}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void gemframebuffer :: startRendering()
{

}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void gemframebuffer :: stopRendering()
{

}
/////////////////////////////////////////////////////////
// dimMess
//
/////////////////////////////////////////////////////////
void gemframebuffer :: dimMess(int width, int height)
{
  if (width != m_width || height != m_height)
  {
    m_width = width;
    m_height = height;

	destroyFBO();
	initFBO();
	setModified();
  }
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
	  post("[gemframebuffer]: format is GL_RGB, %d",m_format);
      return;
    } else
    
    if (!strcmp(format, "RGB")){
	  m_internalformat = GL_RGB;
      m_format = GL_RGB;
      post("[gemframebuffer]: format is GL_RGB, %d",m_format);

      return;
    } else
    
    if (!strcmp(format, "RGBA")){
	// colorspace will equal RGBA
      post("[gemframebuffer]: format is GL_RGBA, %d",m_format);
	  m_internalformat = GL_RGBA;
#ifdef __APPLE__
      m_format = GL_BGRA;
#else 
      m_format = GL_RGBA;
#endif
      return;
	} else {
	//default
	  post("[gemframebuffer]: default format is GL_RGB, %d",m_format);
	  m_internalformat = GL_RGB;
      m_format = GL_RGB;
    }
}

void gemframebuffer :: typeMess(char* type)
{
    if (!strcmp(type, "BYTE")){
      m_type = GL_UNSIGNED_BYTE;
      post("[gemframebuffer]: type is BYTE, %d",m_type);
      return;
    } 
	else if (!strcmp(type, "INT")){
	  m_type = GL_UNSIGNED_BYTE;
      post("[gemframebuffer]: type is INT, %d",m_type);
      return;
    }
	else if (!strcmp(type, "FLOAT")){
	  post("[gemframebuffer]: type is GL_FLOAT, %d",m_type);
	  m_type = GL_FLOAT;
	  return;
    } else {
	//default
	  m_type = GL_UNSIGNED_BYTE;
	  post("[gemframebuffer]: default type is BYTE, %d",m_type);
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemframebuffer :: obj_setupCallback(t_class *classPtr)
{
  class_addbang(classPtr, (t_method)&gemframebuffer::bangMessCallback);
  class_addmethod(classPtr, (t_method)&gemframebuffer::modeCallback,
	gensym("mode"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemframebuffer::dimMessCallback,
	gensym("dim"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemframebuffer::formatMessCallback,
	gensym("format"), A_DEFSYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&gemframebuffer::typeMessCallback,
	gensym("type"), A_DEFSYMBOL, A_NULL);
}
void gemframebuffer :: bangMessCallback(void *data)
{
  GetMyClass(data)->bangMess();
}
void gemframebuffer :: modeCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->m_mode=((int)quality);
  // changed mode, so we need to rebuild the FBO
  GetMyClass(data)->destroyFBO();
  GetMyClass(data)->initFBO();
}
void gemframebuffer :: dimMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->dimMess((int)width, (int)height);
}
void gemframebuffer :: formatMessCallback (void *data, t_symbol *format)
{
  GetMyClass(data)->formatMess((char*)format->s_name);
  GetMyClass(data)->destroyFBO();
  GetMyClass(data)->initFBO();
}
void gemframebuffer :: typeMessCallback (void *data, t_symbol *type)
{
  GetMyClass(data)->typeMess((char*)type->s_name);
  GetMyClass(data)->destroyFBO();
  GetMyClass(data)->initFBO();
}