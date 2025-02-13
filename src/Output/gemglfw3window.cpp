///////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
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
#include <GLFW/glfw3.h>

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

static void error_callback(int err, const char* description)
{
  pd_error(0, "[glfw3window]: %s", description);
}

CPPEXTERN_NEW(gemglfw3window);

/* starting with GLFW-3.2, we can use glfwGetKeyName() */
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))

#if KERNEL_VERSION(GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION) >= KERNEL_VERSION(3,2,0)

#ifndef GLFW_KEY_SPACE
# define GLFW_KEY_SPACE 32
#endif
#ifndef GLFW_KEY_APOSTROPHE
# define GLFW_KEY_APOSTROPHE 39
#endif
#ifndef GLFW_KEY_COMMA
# define GLFW_KEY_COMMA 44
#endif
#ifndef GLFW_KEY_MINUS
# define GLFW_KEY_MINUS 45
#endif
#ifndef GLFW_KEY_PERIOD
# define GLFW_KEY_PERIOD 46
#endif
#ifndef GLFW_KEY_SLASH
# define GLFW_KEY_SLASH 47
#endif
#ifndef GLFW_KEY_0
# define GLFW_KEY_0 48
#endif
#ifndef GLFW_KEY_1
# define GLFW_KEY_1 49
#endif
#ifndef GLFW_KEY_2
# define GLFW_KEY_2 50
#endif
#ifndef GLFW_KEY_3
# define GLFW_KEY_3 51
#endif
#ifndef GLFW_KEY_4
# define GLFW_KEY_4 52
#endif
#ifndef GLFW_KEY_5
# define GLFW_KEY_5 53
#endif
#ifndef GLFW_KEY_6
# define GLFW_KEY_6 54
#endif
#ifndef GLFW_KEY_7
# define GLFW_KEY_7 55
#endif
#ifndef GLFW_KEY_8
# define GLFW_KEY_8 56
#endif
#ifndef GLFW_KEY_9
# define GLFW_KEY_9 57
#endif
#ifndef GLFW_KEY_SEMICOLON
# define GLFW_KEY_SEMICOLON 59
#endif
#ifndef GLFW_KEY_EQUAL
# define GLFW_KEY_EQUAL 61
#endif
#ifndef GLFW_KEY_A
# define GLFW_KEY_A 65
#endif
#ifndef GLFW_KEY_B
# define GLFW_KEY_B 66
#endif
#ifndef GLFW_KEY_C
# define GLFW_KEY_C 67
#endif
#ifndef GLFW_KEY_D
# define GLFW_KEY_D 68
#endif
#ifndef GLFW_KEY_E
# define GLFW_KEY_E 69
#endif
#ifndef GLFW_KEY_F
# define GLFW_KEY_F 70
#endif
#ifndef GLFW_KEY_G
# define GLFW_KEY_G 71
#endif
#ifndef GLFW_KEY_H
# define GLFW_KEY_H 72
#endif
#ifndef GLFW_KEY_I
# define GLFW_KEY_I 73
#endif
#ifndef GLFW_KEY_J
# define GLFW_KEY_J 74
#endif
#ifndef GLFW_KEY_K
# define GLFW_KEY_K 75
#endif
#ifndef GLFW_KEY_L
# define GLFW_KEY_L 76
#endif
#ifndef GLFW_KEY_M
# define GLFW_KEY_M 77
#endif
#ifndef GLFW_KEY_N
# define GLFW_KEY_N 78
#endif
#ifndef GLFW_KEY_O
# define GLFW_KEY_O 79
#endif
#ifndef GLFW_KEY_P
# define GLFW_KEY_P 80
#endif
#ifndef GLFW_KEY_Q
# define GLFW_KEY_Q 81
#endif
#ifndef GLFW_KEY_R
# define GLFW_KEY_R 82
#endif
#ifndef GLFW_KEY_S
# define GLFW_KEY_S 83
#endif
#ifndef GLFW_KEY_T
# define GLFW_KEY_T 84
#endif
#ifndef GLFW_KEY_U
# define GLFW_KEY_U 85
#endif
#ifndef GLFW_KEY_V
# define GLFW_KEY_V 86
#endif
#ifndef GLFW_KEY_W
# define GLFW_KEY_W 87
#endif
#ifndef GLFW_KEY_X
# define GLFW_KEY_X 88
#endif
#ifndef GLFW_KEY_Y
# define GLFW_KEY_Y 89
#endif
#ifndef GLFW_KEY_Z
# define GLFW_KEY_Z 90
#endif
#ifndef GLFW_KEY_LEFT_BRACKET
# define GLFW_KEY_LEFT_BRACKET 91
#endif
#ifndef GLFW_KEY_BACKSLASH
# define GLFW_KEY_BACKSLASH 92
#endif
#ifndef GLFW_KEY_RIGHT_BRACKET
# define GLFW_KEY_RIGHT_BRACKET 93
#endif
#ifndef GLFW_KEY_GRAVE_ACCENT
# define GLFW_KEY_GRAVE_ACCENT 96
#endif
#ifndef GLFW_KEY_WORLD_1
# define GLFW_KEY_WORLD_1 161
#endif
#ifndef GLFW_KEY_WORLD_2
# define GLFW_KEY_WORLD_2 162
#endif
#ifndef GLFW_KEY_ESCAPE
# define GLFW_KEY_ESCAPE 256
#endif
#ifndef GLFW_KEY_ENTER
# define GLFW_KEY_ENTER 257
#endif
#ifndef GLFW_KEY_TAB
# define GLFW_KEY_TAB 258
#endif
#ifndef GLFW_KEY_BACKSPACE
# define GLFW_KEY_BACKSPACE 259
#endif
#ifndef GLFW_KEY_INSERT
# define GLFW_KEY_INSERT 260
#endif
#ifndef GLFW_KEY_DELETE
# define GLFW_KEY_DELETE 261
#endif
#ifndef GLFW_KEY_RIGHT
# define GLFW_KEY_RIGHT 262
#endif
#ifndef GLFW_KEY_LEFT
# define GLFW_KEY_LEFT 263
#endif
#ifndef GLFW_KEY_DOWN
# define GLFW_KEY_DOWN 264
#endif
#ifndef GLFW_KEY_UP
# define GLFW_KEY_UP 265
#endif
#ifndef GLFW_KEY_PAGE_UP
# define GLFW_KEY_PAGE_UP 266
#endif
#ifndef GLFW_KEY_PAGE_DOWN
# define GLFW_KEY_PAGE_DOWN 267
#endif
#ifndef GLFW_KEY_HOME
# define GLFW_KEY_HOME 268
#endif
#ifndef GLFW_KEY_END
# define GLFW_KEY_END 269
#endif
#ifndef GLFW_KEY_CAPS_LOCK
# define GLFW_KEY_CAPS_LOCK 280
#endif
#ifndef GLFW_KEY_SCROLL_LOCK
# define GLFW_KEY_SCROLL_LOCK 281
#endif
#ifndef GLFW_KEY_NUM_LOCK
# define GLFW_KEY_NUM_LOCK 282
#endif
#ifndef GLFW_KEY_PRINT_SCREEN
# define GLFW_KEY_PRINT_SCREEN 283
#endif
#ifndef GLFW_KEY_PAUSE
# define GLFW_KEY_PAUSE 284
#endif
#ifndef GLFW_KEY_F1
# define GLFW_KEY_F1 290
#endif
#ifndef GLFW_KEY_F2
# define GLFW_KEY_F2 291
#endif
#ifndef GLFW_KEY_F3
# define GLFW_KEY_F3 292
#endif
#ifndef GLFW_KEY_F4
# define GLFW_KEY_F4 293
#endif
#ifndef GLFW_KEY_F5
# define GLFW_KEY_F5 294
#endif
#ifndef GLFW_KEY_F6
# define GLFW_KEY_F6 295
#endif
#ifndef GLFW_KEY_F7
# define GLFW_KEY_F7 296
#endif
#ifndef GLFW_KEY_F8
# define GLFW_KEY_F8 297
#endif
#ifndef GLFW_KEY_F9
# define GLFW_KEY_F9 298
#endif
#ifndef GLFW_KEY_F10
# define GLFW_KEY_F10 299
#endif
#ifndef GLFW_KEY_F11
# define GLFW_KEY_F11 300
#endif
#ifndef GLFW_KEY_F12
# define GLFW_KEY_F12 301
#endif
#ifndef GLFW_KEY_F13
# define GLFW_KEY_F13 302
#endif
#ifndef GLFW_KEY_F14
# define GLFW_KEY_F14 303
#endif
#ifndef GLFW_KEY_F15
# define GLFW_KEY_F15 304
#endif
#ifndef GLFW_KEY_F16
# define GLFW_KEY_F16 305
#endif
#ifndef GLFW_KEY_F17
# define GLFW_KEY_F17 306
#endif
#ifndef GLFW_KEY_F18
# define GLFW_KEY_F18 307
#endif
#ifndef GLFW_KEY_F19
# define GLFW_KEY_F19 308
#endif
#ifndef GLFW_KEY_F20
# define GLFW_KEY_F20 309
#endif
#ifndef GLFW_KEY_F21
# define GLFW_KEY_F21 310
#endif
#ifndef GLFW_KEY_F22
# define GLFW_KEY_F22 311
#endif
#ifndef GLFW_KEY_F23
# define GLFW_KEY_F23 312
#endif
#ifndef GLFW_KEY_F24
# define GLFW_KEY_F24 313
#endif
#ifndef GLFW_KEY_F25
# define GLFW_KEY_F25 314
#endif
#ifndef GLFW_KEY_KP_0
# define GLFW_KEY_KP_0 320
#endif
#ifndef GLFW_KEY_KP_1
# define GLFW_KEY_KP_1 321
#endif
#ifndef GLFW_KEY_KP_2
# define GLFW_KEY_KP_2 322
#endif
#ifndef GLFW_KEY_KP_3
# define GLFW_KEY_KP_3 323
#endif
#ifndef GLFW_KEY_KP_4
# define GLFW_KEY_KP_4 324
#endif
#ifndef GLFW_KEY_KP_5
# define GLFW_KEY_KP_5 325
#endif
#ifndef GLFW_KEY_KP_6
# define GLFW_KEY_KP_6 326
#endif
#ifndef GLFW_KEY_KP_7
# define GLFW_KEY_KP_7 327
#endif
#ifndef GLFW_KEY_KP_8
# define GLFW_KEY_KP_8 328
#endif
#ifndef GLFW_KEY_KP_9
# define GLFW_KEY_KP_9 329
#endif
#ifndef GLFW_KEY_KP_DECIMAL
# define GLFW_KEY_KP_DECIMAL 330
#endif
#ifndef GLFW_KEY_KP_DIVIDE
# define GLFW_KEY_KP_DIVIDE 331
#endif
#ifndef GLFW_KEY_KP_MULTIPLY
# define GLFW_KEY_KP_MULTIPLY 332
#endif
#ifndef GLFW_KEY_KP_SUBTRACT
# define GLFW_KEY_KP_SUBTRACT 333
#endif
#ifndef GLFW_KEY_KP_ADD
# define GLFW_KEY_KP_ADD 334
#endif
#ifndef GLFW_KEY_KP_ENTER
# define GLFW_KEY_KP_ENTER 335
#endif
#ifndef GLFW_KEY_KP_EQUAL
# define GLFW_KEY_KP_EQUAL 336
#endif
#ifndef GLFW_KEY_LEFT_SHIFT
# define GLFW_KEY_LEFT_SHIFT 340
#endif
#ifndef GLFW_KEY_LEFT_CONTROL
# define GLFW_KEY_LEFT_CONTROL 341
#endif
#ifndef GLFW_KEY_LEFT_ALT
# define GLFW_KEY_LEFT_ALT 342
#endif
#ifndef GLFW_KEY_LEFT_SUPER
# define GLFW_KEY_LEFT_SUPER 343
#endif
#ifndef GLFW_KEY_RIGHT_SHIFT
# define GLFW_KEY_RIGHT_SHIFT 344
#endif
#ifndef GLFW_KEY_RIGHT_CONTROL
# define GLFW_KEY_RIGHT_CONTROL 345
#endif
#ifndef GLFW_KEY_RIGHT_ALT
# define GLFW_KEY_RIGHT_ALT 346
#endif
#ifndef GLFW_KEY_RIGHT_SUPER
# define GLFW_KEY_RIGHT_SUPER 347
#endif
#ifndef GLFW_KEY_MENU
# define GLFW_KEY_MENU 348
#endif

