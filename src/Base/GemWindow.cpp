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
#include "GemWindow.h"
#include "GemMan.h"
#include "RTE/MessageCallbacks.h"

#include "GemSettings.h"

#ifdef GEM_MULTICONTEXT
# warning multicontext rendering currently under development
#endif /* GEM_MULTICONTEXT */

static GLEWContext*s_glewcontext=NULL;
static GemGlewXContext*s_glewxcontext=NULL;
static unsigned int s_contextid;


static unsigned int GemWindow_newid(void)
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

static void GemWindow_freeid(unsigned int id)
{
  if(s_contextid==id) {
    s_contextid=0;
  }

  /* LATER reuse freed ids */
  id=0;
}


class GemWindow::PIMPL {
public:
  PIMPL(GemWindow*gc) : parent(gc),
                         context(NULL), xcontext(NULL), contextid(0),
                         infoOut(NULL),
                         dispatchClock(NULL),
                         dispatchTime(10.),
                         qClock(NULL)
  {
    qClock=clock_new(this, reinterpret_cast<t_method>(qCallBack));
    dispatchClock=clock_new(this, reinterpret_cast<t_method>(dispatchCallBack));
  }
  ~PIMPL(void) {
    if(qClock) clock_free (qClock);  qClock=NULL;
    if(dispatchClock) clock_free (dispatchClock);  dispatchClock=NULL;
    if(infoOut)outlet_free(infoOut); infoOut=NULL;
  }

  GemWindow*parent;

  GLint maxStackDepth[4];

  GLEWContext    *context;
  GemGlewXContext*xcontext;

  unsigned int contextid;

  t_outlet*infoOut;

  t_clock*dispatchClock;
  double dispatchTime;
  void dispatch(void) {
    parent->dispatch();
    clock_delay(dispatchClock, dispatchTime);
  }
  static void dispatchCallBack(PIMPL*x) {
    x->dispatch();
  }
  void undispatch(void) {
    clock_unset(dispatchClock);
  }

  std::vector<std::vector<t_atom> >qQueue;
  t_clock*qClock;

  void queue(std::vector<t_atom>alist) {
    if(alist.size()>0)
      qQueue.push_back(alist);

    requeue();
  }

  void queue(t_symbol*s,int argc, t_atom*argv) {
    std::vector<t_atom>alist;
    t_atom at[1];
    SETSYMBOL(at, s);
    alist.push_back(at[0]);
    while(argc-->0) {
      alist.push_back(*argv++);
    }
    queue(alist);
  }

  void sendInfo(std::vector<t_atom>alist) {
    int argc=alist.size();
    t_atom*ap=NULL;
    t_atom*argv=NULL;
#if 0
    argv=alist.data();
#else
    int i=0;
    ap=new t_atom[argc];
    argv=ap;
    for(i=0; i<argc; i++) {
      argv[i]=alist[i];
    }
#endif
    outlet_anything(infoOut, atom_getsymbol(argv), argc-1, argv+1); 
    if(ap)
      delete[]ap;
  }
  void dequeue(void) {
    int i=0;
    for(i=0; i<qQueue.size(); i++) {
      sendInfo(qQueue[i]);
    }
    qQueue.clear();
  }

  /* qClock callback for dequeueing */
  static void qCallBack(PIMPL*x) {
    x->dequeue();
  }

  /* start the clock again */
  void requeue(void) {
    clock_delay(qClock, 0);
  }


};


/////////////////////////////////////////////////////////
//
// GemWindow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemWindow :: GemWindow()
  : m_pimpl(new PIMPL(this)),
    m_width(500), m_height(500),
    m_xoffset(0), m_yoffset(0),
    m_border(true), m_fullscreen(false),
    m_buffer(2),
    m_title("Gem"), 
    m_cursor(true),
    m_fsaa(0)
{
  int i;

  i=m_width;  GemSettings::get("window.width" , i), m_width =i;
  i=m_height; GemSettings::get("window.height", i), m_height=i;
  m_pimpl->infoOut = outlet_new(this->x_obj, 0);
}
/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemWindow :: ~GemWindow()
{
  destroyContext();
  delete m_pimpl;
  m_pimpl=NULL;
}

void GemWindow::info(std::vector<t_atom>l) {
  m_pimpl->queue(l);
}

