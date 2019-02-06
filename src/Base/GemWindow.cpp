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
#include "GemWindow.h"
#include "Gem/Manager.h"
#include "RTE/MessageCallbacks.h"

#include "Gem/Settings.h"
#include "GemContext.h"
#include "Gem/Exception.h"
#include "GemBase.h"

#include <set>
#include <sstream>

namespace
{
bool sendContextDestroyedMsg(t_pd*x)
{
  if(!x) {
    return false;
  }
  t_symbol*s=gensym("__gem_context");
  t_atom a[1];
  SETFLOAT(a+0, 0);
  pd_typedmess(x, s, 1, a);
  return true;
}
};

class GemWindow::PIMPL
{
public:
  explicit PIMPL(GemWindow*gc)
    : parent(gc)
    , mycontext(0)
    , infoOut(0)
    , rejectOut(0)
    , dispatchClock(0)
    , dispatchTime(10.)
    , qClock(0)
  {
    qClock=clock_new(this, reinterpret_cast<t_method>(qCallBack));
    dispatchClock=clock_new(this,
                            reinterpret_cast<t_method>(dispatchCallBack));
  }
  ~PIMPL(void)
  {
    if(qClock) {
      clock_free (qClock);
    }
    qClock=0;
    if(dispatchClock) {
      clock_free (dispatchClock);
    }
    dispatchClock=0;
    if(infoOut) {
      outlet_free(infoOut);
    }
    infoOut=0;
    if(rejectOut) {
      outlet_free(rejectOut);
    }
    rejectOut=0;
  }

  GemWindow*parent;
  gem::Context*mycontext;

  t_outlet*infoOut;
  t_outlet*rejectOut;

  t_clock*dispatchClock;
  double dispatchTime;
  void dispatch(void)
  {
    parent->dispatch();
    clock_delay(dispatchClock, dispatchTime);
  }
  static void dispatchCallBack(PIMPL*x)
  {
    x->dispatch();
  }
  void undispatch(void)
  {
    clock_unset(dispatchClock);
  }

  std::vector<std::vector<t_atom> >qQueue;
  t_clock*qClock;

  void queue(std::vector<t_atom>alist)
  {
    if(alist.size()>0) {
      qQueue.push_back(alist);
    }

    requeue();
  }

  void queue(t_symbol*s,int argc, t_atom*argv)
  {
    std::vector<t_atom>alist;
    t_atom at[1];
    SETSYMBOL(at, s);
    alist.push_back(at[0]);
    while(argc-->0) {
      alist.push_back(*argv++);
    }
    queue(alist);
  }

  void sendInfo(std::vector<t_atom>alist)
  {
    int argc=alist.size();
    t_atom*ap=0;
    t_atom*argv=0;
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
    if(ap) {
      delete[]ap;
    }
  }
  void dequeue(void)
  {
    unsigned int i=0;
    for(i=0; i<qQueue.size(); i++) {
      sendInfo(qQueue[i]);
    }
    qQueue.clear();
  }

  /* qClock callback for dequeueing */
  static void qCallBack(PIMPL*x)
  {
    x->dequeue();
  }

  /* start the clock again */
  void requeue(void)
  {
    clock_delay(qClock, 0);
  }

  static std::set<GemWindow*>s_contexts;
}; /* GemWindow::PIMPL */
std::set<GemWindow*>GemWindow::PIMPL::s_contexts;

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
    /* in order to not hide the window-title, offset is >0 */
    m_xoffset(50), m_yoffset(50),
    m_border(true), m_fullscreen(false),
    m_buffer(2),
    m_title("Gem"),
    m_cursor(true),
    m_fsaa(0),
    m_context(0),
    m_transparent(0)
{
  int i;

  i=m_width;
  gem::Settings::get("window.width", i), m_width =i;
  i=m_height;
  gem::Settings::get("window.height", i), m_height=i;
  m_pimpl->infoOut   = outlet_new(this->x_obj, 0);
  m_pimpl->rejectOut = outlet_new(this->x_obj, 0);
  GemWindow::PIMPL::s_contexts.insert(this);
}
/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemWindow :: ~GemWindow()
{
  if(m_pimpl) {
    m_pimpl->mycontext=destroyContext(m_pimpl->mycontext);
    delete m_pimpl;
    m_pimpl=0;
  }
  GemWindow::PIMPL::s_contexts.erase(this);
}

