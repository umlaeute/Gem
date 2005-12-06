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
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "gemframebuffer.h"

#ifdef __APPLE__
extern bool HaveValidContext (void);
#endif

CPPEXTERN_NEW(gemframebuffer)

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
		 m_offScreenID(0), m_texTarget(1), m_width(256), m_height(256),
		 m_mode(1)
{
  // create an outlet to send out texture info:
  //  - ID
  //  - width & height
  //  - format/type (ie. GL_TEXTURE_RECTANGLE or GL_TEXTURE_2D)
  //  - anything else?
  m_outTexInfo = outlet_new(this->x_obj, 0);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemframebuffer :: ~gemframebuffer()
{
    outlet_free(m_outTexInfo);
}

/////////////////////////////////////////////////////////
// renderGL
//
/////////////////////////////////////////////////////////
void gemframebuffer :: render(GemState *state)
{
#ifdef __APPLE__
  if (!HaveValidContext ()) {
	post("[%s] needs window/context to load program", m_objectname->s_name);
	return;
  }
#endif
  if ( !m_mode )
	m_texTarget = GL_TEXTURE_2D;
  else
	m_texTarget = GL_TEXTURE_RECTANGLE_EXT;
	
  if (!m_init && m_width && m_height)
    initFBO();
#ifdef GL_EXT_framebuffer_object
  glBindTexture( m_texTarget, 0 );
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameBufferIndex);
  // Clear the buffers and reset the model view matrix.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  glLoadIdentity();
#endif

  t_atom ap[4];
  SETFLOAT(ap, (t_float)m_offScreenID);
  SETFLOAT(ap+1, m_width);
  SETFLOAT(ap+2, m_height);
  SETFLOAT(ap+3, m_texTarget);
  //outlet_float(m_outTexID, (t_float)m_offScreenID);
  outlet_list(m_outTexInfo, 0, 4, ap);
}

/////////////////////////////////////////////////////////
// postRender
//
/////////////////////////////////////////////////////////
void gemframebuffer :: postrender(GemState *state)
{
#ifdef GL_EXT_framebuffer_object
  glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
#endif
}

/////////////////////////////////////////////////////////
// initFBO
//
/////////////////////////////////////////////////////////
void gemframebuffer :: initFBO()
{
#ifdef __APPLE__
  if (!HaveValidContext ()) {
	post("[%s] needs window/context to load program", m_objectname->s_name);
	return;
  }
#endif
#ifdef GL_EXT_framebuffer_object
  // Generate frame buffer object then bind it.
  glGenFramebuffersEXT(1, &m_frameBufferIndex);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameBufferIndex);
  glGenRenderbuffersEXT(1, &m_depthBufferIndex);
  CheckErrorsGL("genRenderbuffers generate");

  // Create the texture we will be using to render to.
  glGenTextures(1, &m_offScreenID);
  glBindTexture(m_texTarget, m_offScreenID);
  CheckErrorsGL("glBindtexture create");
  glTexParameteri(m_texTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(m_texTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(m_texTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(m_texTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
  CheckErrorsGL("glTexParameter create");
  glTexImage2D(m_texTarget, 0, GL_RGB, m_width, m_height, 0,
                GL_RGB, GL_FLOAT, NULL);
				//GL_RGB, GL_BYTE, NULL);
				//GL_RGBA, GL_RGBA_FLOAT32_APPLE, NULL);
				//GL_RGB, GL_RGB_FLOAT32_APPLE, NULL); // What's correct for here?
  CheckErrorsGL("glTexImage2D create");

  // Bind the texture to the frame buffer.
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                             m_texTarget, m_offScreenID, 0);
  CheckErrorsGL("FramebufferTexture bind");

  // Initialize the render buffer.
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthBufferIndex);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, m_width, m_height);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                                GL_RENDERBUFFER_EXT, m_depthBufferIndex);
  CheckErrorsGL("RenderbufferTexture initialize");
  // Make sure we have not errors.
  if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
  {
    post("[%s]:  FRAMEBUFFER status incomplete", m_objectname->s_name);
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
#ifdef __APPLE__
  if (!HaveValidContext ()) {
	post("[%s] needs window/context to load program", m_objectname->s_name);
	return;
  }
#endif
#ifdef GL_EXT_framebuffer_object
  // Release all resources.
  if(m_depthBufferIndex) glDeleteRenderbuffersEXT(1, &m_depthBufferIndex);
  if(m_frameBufferIndex) glDeleteFramebuffersEXT(1, &m_frameBufferIndex);
  if(m_offScreenID) glDeleteTextures(1, &m_offScreenID);
#endif
  m_init = 0;
}

/////////////////////////////////////////////////////////
// CheckErrorsGL
//
/////////////////////////////////////////////////////////
void gemframebuffer :: CheckErrorsGL(const char* location, ostream& ostr) 
{
	GLuint errnum;
	const char *errstr;
	while (errnum = glGetError()) 
	{
		errstr = reinterpret_cast<const char *>(gluErrorString(errnum));
		ostr << errstr; 

		if(location) ostr << " at " << location;		
		ostr << endl;
	}
	return;
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
//    setModified();
	destroyFBO();
	initFBO();
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
}
void gemframebuffer :: bangMessCallback(void *data)
{
    GetMyClass(data)->bangMess();
}
void gemframebuffer :: modeCallback(void *data, t_floatarg quality)
{
  GetMyClass(data)->m_mode=((int)quality);
}
void gemframebuffer :: dimMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->dimMess((int)width, (int)height);
}