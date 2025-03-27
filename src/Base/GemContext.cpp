////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2009-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "GemContext.h"
#include "Gem/Manager.h"
#include "Gem/Exception.h"

#include "Gem/RTE.h"

#include <stack>
#include <set>

#ifdef GEM_MULTICONTEXT
# warning multicontext rendering currently under development
#endif /* GEM_MULTICONTEXT */

using namespace gem;

class Context::PIMPL
{
  void initMaxDepth(int id) {
    GLenum pname = 0;
    switch (id) {
    case GemMan::STACKMODELVIEW: pname = GL_MAX_MODELVIEW_STACK_DEPTH; break;
    case GemMan::STACKTEXTURE: pname = GL_MAX_TEXTURE_STACK_DEPTH; break;
    case GemMan::STACKPROJECTION: pname = GL_MAX_PROJECTION_STACK_DEPTH; break;
    case GemMan::STACKCOLOR: pname = GL_MAX_COLOR_MATRIX_STACK_DEPTH; break;
    default:
      return;
    }
    glGetIntegerv(pname, maxStackDepth+id);
  }
public:
  PIMPL(void) :
#ifdef GEM_MULTICONTEXT
    context(new GLEWContext),
#else
    context(NULL),
#endif
#ifdef GemGlewXContext
    xcontext(new GemGlewXContext),
#endif /* GemGlewXContext */
    contextid(makeID())
  {
    /* check the stack-sizes */
    initMaxDepth(GemMan::STACKMODELVIEW);
    initMaxDepth(GemMan::STACKTEXTURE);
    initMaxDepth(GemMan::STACKPROJECTION);

    maxStackDepth[GemMan::STACKCOLOR]=0;
  }

  PIMPL(const PIMPL&p) :
#ifdef GEM_MULTICONTEXT
    context(new GLEWContext(*p.context)),
#else
    context(NULL),
#endif
#ifdef GemGlewXContext
    xcontext(new GemGlewXContext(*p.xcontext)),
#endif /* GemGlewXContext */
    contextid(makeID())
  {
    /* check the stack-sizes */
    initMaxDepth(GemMan::STACKMODELVIEW);
    initMaxDepth(GemMan::STACKCOLOR);
    initMaxDepth(GemMan::STACKTEXTURE);
    initMaxDepth(GemMan::STACKPROJECTION);
  }

  ~PIMPL(void)
  {
    freeID(contextid);
#ifdef GEM_MULTICONTEXT
    if(context ) {
      delete context;
    }
    context=NULL;
# ifdef GemGlewXContext
    if(xcontext) {
      delete xcontext;
    }
    xcontext=0;
# endif /* GemGlewXContext */
#endif
  }

  GLint maxStackDepth[4];

  GLEWContext    *context;
#ifdef GemGlewXContext
  GemGlewXContext*xcontext;
#endif /* GemGlewXContext */

  unsigned int contextid;

  // LATER: reusing IDs prevents a memleak in gem::ContextData
  // LATER: reusing IDs might make us re-use invalid gem::ContextData!
  static std::set<unsigned int>s_takenIDs;
  static unsigned int makeID(void) //  GemContext_newid
  {
    unsigned int id=0;
#ifdef GEM_MULTICONTEXT
    while(s_takenIDs.find(id) != s_takenIDs.end()) {
      id++;
    }
#endif /* GEM_MULTICONTEXT */
    s_takenIDs.insert(id);
    return id;
  }
  static void freeID(unsigned int id)
  {
    /* LATER reuse freed ids */
    /* LATER remove this ID from the s_contextid stack and related (x)context */
    s_takenIDs.erase(id);
  }

  static unsigned int s_contextid;
  static GLEWContext*s_context;
#ifdef GemGlewXContext
  static GemGlewXContext*s_xcontext;
#endif /* GemGlewXContext */
};
unsigned int    Context::PIMPL::s_contextid=0;
GLEWContext*    Context::PIMPL::s_context=NULL;
#ifdef GemGlewXContext
GemGlewXContext*Context::PIMPL::s_xcontext=NULL;
#endif /* GemGlewXContext */
std::set<unsigned int>      Context::PIMPL::s_takenIDs;