void GemWindow::info(std::vector<t_atom>l)
{
  m_pimpl->queue(l);
}

void GemWindow::info(t_symbol*s, int argc, t_atom*argv)
{
  m_pimpl->queue(s, argc, argv);
}
void GemWindow::info(const std::string& s)
{
  info(gensym(s.c_str()), 0, 0);
}
void GemWindow::info(const std::string& s, int i)
{
  info(s, (t_float)i);
}

void GemWindow :: info(const std::string& s, t_float value)
{
  t_atom atom;
  SETFLOAT(&atom, value);
  info(gensym(s.c_str()), 1, &atom);
}
void GemWindow :: info(const std::string& s, const std::string& value)
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
void GemWindow::motion(int devId, int x, int y)
{
  t_atom ap[4];
  SETFLOAT (ap+0, devId);
  SETSYMBOL(ap+1, gensym("motion"));
  SETFLOAT (ap+2, x);
  SETFLOAT (ap+3, y);

  info(gensym("mouse"), 4, ap);
}
/* mouse buttons */
void GemWindow::button(int devId, int id, int state)
{
  t_atom ap[4];
  SETFLOAT (ap+0, devId);
  SETSYMBOL(ap+1, gensym("button"));
  SETFLOAT (ap+2, id);
  SETFLOAT (ap+3, state);

  info(gensym("mouse"), 4, ap);
}
/* mouse entering/leaving window */
void GemWindow::entry(int devId, int state)
{
  t_atom ap[3];
  SETFLOAT (ap+0, devId);
  SETSYMBOL(ap+1, gensym("entry"));
  SETFLOAT (ap+2, state);

  info(gensym("mouse"), 3, ap);
}

/* keyboard buttons */
void GemWindow::key(int devId, const std::string& sid, int iid, int state)
{
  t_atom ap[4];
  SETFLOAT (ap+0, devId);
  SETSYMBOL(ap+1, gensym("keyname"));
  SETSYMBOL(ap+2, gensym(sid.c_str()));
  SETFLOAT (ap+3, state);
  info(gensym("keyboard"), 4, ap);

  //SETFLOAT (ap+0, devId);
  SETSYMBOL(ap+1, gensym("key"));
  SETFLOAT (ap+2, iid);
  //SETFLOAT (ap+3, state);
  info(gensym("keyboard"), 4, ap);
}

void GemWindow::dimension(unsigned int w, unsigned int h)
{
  t_atom ap[2];
  SETFLOAT (ap+0, w);
  SETFLOAT (ap+1, h);

  info(gensym("dimen"), 2, ap);
}

void GemWindow::position(int x, int y)
{
  t_atom ap[2];
  SETFLOAT (ap+0, x);
  SETFLOAT (ap+1, y);

  info(gensym("offset"), 2, ap);
}

void GemWindow::dispatch()
{
  // LATER setup a clock that calls dispatch() every so often
}

gem::Context*GemWindow::createContext(void)
{
  gem::Context*ctx=new gem::Context();
  return ctx;
}
gem::Context*GemWindow::destroyContext(gem::Context*ctx)
{
  if(ctx) {
    delete ctx;
  }
  ctx=0;
  return ctx;
}
void GemWindow::stopInAllContexts(GemBase*obj)
{
  for (std::set<GemWindow*>::iterator it =
         GemWindow::PIMPL::s_contexts.begin();
       it!=GemWindow::PIMPL::s_contexts.end();
       ++it) {
    GemWindow*w=(*it);
    w->makeCurrent();
    t_pd*x=&obj->x_obj->ob_pd;
    sendContextDestroyedMsg(x);
  }
}

bool GemWindow::createGemWindow(void)
{
  if(!m_context) {
    try {
      if(m_pimpl->mycontext) {
        delete m_pimpl->mycontext;
      }
      m_pimpl->mycontext = new gem::Context();
    } catch (GemException&x) {
      m_context=0;
      error("%s", x.what());
      return false;
    }
    m_context=m_pimpl->mycontext;
  } else {
    m_pimpl->mycontext = 0;
  }

  m_pimpl->dispatch();

  return true;
}