static std::string get_keyname(int key, int scancode)
{
  const char*sid;

  sid = glfwGetKeyName(key, scancode);
  if(sid && *sid>32) {
    return (sid);
  }
  sid = glfwGetKeyName(GLFW_KEY_UNKNOWN, scancode);
  if(sid && *sid>32) {
    return (sid);
  }

  switch (key) {
  case GLFW_KEY_SPACE:
    return ("Space");
  case GLFW_KEY_ESCAPE:
    return ("Escape");
  case GLFW_KEY_ENTER:
    return ("Return");
  case GLFW_KEY_TAB:
    return ("Tab");
  case GLFW_KEY_BACKSPACE:
    return ("BackSpace");
  case GLFW_KEY_INSERT:
    return ("Insert");
  case GLFW_KEY_DELETE:
    return ("Delete");
  case GLFW_KEY_RIGHT:
    return ("Right");
  case GLFW_KEY_LEFT:
    return ("Left");
  case GLFW_KEY_DOWN:
    return ("Down");
  case GLFW_KEY_UP:
    return ("Up");
  case GLFW_KEY_PAGE_UP:
    return ("Prior");
  case GLFW_KEY_PAGE_DOWN:
    return ("Next");
  case GLFW_KEY_HOME:
    return ("Home");
  case GLFW_KEY_END:
    return ("End");
  case GLFW_KEY_CAPS_LOCK:
    return ("Caps_Lock");
  case GLFW_KEY_SCROLL_LOCK:
    return ("Scroll_Lock");
  case GLFW_KEY_NUM_LOCK:
    return ("Num_Lock");
  case GLFW_KEY_PRINT_SCREEN:
    return ("Print");
  case GLFW_KEY_PAUSE:
    return ("Pause");
  case GLFW_KEY_F1:
    return ("F1");
  case GLFW_KEY_F2:
    return ("F2");
  case GLFW_KEY_F3:
    return ("F3");
  case GLFW_KEY_F4:
    return ("F4");
  case GLFW_KEY_F5:
    return ("F5");
  case GLFW_KEY_F6:
    return ("F6");
  case GLFW_KEY_F7:
    return ("F7");
  case GLFW_KEY_F8:
    return ("F8");
  case GLFW_KEY_F9:
    return ("F9");
  case GLFW_KEY_F10:
    return ("F10");
  case GLFW_KEY_F11:
    return ("F11");
  case GLFW_KEY_F12:
    return ("F12");
  case GLFW_KEY_F13:
    return ("F13");
  case GLFW_KEY_F14:
    return ("F14");
  case GLFW_KEY_F15:
    return ("F15");
  case GLFW_KEY_F16:
    return ("F16");
  case GLFW_KEY_F17:
    return ("F17");
  case GLFW_KEY_F18:
    return ("F18");
  case GLFW_KEY_F19:
    return ("F19");
  case GLFW_KEY_F20:
    return ("F20");
  case GLFW_KEY_F21:
    return ("F21");
  case GLFW_KEY_F22:
    return ("F22");
  case GLFW_KEY_F23:
    return ("F23");
  case GLFW_KEY_F24:
    return ("F24");
  case GLFW_KEY_F25:
    return ("F25");
  case GLFW_KEY_KP_0:
    return ("KP_0");
  case GLFW_KEY_KP_1:
    return ("KP_1");
  case GLFW_KEY_KP_2:
    return ("KP_2");
  case GLFW_KEY_KP_3:
    return ("KP_3");
  case GLFW_KEY_KP_4:
    return ("KP_4");
  case GLFW_KEY_KP_5:
    return ("KP_5");
  case GLFW_KEY_KP_6:
    return ("KP_6");
  case GLFW_KEY_KP_7:
    return ("KP_7");
  case GLFW_KEY_KP_8:
    return ("KP_8");
  case GLFW_KEY_KP_9:
    return ("KP_9");
  case GLFW_KEY_KP_DECIMAL:
    return ("KP_Separator");
  case GLFW_KEY_KP_DIVIDE:
    return ("KP_Divide");
  case GLFW_KEY_KP_MULTIPLY:
    return ("KP_Multiply");
  case GLFW_KEY_KP_SUBTRACT:
    return ("KP_Subtract");
  case GLFW_KEY_KP_ADD:
    return ("KP_Add");
  case GLFW_KEY_KP_ENTER:
    return ("KP_Enter");
  case GLFW_KEY_KP_EQUAL:
    return ("KP_Equal");
  case GLFW_KEY_LEFT_SHIFT:
    return ("Shift_L");
  case GLFW_KEY_LEFT_CONTROL:
    return ("Control_L");
  case GLFW_KEY_LEFT_ALT:
    return ("Alt_L");
  case GLFW_KEY_LEFT_SUPER:
    return ("Super_L");
  case GLFW_KEY_RIGHT_SHIFT:
    return ("Shift_R");
  case GLFW_KEY_RIGHT_CONTROL:
    return ("Control_R");
  case GLFW_KEY_RIGHT_ALT:
    return ("Mode"); /* Alt_R? ISO_Level3_Shift? */
  case GLFW_KEY_RIGHT_SUPER:
    return ("Super_R");
  case GLFW_KEY_MENU:
    return ("Menu");
  default:
    break;
  }

  if(sid && *sid>32) {
    return (sid);
  }
  return ("<unknown>");
}
#endif


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
  m_width = m_height = 0;
  if(s_instances==0) {
    glfwSetErrorCallback(error_callback);
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
      static int warned = 0;
      if(!warned)
        post("changing buffer type will only effect newly created windows");
      warned = 1;
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
  if(makeCurrent()) {
    GLFWmonitor *monitor = NULL;
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    unsigned int width = m_width, height = m_height;
    if(on) {
      if (on<0 || on>count) {
        monitor = glfwGetPrimaryMonitor();
        logpost(0, 3+0, "switching to fullscreen on primary monitor: %s", glfwGetMonitorName(monitor));
      } else {
        monitor = monitors[on-1];
        logpost(0, 3+0, "switching to fullscreen on monitor #%d: %s", on, glfwGetMonitorName(monitor));
      }
    }
    if(monitor) {
      const GLFWvidmode* mode = glfwGetVideoMode(monitor);
      width = mode->width;
      height = mode->height;
    }
    if(!width) width = 500;
    if(!height) height = 500;
    glfwSetWindowMonitor(m_window, monitor, m_xoffset, m_yoffset, width, height, GLFW_DONT_CARE);
  }
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
  unsigned int width = m_width;
  unsigned int height = m_height;

  GLFWmonitor*monitor=NULL;
  if(m_fullscreen) {
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    if(m_fullscreen<0 || m_fullscreen > count) {
      monitor=glfwGetPrimaryMonitor();
    } else {
      monitor=monitors[m_fullscreen-1];
    }
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if(!width)width = mode->width;
    if(!height)height = mode->height;
  } else {
    if(!width)width = 500;
    if(!height)height = 500;
  }
  glfwWindowHint(GLFW_DECORATED, (m_border)?GLFW_TRUE:GLFW_FALSE);

  glfwWindowHint(GLFW_SAMPLES, m_fsaa);
  glfwWindowHint(GLFW_DOUBLEBUFFER, (1==m_buffer)?GLFW_FALSE:GLFW_TRUE);
  glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);