void GemWindow::info(t_symbol*s, int argc, t_atom*argv) {
  m_pimpl->queue(s, argc, argv);
}
void GemWindow::info(std::string s) { 
  info(gensym(s.c_str()), 0, NULL); 
}
void GemWindow::info(std::string s, int i) {
  info(s, (t_float)i);
}

void GemWindow :: info(std::string s, t_float value)
{
  t_atom atom;
  SETFLOAT(&atom, value);
  info(gensym(s.c_str()), 1, &atom); 
}
void GemWindow :: info(std::string s, std::string value)
{
  t_atom atom;
  SETSYMBOL(&atom, gensym(value.c_str()));
  info(gensym(s.c_str()), 1, &atom); 
}

void GemWindow :: bang(void)
{
  outlet_bang(m_pimpl->infoOut);
}




/* mouse movement */
void GemWindow::motion(int x, int y)
{
  t_atom ap[3];
  SETSYMBOL(ap+0, gensym("motion"));
  SETFLOAT (ap+1, x);
  SETFLOAT (ap+2, y);

  info(gensym("mouse"), 3, ap);
}
/* mouse buttons */
void GemWindow::button(int id, int state)
{
  t_atom ap[3];
  SETSYMBOL(ap+0, gensym("button"));
  SETFLOAT (ap+1, id);
  SETFLOAT (ap+2, state);

  info(gensym("mouse"), 3, ap);
}

/* keyboard buttons */
void GemWindow::key(std::string sid, int iid, int state) {
  t_atom ap[3];
  SETSYMBOL(ap+0, gensym("key"));
  SETFLOAT (ap+1, iid);
  SETFLOAT (ap+2, state);

  info(gensym("keyboard"), 3, ap);

  SETSYMBOL(ap+0, gensym("keyname"));
  SETSYMBOL(ap+1, gensym(sid.c_str()));
  //  SETFLOAT (ap+2, state);

  info(gensym("keyboard"), 3, ap);
}

void GemWindow::dimension(unsigned int w, unsigned int h) {
  t_atom ap[2];
  SETFLOAT (ap+0, w);
  SETFLOAT (ap+1, h);

  info(gensym("dimen"), 2, ap);
}

void GemWindow::position(int x, int y) {
  t_atom ap[2];
  SETFLOAT (ap+0, x);
  SETFLOAT (ap+1, y);

  info(gensym("offset"), 2, ap);
}

void GemWindow::dispatch() {
  // LATER setup a clock that calls dispatch() every so often
}

bool GemWindow::createContext(void){
  bool ret=true;
  static bool firsttime=true;
  unsigned int oldcontextid=s_contextid;
  GLEWContext*oldcontext=s_glewcontext;
  GemGlewXContext*oldcontextx=s_glewxcontext;

#ifdef GEM_MULTICONTEXT
  firsttime=true; /* always the first time with multicontexts */

  m_pimpl->context = new GLEWContext;
  m_pimpl->xcontext = new GemGlewXContext;
#endif /* GEM_MULTICONTEXT */

  s_glewcontext=m_pimpl->context;
  s_glewxcontext=m_pimpl->xcontext;
  

  m_pimpl->contextid=GemWindow_newid();
  s_contextid=m_pimpl->contextid;

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
  glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH,    m_pimpl->maxStackDepth+GemMan::STACKMODELVIEW);
  glGetIntegerv(GL_MAX_COLOR_MATRIX_STACK_DEPTH, m_pimpl->maxStackDepth+GemMan::STACKCOLOR);
  glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH,      m_pimpl->maxStackDepth+GemMan::STACKTEXTURE);
  glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH,   m_pimpl->maxStackDepth+GemMan::STACKPROJECTION);

  firsttime=false;

#ifdef GEM_MULTICONTEXT
# if 0
  /* LATER think about whether it is a good idea to restore the original context... */
  s_contextid=oldcontextid;
  s_glewcontext=oldcontext;
  oldcontextx=s_glewxcontext=oldcontextx;
# endif
#endif /* GEM_MULTICONTEXT */

  GemMan::m_windowState++;
  m_pimpl->dispatch();

  return true;
}


void GemWindow::destroyContext(void){
#ifdef GEM_MULTICONTEXT
  if(m_pimpl->context) {
    if(m_pimpl->context==s_glewcontext) {
      s_glewcontext=NULL;
    }
    delete m_pimpl->context;
  }
  m_pimpl->context=NULL;
#endif /* GEM_MULTICONTEXT */
  GemWindow_freeid(m_pimpl->contextid);
  m_pimpl->contextid=0;
  GemMan::m_windowState--;

  m_pimpl->undispatch();


}