Context::Context(void)
  : m_pimpl(new PIMPL())
{
  if(!m_pimpl) {
    throw(GemException("failed to initialize GemContext"));
  }
  std::string errstring="";
  push(); // make our context the current one, for subsequent glew-calls
  GLenum err = glewInit();

  if (GLEW_OK != err) {
    if(GLEW_ERROR_GLX_VERSION_11_ONLY == err) {
      errstring=
        "failed to init GLEW (glx): continuing anyhow - please report any problems to the gem-dev mailinglist!";
    } else if (GLEW_ERROR_GL_VERSION_10_ONLY == err) {
      errstring="failed to init GLEW: your system only supports openGL-1.0";
    } else {
      errstring="failed to init GLEW";
    }
  } else {
    if(GLEW_ARB_imaging) {
      pimpl->initMaxDepth(GemMan::STACKCOLOR);
    }
    }
  }

  pop();

  if(!errstring.empty()) {
    delete m_pimpl;
    m_pimpl=NULL;
    throw(GemException(errstring));
  }

  /* update the stack variables (late initialization) */
  push();
  pop();
  GemMan::m_windowState++;
}

Context::Context(const Context&c)
  : m_pimpl(new PIMPL(*(c.m_pimpl)))
{
  push();
  post("foo GLEW version %s",glewGetString(GLEW_VERSION));
  pop();
}

Context&Context::operator=(const Context&c)
{
  if(&c == this || c.m_pimpl == m_pimpl) {
    return (*this);
  }

  if(m_pimpl) {
    delete m_pimpl;
  }
  m_pimpl=new PIMPL(*c.m_pimpl);
  push();
  pop();

  return(*this);
}


Context::~Context(void)
{
  if(m_pimpl) {
    delete m_pimpl;
  }
  m_pimpl=NULL;
  GemMan::m_windowState--;
}

bool Context::push(void)
{
  GemMan::maxStackDepth[GemMan::STACKMODELVIEW]=
    m_pimpl->maxStackDepth[GemMan::STACKMODELVIEW];
  GemMan::maxStackDepth[GemMan::STACKCOLOR]=
    m_pimpl->maxStackDepth[GemMan::STACKCOLOR];
  GemMan::maxStackDepth[GemMan::STACKTEXTURE]=
    m_pimpl->maxStackDepth[GemMan::STACKTEXTURE];
  GemMan::maxStackDepth[GemMan::STACKPROJECTION]=
    m_pimpl->maxStackDepth[GemMan::STACKPROJECTION];

  m_pimpl->s_context=m_pimpl->context;
#ifdef GemGlewXContext
  m_pimpl->s_xcontext=m_pimpl->xcontext;
#endif /* GemGlewXContext */
  m_pimpl->s_contextid=m_pimpl->contextid;

  return true;
}

bool Context::isActive(void)
{
  return (m_pimpl->s_contextid == m_pimpl->contextid);
}

bool Context::pop(void)
{
  return true;
}

unsigned int Context::getContextId(void)
{
  return PIMPL::s_contextid;
}

/* returns the last GemWindow that called makeCurrent()
 * LATER: what to do if this has been invalidated (e.g. because the context was destroyed) ?
 */
GLEWContext*Context::getGlewContext(void)
{
  return PIMPL::s_context;
}
GLEWContext*glewGetContext(void)
{
  return  gem::Context::getGlewContext();
}

#ifdef GemGlewXContext
GemGlewXContext*Context::getGlewXContext(void)
{
  return PIMPL::s_xcontext;
}
GemGlewXContext*wglewGetContext(void)
{
  return  gem::Context::getGlewXContext();
}
GemGlewXContext*glxewGetContext(void)
{
  return  gem::Context::getGlewXContext();
}
#endif /* GemGlewXContext */
