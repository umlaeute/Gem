////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2009-2011 IOhannes m zmoelnig. forum::für::umläute, iem @ kug
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "GemContext.h"
#include "GemMan.h"
#include "GemException.h"

#include <stack>

#ifdef GEM_MULTICONTEXT
# warning multicontext rendering currently under development
#endif /* GEM_MULTICONTEXT */

static GLEWContext*s_glewcontext=NULL;
static GemGlewXContext*s_glewxcontext=NULL;

using namespace gem;

class Context::PIMPL {
public:
  PIMPL(void) : 
#ifdef GEM_MULTICONTEXT
    context(new GLEWContext), xcontext(new GemGlewXContext), 
#else
    context(NULL), xcontext(NULL), 
#endif
    contextid(makeID())
  {
    /* check the stack-sizes */
    glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH,    maxStackDepth+GemMan::STACKMODELVIEW);
    glGetIntegerv(GL_MAX_COLOR_MATRIX_STACK_DEPTH, maxStackDepth+GemMan::STACKCOLOR);
    glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH,      maxStackDepth+GemMan::STACKTEXTURE);
    glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH,   maxStackDepth+GemMan::STACKPROJECTION);
  }
  ~PIMPL(void) {
    freeID(contextid);
#ifdef GEM_MULTICONTEXT
    if(context )delete context; context=NULL;
    if(xcontext)delete xcontext; xcontext=0;
#endif
  }

  GLint maxStackDepth[4];

  GLEWContext    *context;
  GemGlewXContext*xcontext;

  unsigned int contextid;

  static std::stack<unsigned int>s_contextid;

  static unsigned int makeID(void) //  GemContext_newid
  {
    unsigned int id=0;
#ifdef GEM_MULTICONTEXT
    /* LATER reuse freed ids */
    static unsigned int nextid=0;
    id=nextid;
    nextid++;
#endif /* GEM_MULTICONTEXT */
    return id;
  }

  static void freeID(unsigned int id)
  {
    /* LATER reuse freed ids */
    /* LATER remove this ID from the s_contextid stack and related (x)context */
  }

  static std::stack<GLEWContext*>s_context;
  static std::stack<GemGlewXContext*>s_xcontext;
};
std::stack<unsigned int>    Context::PIMPL::s_contextid;
std::stack<GLEWContext*>    Context::PIMPL::s_context;
std::stack<GemGlewXContext*>Context::PIMPL::s_xcontext;


Context::Context(void) 
  : m_pimpl(new PIMPL())
{
  push();
  std::string errstring="";

  GLenum err = glewInit();
  
  if (GLEW_OK != err) {
    if(GLEW_ERROR_GLX_VERSION_11_ONLY == err) {
      errstring="failed to init GLEW (glx): continuing anyhow - please report any problems to the gem-dev mailinglist!";
    } else if (GLEW_ERROR_GL_VERSION_10_ONLY) {
      errstring="failed to init GLEW: your system only supports openGL-1.0";
    } else {
      errstring="failed to init GLEW";
    }
  }

  post("GLEW version %s",glewGetString(GLEW_VERSION));

  if(!m_pimpl) {
    errstring="failed to init GemContext";
  }

  pop();

  if(!errstring.empty()) {
    if(m_pimpl)delete m_pimpl; m_pimpl=NULL;
    throw(GemException(errstring));
  }

}

Context::~Context(void) {
  if(m_pimpl)delete m_pimpl; m_pimpl=NULL;
}

bool Context::push(void) {
  GemMan::maxStackDepth[GemMan::STACKMODELVIEW]= m_pimpl->maxStackDepth[GemMan::STACKMODELVIEW];
  GemMan::maxStackDepth[GemMan::STACKCOLOR]=     m_pimpl->maxStackDepth[GemMan::STACKCOLOR];
  GemMan::maxStackDepth[GemMan::STACKTEXTURE]=   m_pimpl->maxStackDepth[GemMan::STACKTEXTURE];
  GemMan::maxStackDepth[GemMan::STACKPROJECTION]=m_pimpl->maxStackDepth[GemMan::STACKPROJECTION];

  m_pimpl->s_context.push(m_pimpl->context);
  m_pimpl->s_xcontext.push(m_pimpl->xcontext);
  m_pimpl->s_contextid.push(m_pimpl->contextid);
  return true;
}

bool Context::pop(void) {
  if(m_pimpl->s_context.empty())
    return false;

  if(m_pimpl->s_contextid.top() != m_pimpl->contextid) {
    error("hmm, invalid pop: %d!=%d", m_pimpl->s_contextid.top() , m_pimpl->contextid);
  }

  m_pimpl->s_context.pop();
  m_pimpl->s_xcontext.pop();
  m_pimpl->s_contextid.pop();
  return true;
}

unsigned int Context::getContextId(void) {
  if(PIMPL::s_contextid.empty())
    return 0;

  return PIMPL::s_contextid.top();
}

/* returns the last GemWindow that called makeCurrent()
 * LATER: what to do if this has been invalidated (e.g. because the context was destroyed) ? 
 */
GLEWContext*Context::getGlewContext(void) {
  if(PIMPL::s_context.empty())
    return NULL;

  return PIMPL::s_context.top();
}
GemGlewXContext*Context::getGlewXContext(void) {
  if(PIMPL::s_xcontext.empty())
    return NULL;

  return PIMPL::s_xcontext.top();
}


#if 0
#warning GLEWcontext fun
#else
GLEWContext*glewGetContext(void)     {return  gem::Context::getGlewContext();}
GemGlewXContext*wglewGetContext(void){return  gem::Context::getGlewXContext();}
GemGlewXContext*glxewGetContext(void){return  gem::Context::getGlewXContext();}
#endif