bool GemWindow::makeGemWindowCurrent(void){
  GemMan::maxStackDepth[GemMan::STACKMODELVIEW]= m_pimpl->maxStackDepth[GemMan::STACKMODELVIEW];
  GemMan::maxStackDepth[GemMan::STACKCOLOR]=     m_pimpl->maxStackDepth[GemMan::STACKCOLOR];
  GemMan::maxStackDepth[GemMan::STACKTEXTURE]=   m_pimpl->maxStackDepth[GemMan::STACKTEXTURE];
  GemMan::maxStackDepth[GemMan::STACKPROJECTION]=m_pimpl->maxStackDepth[GemMan::STACKPROJECTION];

#ifdef GEM_MULTICONTEXT
  if(!m_pimpl->context) {
    return false;
    /* alternatively we could create a context on the fly... */
  }
  s_glewcontext=m_pimpl->context;
#endif /* GEM_MULTICONTEXT */
  s_contextid=m_pimpl->contextid;

  dispatch();
  return true;
}

bool GemWindow::assertCurrentContext(void) {
  if(!makeCurrent())
    return false;
  return makeGemWindowCurrent();
}

void GemWindow::render(void){
  if(assertCurrentContext()) {
    bang();
    if(m_buffer==2)
      swapBuffers();
  } else {
    error("unable to switch to current window (do you have one?), cannot render!");
  }
}

void GemWindow:: bufferMess(int buf) {
  switch(buf) {
  case 1: case 2:
    m_buffer=buf;
    break;
  default:
    error("buffer can only be '1' (single) or '2' (double) buffered");
    break;
  }
}
void GemWindow::   fsaaMess(int value) {
  m_fsaa=value;
}

void GemWindow::titleMess(std::string s) {
  m_title=s;
}
void GemWindow::borderMess(bool on) {
  m_border=on;
}

void GemWindow::   fullscreenMess(int on) {
  m_fullscreen=on;
}
void GemWindow::       offsetMess(int x, int y) {
  m_xoffset=x;
  m_yoffset=y;
}
 
void GemWindow::       createMess(std::string) {
  create();
}
void GemWindow::      destroyMess(void) {
  destroy();
}

void GemWindow::       cursorMess(bool on) {
  m_cursor=on;
}



void GemWindow::       printMess(void) {
  // nada
}




unsigned int GemWindow::getContextId(void) {
  return s_contextid;
}

#ifdef GEM_MULTICONTEXT
GLEWContext*GemWindow::getGlewContext(void) {
  if(NULL==s_glewcontext) {
    /* we should find another glew-context asap and make that one current! */
    return NULL;
  } else {
    return s_glewcontext;
  }

  return NULL;
}

GemGlewXContext*GemWindow::getGlewXContext(void) {
  if(NULL==s_glewxcontext) {
    /* we should find another glew-context asap and make that one current! */
    return NULL;
  } else {
    return s_glewxcontext;
  }

  return NULL;
}

GLEWContext*glewGetContext(void){return  GemWindow::getGlewContext();}
GemGlewXContext*wglewGetContext(void){return  GemWindow::getGlewXContext();}
GemGlewXContext*glxewGetContext(void){return  GemWindow::getGlewXContext();}

#endif /* GEM_MULTICONTEXT */


void GemWindow :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG0(classPtr, "bang", render);
  CPPEXTERN_MSG1(classPtr, "create", createMess, std::string);
  CPPEXTERN_MSG0(classPtr, "destroy", destroyMess);

  CPPEXTERN_MSG1(classPtr, "buffer", bufferMess, int);
  CPPEXTERN_MSG1(classPtr, "FSAA", fsaaMess, int);
  CPPEXTERN_MSG1(classPtr, "title", titleMess, std::string);
  CPPEXTERN_MSG2(classPtr, "dimen", dimensionsMess, unsigned int, unsigned int);
  CPPEXTERN_MSG2(classPtr, "offset", offsetMess, int, int);
  CPPEXTERN_MSG1(classPtr, "fullscreen", fullscreenMess, int);
  CPPEXTERN_MSG1(classPtr, "border", borderMess, bool);
  CPPEXTERN_MSG1(classPtr, "cursor", cursorMess, bool);

  //  CPPEXTERN_MSG0(classPtr, "print", printMess);
}
