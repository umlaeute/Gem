///////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2013 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Gem/GemConfig.h"

#include "Gem/GemGL.h"
#include "GLFW/glfw3.h"

#include "gemglfw3window.h"

#include "RTE/MessageCallbacks.h"
#include "Gem/Exception.h"
#include <map>

#define DEBUG ::startpost("%s:%d [%s]:: ", __FILE__, __LINE__, __FUNCTION__), ::post

/* whether we have a window created or not
 * (when all windows have been closed, we terminate GLFW)
 */
static unsigned int s_instances=0;

static std::map<GLFWwindow *, gemglfw3window*>s_windowmap;

CPPEXTERN_NEW(gemglfw3window);

/////////////////////////////////////////////////////////
//
// gemglfw3window
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemglfw3window :: gemglfw3window(void) :
  m_profile_major(0), m_profile_minor(0),
  m_window(0),
  m_gles(false)
{
  if(s_instances==0) {
    if(!glfwInit()) {
      throw(GemException("could not initialize GLFW infrastructure"));
    }
  }
  s_instances++;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemglfw3window :: ~gemglfw3window()
{
  s_instances--;
  destroyMess();
  if(s_instances==0) {
    glfwTerminate();
  }
}


bool gemglfw3window :: makeCurrent(void)
{
  if(!m_window) {
    return false;
  }
  glfwMakeContextCurrent(m_window);
  return true;
}

void gemglfw3window :: swapBuffers(void)
{
  if(makeCurrent()) { // FIXME: is this needed?
    glfwSwapBuffers(m_window);
  }
}

void gemglfw3window :: doRender()
{
  // FIXME: ?????
  bang();
}
void gemglfw3window :: dispatch()
{
  if(!m_window) {
    return;
  }
  glfwPollEvents();
}


/////////////////////////////////////////////////////////
// bufferMess
//
/////////////////////////////////////////////////////////
void gemglfw3window :: bufferMess(int buf)
{
  switch(buf) {
  case 1:
  case 2:
    m_buffer=buf;
    if(m_window) {
      post("changing buffer type will only effect newly created windows");
    }
    break;
  default:
    error("buffer can only be '1' (single) or '2' (double) buffered");
    break;
  }
}

/////////////////////////////////////////////////////////
// titleMess
//
/////////////////////////////////////////////////////////
void gemglfw3window :: titleMess(const std::string&s)
{
  m_title = s;
  if(makeCurrent()) {
    glfwSetWindowTitle(m_window, m_title.c_str());
  }
}
/////////////////////////////////////////////////////////
// dimensionsMess
//
/////////////////////////////////////////////////////////
void gemglfw3window :: dimensionsMess(unsigned int width,
                                      unsigned int height)
{
  if (width < 1) {
    error("width must be greater than 0");
    return;
  }

  if (height < 1) {
    error ("height must be greater than 0");
    return;
  }
  m_width = width;
  m_height = height;
  if(makeCurrent()) {
    glfwSetWindowSize(m_window, m_width, m_height);
  }
}
/////////////////////////////////////////////////////////
// fullscreenMess
//
/////////////////////////////////////////////////////////
void gemglfw3window :: fullscreenMess(int on)
{
  m_fullscreen=on;
  // FIXXME: on the fly switching
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void gemglfw3window :: offsetMess(int x, int y)
{
  m_xoffset = x;
  m_yoffset = y;
  if(makeCurrent()) {
    glfwSetWindowPos(m_window, x, y);
  }
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void gemglfw3window :: glprofileMess(int major, int minor)
{
  if(major>0) {
    m_profile_major=major;
    m_profile_minor=0;
    if(minor>0) {
      m_profile_minor=minor;
    }
  } else {
    m_profile_major=0;
    m_profile_minor=0;
  }
}

void gemglfw3window :: glesMess(bool gles)
{
  if(m_window && gles != m_gles) {
    post("openGL-ES mode will only effect newly created windows");
  }
  m_gles=gles;
}


/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
bool gemglfw3window :: create(void)
{

  if(m_window) {
    error("window already made!");
    return false;
  }
  glfwDefaultWindowHints();

  GLFWmonitor*monitor=NULL;
  if(m_fullscreen) {
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    if(m_fullscreen<0 || m_fullscreen >= count) {
      monitor=glfwGetPrimaryMonitor();
    } else {
      monitor=monitors[m_fullscreen];
    }
  }

  glfwWindowHint(GLFW_SAMPLES, m_fsaa);

  /* do we want OpenGL-ES */
  glfwWindowHint(GLFW_CLIENT_API, m_gles?GLFW_OPENGL_ES_API:GLFW_OPENGL_API);

  /* which openGL-profile to use */
  if(m_profile_major) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_profile_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_profile_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  }

  m_window=glfwCreateWindow(m_width, m_height,
                            m_title.c_str(),
                            monitor,
                            NULL);

  if (!m_window) {
    error("glfw couldn't create window");
    return false;
  }
  s_windowmap[m_window]=this;
  if(!makeCurrent()) {
    error("couldn't switch to glfw3-context");
  }

  // FIXXME: single/double buffering

  if(!createGemWindow()) {
    destroyMess();
    return false;
  }

  titleMess(m_title);
  offsetMess(m_xoffset, m_yoffset);
  cursorMess(m_cursor);

  glfwSetWindowSizeCallback   (m_window, windowsizeCb);
  glfwSetWindowCloseCallback  (m_window, windowcloseCb);
  glfwSetWindowRefreshCallback(m_window, windowrefreshCb);
  glfwSetKeyCallback          (m_window, keyCb);
  glfwSetCharCallback         (m_window, charCb);
  glfwSetMouseButtonCallback  (m_window, mousebuttonCb);
  glfwSetCursorPosCallback    (m_window, mouseposCb);
  glfwSetScrollCallback       (m_window, scrollCb);
  dispatch();
  return (0!=m_window);
}
void gemglfw3window :: createMess(const std::string&)
{
  create();
}


/////////////////////////////////////////////////////////
// destroy window
//
/////////////////////////////////////////////////////////
void gemglfw3window :: destroy(void)
{
  destroyGemWindow();
  s_windowmap.erase(m_window);
  m_window=0;
  info("window", "closed");
  glfwPollEvents();
}
void gemglfw3window :: destroyMess(void)
{
  if(makeCurrent()) {
    glfwPollEvents();
    glfwDestroyWindow(m_window);
    glfwPollEvents();
  }
  destroy();
}

/////////////////////////////////////////////////////////
// cursorMess
//
/////////////////////////////////////////////////////////
void gemglfw3window :: cursorMess(bool setting)
{
  m_cursor=setting;
  if(m_window) {
    glfwSetInputMode(m_window, GLFW_CURSOR,
                     m_cursor?GLFW_CURSOR_NORMAL:GLFW_CURSOR_HIDDEN); /* GLFW_CURSOR_DISABLED */
  }
}


void gemglfw3window::windowsizeCallback(int w, int h)
{
  dimension(w, h);
}
int gemglfw3window::windowcloseCallback()
{
  info("window", "destroy");
  return 0;
}
void gemglfw3window::windowrefreshCallback()
{
  info("window", "exposed");
}
void gemglfw3window::keyCallback(int key, int scancode, int action,
                                 int mods)
{
  t_atom ap[3];
  SETSYMBOL(ap+0, gensym("key"));
  SETFLOAT (ap+1, key);
  SETFLOAT (ap+2, action);

  info(gensym("keyboard"), 3, ap);
}
void gemglfw3window::charCallback(unsigned int character)
{
  t_atom ap[3];
  std::string sid;
  int action=-1; // ????
  switch(character) {
  case 32:
    sid = "Space";
    break;
  default:
    sid += character;
  }
  SETSYMBOL(ap+0, gensym("keyname"));
  SETSYMBOL(ap+1, gensym(sid.c_str()));
  SETFLOAT (ap+2, action);

  info(gensym("keyboard"), 3, ap);
}
void gemglfw3window::mousebuttonCallback(int button, int action, int mods)
{
  int devID=0;
  gemglfw3window:: button(devID, button, action);
}
void gemglfw3window::mouseposCallback(float x, float y)
{
  int devID=0;
  motion(devID, x, y);
}
#define WHEELUP   3
#define WHEELDOWN 4

#define WHEELLEFT  5
#define WHEELRIGHT 6


void gemglfw3window::scrollCallback(float x, float y)
{
  int i, dir;
  int devID=0;
  dir=(x>0)?WHEELUP:WHEELDOWN;
  if(x<0) {
    x=-x;
  }
  for(i=0; i<x; i++) {
    button(devID, dir, 1);
    button(devID, dir, 0);
  }

  dir=(y>0)?WHEELRIGHT:WHEELLEFT;
  if(y<0) {
    y=-y;
  }
  for(i=0; i<y; i++) {
    button(devID, dir, 1);
    button(devID, dir, 0);
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemglfw3window :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG2(classPtr, "glprofile", glprofileMess, int, int);
  CPPEXTERN_MSG1(classPtr, "gles", glesMess, bool);
}

#ifdef CALLBACK
# undef CALLBACK
#endif
#define CALLBACK(x) gemglfw3window*g3w=s_windowmap[win]; if (!g3w){::error("couldn't find [gemglfw3window] for window#%p", win); return;} else g3w->x
void gemglfw3window::windowsizeCb(GLFWwindow *win,int w, int h)
{
  CALLBACK(windowsizeCallback(w, h));
}
void gemglfw3window::windowcloseCb(GLFWwindow *win)
{
  CALLBACK(windowcloseCallback());
}
void gemglfw3window::windowrefreshCb(GLFWwindow *win)
{
  CALLBACK(windowrefreshCallback());
}
void gemglfw3window::keyCb(GLFWwindow *win, int key, int scancode,
                           int action, int mods)
{
  CALLBACK(keyCallback(key, scancode, action, mods));
}
void gemglfw3window::charCb(GLFWwindow *win, unsigned int character)
{
  CALLBACK(charCallback(character));
}
void gemglfw3window::mousebuttonCb(GLFWwindow *win, int button, int action,
                                   int mods)
{
  CALLBACK(mousebuttonCallback(button, action, mods));
}
void gemglfw3window::mouseposCb(GLFWwindow *win, double x, double y)
{
  CALLBACK(mouseposCallback(x, y));
}
void gemglfw3window::scrollCb(GLFWwindow *win, double x, double y)
{
  CALLBACK(scrollCallback(x, y));
}
