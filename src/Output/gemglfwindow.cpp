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
static bool s_window = false;
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
gemglfwindow :: gemglfwindow(void) {
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
  return(s_window);
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
  if(!s_window)return;
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
    if(s_window) {
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
// createMess
//
/////////////////////////////////////////////////////////
bool gemglfwindow :: create(void)
{
  int mode = GLFW_WINDOW;
  if(s_window) {
    error("window already made!");
    return false;
  }

  if(m_fullscreen)
    mode=GLFW_FULLSCREEN;

  if (!glfwOpenWindow(m_width, m_height,
                      8, 8, 8, 8,  /* RGBA bits */
                      24, 8,       /* depth/stencil bits */
                      mode)) {
    error("glfw couldn't create window");
    return false;
  }
  glfwOpenWindowHint(GLFW_FSAA_SAMPLES, m_fsaa);

  // FIXXME: single/double buffering

  if(!createGemWindow()) {
    destroyMess();
    return false;
  }
  s_window=true;

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
  return s_window;
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
  s_window=false;
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

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemglfwindow :: obj_setupCallback(t_class *classPtr)
{
  int argc=0;
  char*argv=NULL;

  static bool firsttime=true;

  if(firsttime) {

  }
  firsttime=false;
}

void gemglfwindow::windowsizeCb(int w, int h) {
  ::startpost("%s:%d %s  ", __FILE__, __LINE__, __FUNCTION__); ::post("%d %d", w, h);
}
int gemglfwindow::windowcloseCb() {
  ::post("%s:%d %s  ", __FILE__, __LINE__, __FUNCTION__);
  return 0;
}
void gemglfwindow::windowrefreshCb() {
  ::post("%s:%d %s  ", __FILE__, __LINE__, __FUNCTION__);
}
void gemglfwindow::keyCb(int key, int action) {
  ::startpost("%s:%d %s  ", __FILE__, __LINE__, __FUNCTION__); ::post("%d[%d]", key, action);
}
void gemglfwindow::charCb(int character, int action) {
  ::startpost("%s:%d %s  ", __FILE__, __LINE__, __FUNCTION__); ::post("%d[%d]", character, action);
}
void gemglfwindow::mousebuttonCb(int button, int action) {
  ::startpost("%s:%d %s  ", __FILE__, __LINE__, __FUNCTION__); ::post("%d[%d]", button, action);
}
void gemglfwindow::mouseposCb(int x, int y) {
  ::startpost("%s:%d %s  ", __FILE__, __LINE__, __FUNCTION__); ::post("%d %d", x, y);
}
void gemglfwindow::mousewheelCb(int pos) {
  ::startpost("%s:%d %s  ", __FILE__, __LINE__, __FUNCTION__); ::post("%d", pos);
}
