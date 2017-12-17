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

#include "gemglfw2window.h"
#include "Gem/GemGL.h"
#include "GL/glfw.h"

#include "RTE/MessageCallbacks.h"
#include "Gem/Exception.h"

#define DEBUG ::startpost("%s:%d [%s]:: ", __FILE__, __LINE__, __FUNCTION__), ::post

/* whether we have a dinwo created or not
 * (GLFW<3 only allows a single window
 */
static gemglfw2window* s_window = 0;
static unsigned int s_instances=0;

CPPEXTERN_NEW(gemglfw2window);

/////////////////////////////////////////////////////////
//
// gemglfw2window
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemglfw2window :: gemglfw2window(void) :
  m_profile_major(0), m_profile_minor(0),
  m_wheelpos(0)
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
gemglfw2window :: ~gemglfw2window()
{
  s_instances--;
  destroyMess();
  if(s_instances==0) {
    glfwTerminate();
  }
}


bool gemglfw2window :: makeCurrent(void)
{
  return(0!=s_window);
}

void gemglfw2window :: swapBuffers(void)
{
  if(makeCurrent()) { // FIXME: is this needed?
    glfwSwapBuffers();
  }
}

void gemglfw2window :: doRender()
{
  // FIXME: ?????
  bang();
}
void gemglfw2window :: dispatch()
{
  if(0==s_window) {
    return;
  }
  glfwPollEvents();
}


/////////////////////////////////////////////////////////
// bufferMess
//
/////////////////////////////////////////////////////////
void gemglfw2window :: bufferMess(int buf)
{
  switch(buf) {
  case 1:
  case 2:
    m_buffer=buf;
    if(0!=s_window) {
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
void gemglfw2window :: titleMess(const std::string&s)
{
  m_title = s;
  if(makeCurrent()) {
    glfwSetWindowTitle(m_title.c_str());
  }
}
/////////////////////////////////////////////////////////
// dimensionsMess
//
/////////////////////////////////////////////////////////
void gemglfw2window :: dimensionsMess(unsigned int width,
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
    glfwSetWindowSize(m_width, m_height);
  }
}

/////////////////////////////////////////////////////////
// fullscreenMess
//
/////////////////////////////////////////////////////////
void gemglfw2window :: fullscreenMess(int on)
{
  m_fullscreen=on;
  // FIXXME: on the fly switching
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void gemglfw2window :: offsetMess(int x, int y)
{
  m_xoffset = x;
  m_yoffset = y;
  if(makeCurrent()) {
    glfwSetWindowPos(x, y);
  }
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void gemglfw2window :: glprofileMess(int major, int minor)
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

/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
bool gemglfw2window :: create(void)
{
  int mode = GLFW_WINDOW;
  if(0!=s_window) {
    error("window already made!");
    return false;
  }

  if(m_fullscreen) {
    mode=GLFW_FULLSCREEN;
  }

  glfwOpenWindowHint(GLFW_FSAA_SAMPLES, m_fsaa);


  if(m_profile_major) {
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR,
                       m_profile_major); // We want OpenGL 3.3
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, m_profile_minor);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE,
                       GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
  }

  if (!glfwOpenWindow(m_width, m_height,
                      8, 8, 8, 8,  /* RGBA bits */
                      24, 8,       /* depth/stencil bits */
                      mode)) {
    error("glfw couldn't create window");
    return false;
  }



  // FIXXME: single/double buffering

  if(!createGemWindow()) {
    destroyMess();
    return false;
  }
  s_window=this;

  titleMess(m_title);
  offsetMess(m_xoffset, m_yoffset);
  cursorMess(m_cursor);

  glfwSetWindowSizeCallback   (windowsizeCb);
  glfwSetWindowCloseCallback  (windowcloseCb);
  glfwSetWindowRefreshCallback(windowrefreshCb);
  glfwSetKeyCallback          (keyCb);
  glfwSetCharCallback         (charCb);
  glfwSetMouseButtonCallback  (mousebuttonCb);
  glfwSetMousePosCallback     (mouseposCb);
  glfwSetMouseWheelCallback   (mousewheelCb);
  dispatch();
  return (0!=s_window);
}
void gemglfw2window :: createMess(const std::string&)
{
  create();
}


/////////////////////////////////////////////////////////
// destroy window
//
/////////////////////////////////////////////////////////
void gemglfw2window :: destroy(void)
{
  destroyGemWindow();
  s_window=0;
  info("window", "closed");
}
void gemglfw2window :: destroyMess(void)
{
  if(makeCurrent()) {
    glfwCloseWindow();
  }
  destroy();
}

/////////////////////////////////////////////////////////
// cursorMess
//
/////////////////////////////////////////////////////////
void gemglfw2window :: cursorMess(bool setting)
{
  m_cursor=setting;
  if(makeCurrent()) {
    if(m_cursor) {
      glfwEnable(GLFW_MOUSE_CURSOR);
    } else {
      glfwDisable(GLFW_MOUSE_CURSOR);
    }
  }
}


void gemglfw2window::windowsizeCallback(int w, int h)
{
  dimension(w, h);
}
int gemglfw2window::windowcloseCallback()
{
  info("window", "destroy");
  return 0;
}
void gemglfw2window::windowrefreshCallback()
{
  info("window", "exposed");
}
void gemglfw2window::keyCallback(int key, int action)
{
  t_atom ap[3];
  SETSYMBOL(ap+0, gensym("key"));
  SETFLOAT (ap+1, key);
  SETFLOAT (ap+2, action);

  info(gensym("keyboard"), 3, ap);
}
void gemglfw2window::charCallback(int character, int action)
{
  t_atom ap[3];
  std::string sid;
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
void gemglfw2window::mousebuttonCallback(int button, int action)
{
  int devID=0;
  gemglfw2window:: button(devID, button, action);
}
void gemglfw2window::mouseposCallback(int x, int y)
{
  int devID=0;
  motion(devID, x, y);
}
#define WHEELUP   3
#define WHEELDOWN 4
void gemglfw2window::mousewheelCallback(int pos)
{
  int devID=0;
  if(m_wheelpos<pos) {
    while(m_wheelpos++<pos) {
      button(devID, WHEELUP, 1);
      button(devID, WHEELUP, 0);
    }
  } else   if(m_wheelpos>pos) {
    while(m_wheelpos-->pos) {
      button(devID, WHEELDOWN, 1);
      button(devID, WHEELDOWN, 0);
    }
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemglfw2window :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG2(classPtr, "glprofile", glprofileMess, int, int);
}

#ifdef CALLBACK
# undef CALLBACK
#endif
#define CALLBACK(x) if(0!=s_window)return s_window->x
void gemglfw2window::windowsizeCb(int w, int h)
{
  CALLBACK(windowsizeCallback(w, h));
}
int gemglfw2window::windowcloseCb()
{
  CALLBACK(windowcloseCallback());
  return 0;
}
void gemglfw2window::windowrefreshCb()
{
  CALLBACK(windowrefreshCallback());
}
void gemglfw2window::keyCb(int key, int action)
{
  CALLBACK(keyCallback(key, action));
}
void gemglfw2window::charCb(int character, int action)
{
  CALLBACK(charCallback(character, action));
}
void gemglfw2window::mousebuttonCb(int button, int action)
{
  CALLBACK(mousebuttonCallback(button, action));
}
void gemglfw2window::mouseposCb(int x, int y)
{
  CALLBACK(mouseposCallback(x, y));
}
void gemglfw2window::mousewheelCb(int pos)
{
  CALLBACK(mousewheelCallback(pos));
}