#ifdef GLFW_COCOA_FRAME_NAME
  /* setting a name here, enables "autosaving the window frame",
     which basically means, that hints like width/height are ignored
     at window creation.
     so we set it to an empty string...
   */
  glfwWindowHintString(GLFW_COCOA_FRAME_NAME, "");
#endif

  /* do we want OpenGL-ES? */
  glfwWindowHint(GLFW_CLIENT_API, m_gles?GLFW_OPENGL_ES_API:GLFW_OPENGL_API);

  /* which openGL-profile to use */
  if(m_profile_major) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_profile_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_profile_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  }
  m_window=glfwCreateWindow(width, height,
                            m_title.c_str(),
                            monitor,
                            NULL);

  if (!m_window) {
    error("glfw3 couldn't create window");
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
#if KERNEL_VERSION(GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION) >= KERNEL_VERSION(3,2,0)
  /* we use glfwGetKeyName() in they KeyCallback */
#else
  glfwSetCharCallback         (m_window, charCb);
#endif
  glfwSetMouseButtonCallback  (m_window, mousebuttonCb);
  glfwSetCursorPosCallback    (m_window, mouseposCb);
  glfwSetScrollCallback       (m_window, scrollCb);
  glfwSetFramebufferSizeCallback(m_window, framebuffersizeCb);
  dispatch();

  int fb_width=0, fb_height=0;
  glfwGetWindowSize(m_window, &fb_width, &fb_height);
  dimension(fb_width, fb_height);
  glfwGetFramebufferSize(m_window, &fb_width, &fb_height);
  framebuffersize(fb_width, fb_height);

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
void gemglfw3window::framebuffersizeCallback(int w, int h)
{
  framebuffersize(w, h);
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
#if KERNEL_VERSION(GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION) >= KERNEL_VERSION(3,2,0)
  gemglfw3window::key(0, get_keyname(key, scancode), key, action);
#else
  t_atom ap[4];
  SETFLOAT (ap+0, 0);
  SETSYMBOL(ap+1, gensym("key"));
  SETFLOAT (ap+2, key);
  SETFLOAT (ap+3, action);

  info(gensym("keyboard"), 4, ap);
#endif
}
void gemglfw3window::charCallback(unsigned int character)
{
  t_atom ap[4];
  std::string sid;
  int action=1; // ????
  switch(character) {
  case 32:
    sid = "Space";
    break;
  default:
    sid += character;
  }
  SETFLOAT (ap+0, 0);
  SETSYMBOL(ap+1, gensym("keyname"));
  SETSYMBOL(ap+2, gensym(sid.c_str()));
  SETFLOAT (ap+3, action);

  info(gensym("keyboard"), 4, ap);
}
void gemglfw3window::mousebuttonCallback(int button, int action, int mods)
{
  int devID=0;
  switch(button) {
  default: break;
    /* 'middle' and 'right' button are swapped in GLFW */
  case GLFW_MOUSE_BUTTON_MIDDLE:
    button=GLFW_MOUSE_BUTTON_RIGHT;
    break;
  case GLFW_MOUSE_BUTTON_RIGHT:
    button=GLFW_MOUSE_BUTTON_MIDDLE;
    break;
  }
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
  ::logpost(0, 3+0, "[gemglfw3window]\n\tGLFW compile version: %d.%d.%d\n\tGLFW runtime version: %s"
            , GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION
            , glfwGetVersionString()
    );
}

#ifdef CALLBACK
# undef CALLBACK
#endif
#define CALLBACK(x) gemglfw3window*g3w=s_windowmap[win]; if (!g3w){pd_error(0, "couldn't find [gemglfw3window] for window#%p", win); return;} else g3w->x
void gemglfw3window::windowsizeCb(GLFWwindow *win,int w, int h)
{
  CALLBACK(windowsizeCallback(w, h));
}
void gemglfw3window::framebuffersizeCb(GLFWwindow *win,int w, int h)
{
  CALLBACK(framebuffersizeCallback(w, h));
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