void GemWindow::destroyGemWindow(void)
{
  // tell all objects that this context is vanishing
  sendContextDestroyedMsg(gensym("__gemBase")->s_thing);
  // do the rest
  m_pimpl->mycontext=destroyContext(m_pimpl->mycontext);
  m_pimpl->undispatch();
  m_context=m_pimpl->mycontext;
}

bool GemWindow::pushContext(void)
{
  if(!m_context) {
    return false;
  }

  if(!m_context->push()) {
    return false;
  }

  dispatch();
  return true;
}
bool GemWindow::popContext(void)
{
  return (m_context && m_context->pop());
}

void GemWindow::render(void)
{
  if(!makeCurrent()) {
    error("unable to switch to current window (do you have one?), cannot render!");
    return;
  }
  if(!pushContext()) {
    error("unable to switch to current context, cannot render!");
    return;
  }
  bang();
  if(m_buffer==2) {
    swapBuffers();
  }

  popContext();
}

void GemWindow:: bufferMess(int buf)
{
  switch(buf) {
  case 1:
  case 2:
    m_buffer=buf;
    break;
  default:
    error("buffer can only be '1' (single) or '2' (double) buffered");
    break;
  }
}
void GemWindow::   fsaaMess(int value)
{
  m_fsaa=value;
}

void GemWindow::titleMess(const std::string& s)
{
  m_title=s;
}
void GemWindow::borderMess(bool on)
{
  m_border=on;
}

void GemWindow::   fullscreenMess(int on)
{
  m_fullscreen=on;
}
void GemWindow::       offsetMess(int x, int y)
{
  m_xoffset=x;
  m_yoffset=y;
}

void GemWindow::       createMess(const std::string&)
{
  create();
}
void GemWindow::      destroyMess(void)
{
  destroy();
}

void GemWindow::       cursorMess(bool on)
{
  m_cursor=on;
}

void GemWindow::       transparentMess(bool on)
{
  m_transparent=on;
}

void GemWindow::       printMess(void)
{
  verbose(1, "@%p", this);
  if (!makeCurrent()) {
    error("OpenGL has not been initialized yet");
    post("create a window first!");
    return;
  }

  post("OpenGL info");
  post("\tVendor: %s", glGetString(GL_VENDOR));
  post("\tRenderer: %s", glGetString(GL_RENDERER));
  post("\tVersion: %s", glGetString(GL_VERSION));
  post("\tGLEW: %s", glewGetString(GLEW_VERSION));

  std::string extensions = (char*)glGetString(GL_EXTENSIONS);
  std::string ext;
  std::istringstream extStream(extensions);

  while (std::getline(extStream, ext, ' ')) {
    verbose(0, "\tExtension: %s", ext.c_str());    // Print extension string
  }
}

void GemWindow:: anyMess(t_symbol*s, int argc, t_atom*argv)
{
  outlet_anything(m_pimpl->rejectOut, s, argc, argv);
}


void GemWindow :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG0(classPtr, "bang", render);
  CPPEXTERN_MSG1(classPtr, "create", createMess, std::string);
  CPPEXTERN_MSG0(classPtr, "destroy", destroyMess);

  CPPEXTERN_MSG1(classPtr, "buffer", bufferMess, int);
  CPPEXTERN_MSG1(classPtr, "FSAA", fsaaMess, int);
  CPPEXTERN_MSG1(classPtr, "title", titleMess, std::string);
  CPPEXTERN_MSG2(classPtr, "dimen", dimensionsMess, unsigned int,
                 unsigned int);
  CPPEXTERN_MSG2(classPtr, "offset", offsetMess, int, int);
  CPPEXTERN_MSG1(classPtr, "fullscreen", fullscreenMess, int);
  CPPEXTERN_MSG1(classPtr, "border", borderMess, bool);
  CPPEXTERN_MSG1(classPtr, "cursor", cursorMess, bool);
  CPPEXTERN_MSG1(classPtr, "transparent", transparentMess, bool);

  CPPEXTERN_MSG0(classPtr, "print", printMess);

  struct _CB_any {
    static void callback(void*data, t_symbol*s, int argc, t_atom*argv)
    {
      GemWindow*gw=GetMyClass(data);
      gw->anyMess(s, argc, argv);
    }
    explicit _CB_any (struct _class*c)
    {
      class_addanything(c, reinterpret_cast<t_method>(_CB_any::callback));
    }
  };
  _CB_any CB_any (classPtr);
}
