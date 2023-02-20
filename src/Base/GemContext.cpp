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
public:
  PIMPL(void) :
#ifdef GEM_MULTICONTEXT
    context(new GladGLContext()),
#else
    context(NULL),
#endif
    xcontext(0),
    contextid(makeID())
  { }

  PIMPL(const PIMPL&p) :
#ifdef GEM_MULTICONTEXT
    context(new GladGLContext(*p.context)),
#else
    context(NULL),
#endif
    xcontext(0),
    contextid(makeID())
  { }

  ~PIMPL(void)
  {
    freeID(contextid);
#ifdef GEM_MULTICONTEXT
    if(context ) {
      delete context;
    }
#endif
    context=0;
    xcontext=0;
  }

  void initialize(void)
  {
    /* check the stack-sizes */
    glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH,
                  maxStackDepth+GemMan::STACKMODELVIEW);
    glGetIntegerv(GL_MAX_COLOR_MATRIX_STACK_DEPTH,
                  maxStackDepth+GemMan::STACKCOLOR);
    glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH,
                  maxStackDepth+GemMan::STACKTEXTURE);
    glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH,
                  maxStackDepth+GemMan::STACKPROJECTION);

    GLint colorstack = 0;
    if(GLEW_ARB_imaging) {
      glGetIntegerv(GL_MAX_COLOR_MATRIX_STACK_DEPTH, &colorstack);
    }
    maxStackDepth[GemMan::STACKCOLOR]=colorstack;
  }

  GLint maxStackDepth[4];

  GladGLContext    *context;
  void*xcontext;

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
  static GladGLContext*s_context;
  static void*s_xcontext;
};
unsigned int    Context::PIMPL::s_contextid=0;
GladGLContext*  Context::PIMPL::s_context=NULL;
void*Context::PIMPL::s_xcontext=NULL;
std::set<unsigned int>      Context::PIMPL::s_takenIDs;

Context::Context(void)
  : m_pimpl(new PIMPL())
{
  if(!m_pimpl) {
    throw(GemException("failed to initialize GemContext"));
  }
  push(); // make our context the current one, for subsequent glew-calls

  int version = gladLoaderLoadGLContext(m_pimpl->context);
  if(!version) {
    delete m_pimpl;
    m_pimpl=NULL;
    throw(GemException("failed to init openGL/glad"));
  }
  post("GEM: openGL/glad %d.%d", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
  m_pimpl->initialize();

  pop();

  /* update the stack variables (late initialization) */
  push();
  pop();
  GemMan::m_windowState++;
}

Context::Context(const Context&c)
  : m_pimpl(new PIMPL(*(c.m_pimpl)))
{
  push();
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
  m_pimpl->s_xcontext=m_pimpl->xcontext;
  m_pimpl->s_contextid=m_pimpl->contextid;
  return true;
}

bool Context::pop(void)
{
  return true;
}

bool Context::initializeXContext(void*display, int screen)
{
#ifdef __APPLE__
#elif defined _WIN32
  if(display) {
    HDC hdc=static_cast<HDC>(display);
    int version = gladLoaderLoadWGL(hdc);
    if(!version) {
      pd_error(0, "failed to init WGL");
      return false;
    }
    post("GEM: WGL %d.%d", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
  }
#elif defined(__linux__) || defined(__FreeBSD_kernel__)
  if(display) {
    Display*dpy=static_cast<Display*>(display);
    int version = gladLoaderLoadGLX(dpy, screen);
    if(!version) {
      pd_error(0, "failed to init GLX");
      return false;
    }
    post("GEM: GLX %d.%d", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
  }
#endif
  return true;
}
unsigned int Context::getContextId(void)
{
  return PIMPL::s_contextid;
}

/* returns the last GemWindow that called makeCurrent()
 * LATER: what to do if this has been invalidated (e.g. because the context was destroyed) ?
 */
void*Context::getCurrentContext(void)
{
  return PIMPL::s_context;
}
void*gemGetCurrentContext(void)
{
  return gem::Context::getCurrentContext();
}

void*Context::getCurrentXContext(void)
{
  return PIMPL::s_xcontext;
}
void*gemGetCurrentXContext(void)
{
  return gem::Context::getCurrentXContext();
}
