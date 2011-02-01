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

#ifdef GEM_MULTICONTEXT
# warning multicontext rendering currently under development
#endif /* GEM_MULTICONTEXT */

static GLEWContext*s_glewcontext=NULL;
static GemGlewXContext*s_glewxcontext=NULL;
static unsigned int s_contextid;


static unsigned int GemContext_newid(void)
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

static void GemContext_freeid(unsigned int id)
{
  if(s_contextid==id) {
    s_contextid=0;
  }

  /* LATER reuse freed ids */
  id=0;
}


class GemContext::PIMPL {
public:
  PIMPL(void) {
  }
  ~PIMPL(void) {
  }
  GLEWContext    *context;
  GemGlewXContext*xcontext;
};


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
    m_infoOut(NULL),
    m_contextid(0),
    m_pimpl(new PIMPL)
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
  delete m_pimpl;
  m_pimpl=NULL;
}

void GemContext::info(t_symbol*s, int argc, t_atom*argv) {
  if(m_infoOut) {
    outlet_anything(m_infoOut, s, argc, argv); 
  }
}
void GemContext::info(t_symbol*s) { 
  info(s, 0, NULL); 
}
void GemContext::info(t_symbol*s, int i) {
  info(s, (t_float)i);
}

void GemContext :: info(t_symbol*s, t_float value)
{
  t_atom atom;
  SETFLOAT(&atom, value);
  info(s, 1, &atom); 
}
void GemContext :: info(t_symbol*s, t_symbol*value)
{
  t_atom atom;
  SETSYMBOL(&atom, value);
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
  unsigned int oldcontextid=s_contextid;
  GLEWContext*oldcontext=s_glewcontext;
  GemGlewXContext*oldcontextx=s_glewxcontext;

#ifdef GEM_MULTICONTEXT
  firsttime=1; /* always the first time with multicontexts */

  m_pimpl->context = new GLEWContext;
  m_pimpl->xcontext = new GemGlewXContext;
#endif /* GEM_MULTICONTEXT */

  s_glewcontext=m_pimpl->context;
  s_glewxcontext=m_pimpl->xcontext;
  

  m_contextid=GemContext_newid();
  s_contextid=m_contextid;

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
  glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH,    m_maxStackDepth+GemMan::STACKMODELVIEW);
  glGetIntegerv(GL_MAX_COLOR_MATRIX_STACK_DEPTH, m_maxStackDepth+GemMan::STACKCOLOR);
  glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH,      m_maxStackDepth+GemMan::STACKTEXTURE);
  glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH,   m_maxStackDepth+GemMan::STACKPROJECTION);

  firsttime=0;

#ifdef GEM_MULTICONTEXT
# if 0
  /* LATER think about whether it is a good idea to restore the original context... */
  s_contextid=oldcontextid;
  s_glewcontext=oldcontext;
  oldcontextx=s_glewxcontext=oldcontextx;
# endif
#endif /* GEM_MULTICONTEXT */

  GemMan::m_windowState++;
  return true;
}


void GemContext::destroy(void){
#ifdef GEM_MULTICONTEXT
  if(m_pimpl->context) {
    if(m_pimpl->context==s_glewcontext) {
      s_glewcontext=NULL;
    }
    delete m_pimpl->context;
  }
  m_pimpl->context=NULL;
#endif /* GEM_MULTICONTEXT */
  GemContext_freeid(m_contextid);
  m_contextid=0;
  GemMan::m_windowState--;
}

bool GemContext::makeCurrent(void){
  GemMan::maxStackDepth[GemMan::STACKMODELVIEW]=m_maxStackDepth[GemMan::STACKMODELVIEW];
  GemMan::maxStackDepth[GemMan::STACKCOLOR]=m_maxStackDepth[GemMan::STACKCOLOR];
  GemMan::maxStackDepth[GemMan::STACKTEXTURE]=m_maxStackDepth[GemMan::STACKTEXTURE];
  GemMan::maxStackDepth[GemMan::STACKPROJECTION]=m_maxStackDepth[GemMan::STACKPROJECTION];

#ifdef GEM_MULTICONTEXT
  if(!m_pimpl->context) {
    return false;
    /* alternatively we could create a context on the fly... */
  }
  s_glewcontext=m_pimpl->context;
#endif /* GEM_MULTICONTEXT */
  s_contextid=m_contextid;
  return true;
}


void GemContext::dimensionsMess(void){
   t_atom ap[2];
  SETFLOAT (ap+0, m_width);
  SETFLOAT (ap+1, m_height);

  info(gensym("dimen"), 2, ap); 
}


unsigned int GemContext::getContextId(void) {
  return s_contextid;
}

#ifdef GEM_MULTICONTEXT
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
#endif /* GEM_MULTICONTEXT */
