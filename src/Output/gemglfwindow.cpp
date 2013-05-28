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
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Gem/GemConfig.h"

#include "gemglfwindow.h"
#include "Gem/GemGL.h"
#include "GL/glfw.h"

#include "RTE/MessageCallbacks.h"
#include "Gem/Exception.h"

#define DEBUG ::startpost("%s:%d [%s]:: ", __FILE__, __LINE__, __FUNCTION__), ::post

/* whether we have a dinwo created or not
 * (GLFW<3 only allows a single window
 */
static gemglfwindow* s_window = 0;
static unsigned int s_instances=0;

CPPEXTERN_NEW(gemglfwindow);

/////////////////////////////////////////////////////////
//
// gemglfwindow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemglfwindow :: gemglfwindow(void) :
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
gemglfwindow :: ~gemglfwindow()
{
  s_instances--;
  destroyMess();
  if(s_instances==0) {
    glfwTerminate();
  }
}


bool gemglfwindow :: makeCurrent(void){
  return(0!=s_window);
}

void gemglfwindow :: swapBuffers(void) {
  if(makeCurrent()) // FIXME: is this needed?
    glfwSwapBuffers();
}

void gemglfwindow :: doRender()
{
  // FIXME: ?????
  bang();
}
void gemglfwindow :: dispatch()
{
  if(0==s_window)return;
  glfwPollEvents();
}


/////////////////////////////////////////////////////////
// bufferMess
//
/////////////////////////////////////////////////////////
void gemglfwindow :: bufferMess(int buf)
{
  switch(buf) {
  case 1: case 2:
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
// fsaaMess
//
/////////////////////////////////////////////////////////
void gemglfwindow :: fsaaMess(int value)
{
  m_fsaa=value;
}

/////////////////////////////////////////////////////////
// titleMess
//
/////////////////////////////////////////////////////////
void gemglfwindow :: titleMess(std::string s)
{
  m_title = s;
  if(makeCurrent()){
    glfwSetWindowTitle(m_title.c_str());
  }
}
/////////////////////////////////////////////////////////
// dimensionsMess
//
/////////////////////////////////////////////////////////
void gemglfwindow :: dimensionsMess(unsigned int width, unsigned int height)
{
  if (width <= 0) {
    error("width must be greater than 0");
    return;
  }

  if (height <= 0 ) {
    error ("height must be greater than 0");
    return;
  }
  m_width = width;
  m_height = height;
  if(makeCurrent()){
    glfwSetWindowSize(m_width, m_height);
  }
}
/////////////////////////////////////////////////////////
// fullscreenMess
//
/////////////////////////////////////////////////////////
void gemglfwindow :: fullscreenMess(int on)
{
  m_fullscreen=on;
  // FIXXME: on the fly switching
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void gemglfwindow :: offsetMess(int x, int y)
{
  m_xoffset = x;
  m_yoffset = y;
  if(makeCurrent()){
    glfwSetWindowPos(x, y);
  }
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void gemglfwindow :: glprofileMess(int major, int minor)
{
  if(major>0) {
    m_profile_major=major;
    m_profile_minor=0;
    if(minor>0)
      m_profile_minor=minor;
  } else {
    m_profile_major=0;
    m_profile_minor=0;
  }
}

/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
bool gemglfwindow :: create(void)
{
  int mode = GLFW_WINDOW;
  if(0!=s_window) {
    error("window already made!");
    return false;
  }

  if(m_fullscreen)
    mode=GLFW_FULLSCREEN;

  glfwOpenWindowHint(GLFW_FSAA_SAMPLES, m_fsaa);


  if(m_profile_major) {
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, m_profile_major); // We want OpenGL 3.3
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, m_profile_minor);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
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
void gemglfwindow :: createMess(std::string) {
  create();
}


/////////////////////////////////////////////////////////
// destroy window
//
/////////////////////////////////////////////////////////
void gemglfwindow :: destroy(void)
{
  destroyGemWindow();
  s_window=0;
  info("window", "closed");
}
void gemglfwindow :: destroyMess(void)
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
void gemglfwindow :: cursorMess(bool setting)
{
  m_cursor=setting;
  if(makeCurrent()) {
    if(m_cursor)
      glfwEnable(GLFW_MOUSE_CURSOR);
    else
      glfwDisable(GLFW_MOUSE_CURSOR);
  }
}


void gemglfwindow::windowsizeCallback(int w, int h) {
  dimension(w, h);
}
int gemglfwindow::windowcloseCallback() {
  info("window", "destroy");
  return 0;
}
void gemglfwindow::windowrefreshCallback() {
  info("window", "exposed");
}
void gemglfwindow::keyCallback(int key, int action) {
  t_atom ap[3];
  SETSYMBOL(ap+0, gensym("key"));
  SETFLOAT (ap+1, key);
  SETFLOAT (ap+2, action);

  info(gensym("keyboard"), 3, ap);
}
void gemglfwindow::charCallback(int character, int action) {
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
void gemglfwindow::mousebuttonCallback(int button, int action) {
  gemglfwindow:: button(button, action);
}
void gemglfwindow::mouseposCallback(int x, int y) {
  motion(x, y);
}
#define WHEELUP   3
#define WHEELDOWN 4
void gemglfwindow::mousewheelCallback(int pos) {
  if(m_wheelpos<pos) {
    while(m_wheelpos++<pos) {
      button(WHEELUP, 1);
      button(WHEELUP, 0);
    }
  } else   if(m_wheelpos>pos) {
    while(m_wheelpos-->pos) {
      button(WHEELDOWN, 1);
      button(WHEELDOWN, 0);
    }
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemglfwindow :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG2(classPtr, "glprofile", glprofileMess, int, int);
}

#ifdef CALLBACK
# undef CALLBACK
#endif
#define CALLBACK(x) if(0!=s_window)return s_window->x
void gemglfwindow::windowsizeCb(int w, int h) {
  CALLBACK(windowsizeCallback(w, h));
}
int gemglfwindow::windowcloseCb() {
  CALLBACK(windowcloseCallback());
  return 0;
}
void gemglfwindow::windowrefreshCb() {
  CALLBACK(windowrefreshCallback());
}
void gemglfwindow::keyCb(int key, int action) {
  CALLBACK(keyCallback(key, action));
}
void gemglfwindow::charCb(int character, int action) {
  CALLBACK(charCallback(character, action));
}
void gemglfwindow::mousebuttonCb(int button, int action) {
  CALLBACK(mousebuttonCallback(button, action));
}
void gemglfwindow::mouseposCb(int x, int y) {
  CALLBACK(mouseposCallback(x, y));
}
void gemglfwindow::mousewheelCb(int pos) {
  CALLBACK(mousewheelCallback(pos));
}
