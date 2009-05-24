////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute, iem @ kug
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemContext.h"
#include "GemMan.h"

#ifdef GLEW_MX
GLEWContext*s_glewcontext=NULL;
GemGlewXContext*s_glewxcontext=NULL;
#endif

/////////////////////////////////////////////////////////
//
// GemContext
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemContext :: GemContext()
  : m_width(0), m_height(0),
    m_infoOut(NULL)
#ifdef GLEW_MX
  , m_context(NULL), m_xcontext(NULL)
#endif /* GLEW_MX */
{
  m_infoOut = outlet_new(this->x_obj, 0);
}
/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemContext :: ~GemContext()
{
  outlet_free(m_infoOut); m_infoOut=NULL;
  destroy();
}

void GemContext::info(t_symbol*s, int argc, t_atom*argv) {
  if(m_infoOut) {
    outlet_anything(m_infoOut, s, argc, argv); 
  }
}
void GemContext :: info(t_symbol*s, t_float value)
{
  t_atom atom;
  SETFLOAT(&atom, value);
  info(s, 1, &atom); 
}

void GemContext :: bang(void)
{
  outlet_bang(m_infoOut);
}




/* mouse movement */
void GemContext::motion(int x, int y)
{
  t_atom ap[3];
  SETSYMBOL(ap+0, gensym("motion"));
  SETFLOAT (ap+1, x);
  SETFLOAT (ap+2, y);

  info(gensym("mouse"), 3, ap);
}
/* mouse buttons */
void GemContext::button(int id, int state)
{
  t_atom ap[3];
  SETSYMBOL(ap+0, gensym("button"));
  SETFLOAT (ap+1, id);
  SETFLOAT (ap+2, state);

  info(gensym("mouse"), 3, ap);
}

/* keyboard buttons */
void GemContext::key(t_symbol*id, int state) {
  t_atom ap[3];
  SETSYMBOL(ap+0, gensym("key"));
  SETSYMBOL(ap+1, id);
  SETFLOAT (ap+2, state);

  info(gensym("keyboard"), 3, ap);
}

bool GemContext::create(void){
  bool ret=true;
  static int firsttime=1;
#ifdef GLEW_MX
  GLEWContext*oldcontext=s_glewcontext;
  GemGlewXContext*oldcontextx=s_glewxcontext;
  m_context = new GLEWContext;
  m_xcontext = new GemGlewXContext;
  s_glewcontext=m_context;
  s_glewxcontext=m_xcontext;
  
  firsttime=1;
#endif

  if(firsttime) {
    GLenum err = glewInit();
  
    if (GLEW_OK != err) {
      if(GLEW_ERROR_GLX_VERSION_11_ONLY == err) {
	error("GEM: failed to init GLEW (glx): continuing anyhow - please report any problems to the gem-dev mailinglist!");
      } else if (GLEW_ERROR_GL_VERSION_10_ONLY) {
	error("GEM: failed to init GLEW: your system only supports openGL-1.0");
	ret=false;
      } else {
	error("GEM: failed to init GLEW");
	ret=false;
      }
    }
    post("GLEW version %s",glewGetString(GLEW_VERSION));

    

  }

  /* check the stack-sizes */
  glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH,    m_maxStackDepth+0);
  glGetIntegerv(GL_MAX_COLOR_MATRIX_STACK_DEPTH, m_maxStackDepth+1);
  glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH,      m_maxStackDepth+2);
  glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH,   m_maxStackDepth+3);

  firsttime=0;
  return true;
}


void GemContext::destroy(void){
#ifdef GLEW_MX
  if(m_context) {
    if(m_context==s_glewcontext) {
      s_glewcontext=NULL;
    }
    delete m_context;
  }
  m_context=NULL;
#endif
}

bool GemContext::makeCurrent(void){
  GemMan::maxStackDepth[0]=m_maxStackDepth[0];
  GemMan::maxStackDepth[1]=m_maxStackDepth[1];
  GemMan::maxStackDepth[2]=m_maxStackDepth[2];
  GemMan::maxStackDepth[3]=m_maxStackDepth[3];

#ifdef GLEW_MX
  if(!m_context) {
    return false;
    /* alternatively we could create a context on the fly... */
  }
  s_glewcontext=m_context;
#endif
  return true;

}


void GemContext::dimensionsMess(void){
   t_atom ap[2];
  SETFLOAT (ap+0, m_width);
  SETFLOAT (ap+1, m_height);

  info(gensym("dimen"), 2, ap); 
}



#ifdef GLEW_MX
GLEWContext*GemContext::getGlewContext(void) {
  if(NULL==s_glewcontext) {
    /* we should find another glew-context asap and make that one current! */
    return NULL;
  } else {
    return s_glewcontext;
  }

  return NULL;
}

GemGlewXContext*GemContext::getGlewXContext(void) {
  if(NULL==s_glewxcontext) {
    /* we should find another glew-context asap and make that one current! */
    return NULL;
  } else {
    return s_glewxcontext;
  }

  return NULL;
}

GLEWContext*glewGetContext(void){return  GemContext::getGlewContext();}
GemGlewXContext*wglewGetContext(void){return  GemContext::getGlewXContext();}
GemGlewXContext*glxewGetContext(void){return  GemContext::getGlewXContext();}
#endif
