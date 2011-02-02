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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "gemglutwindow.h"

#include "Base/GemGL.h"
#include "GL/freeglut.h"

#include <stdio.h>

gemglutwindow::t_list *gemglutwindow::ggw_list = NULL;

gemglutwindow* gemglutwindow::list_find(int win)
{
  t_list*mylist=0;

  for(mylist=ggw_list; mylist; mylist=mylist->next) {
    if(mylist->window == win)
      return mylist->object;
  }
  /* not found */
  return 0;
}
void gemglutwindow::list_add(gemglutwindow*obj, int win)
{
  t_list*last=ggw_list;
  t_list*element=new t_list;
  element->object=obj;
  element->window=win;
  element->next  =NULL;
 
  if(last) {
    while(last->next) {
      last=last->next;
    }
    last->next = element;
  } else {
    ggw_list = element;
  }
}

void gemglutwindow::list_del(int win)
{
  t_list*mylist=NULL, *last=NULL;
  
  for(mylist=ggw_list; mylist; mylist=mylist->next) {
    if(mylist->window == win) {
      if(last) {
	last->next=mylist->next;
      } else {
	ggw_list = NULL;
      }
      mylist->window=0;
      mylist->object=NULL;
      mylist->next=NULL;
      delete mylist;
      return;
    }
    last=mylist;
  }
}
 
CPPEXTERN_NEW(gemglutwindow)

/////////////////////////////////////////////////////////
//
// gemglutwindow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemglutwindow :: gemglutwindow(void) :
  m_buffer(2),
  m_fsaa(0),
  m_title((char*)"GEM"),
  m_border(false),
  //  m_width(500), m_height(500),
  m_fullscreen(false),
  m_xoffset(-1), m_yoffset(-1),
  m_cursor(false),
  m_window(0),
  m_clock(NULL),
  m_polltime(5)
{
  m_width =500;
  m_height=500;

  m_clock=clock_new(this, reinterpret_cast<t_method>(gemglutwindow::clockCallback));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemglutwindow :: ~gemglutwindow()
{
  destroyMess();
  clock_free(m_clock);
}


bool gemglutwindow :: makeCurrent(void){
  if(m_window>0) {
    glutSetWindow(m_window);
    return GemContext::makeCurrent();
  }

  return(false);
}

/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void gemglutwindow :: renderMess()
{
  if(!makeCurrent()){ 
    error("no window made, cannot render!");
    return;
  }
  // mark the render-buffer as dirty, so the displayCb() gets called
  // other things that mark dirty are (e.g.) resizing, making (parts of) the window visible,...
  glutPostRedisplay();

  glutMainLoopEvent();
  if(makeCurrent())
    glutSwapBuffers();
}
void gemglutwindow :: doRender()
{
  bang();
}


/////////////////////////////////////////////////////////
// bufferMess
//
/////////////////////////////////////////////////////////
void gemglutwindow :: bufferMess(int buf)
{
  switch(buf) {
  case 1: case 2:
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
// fsaaMess
//
/////////////////////////////////////////////////////////
void gemglutwindow :: fsaaMess(int value)
{
}

/////////////////////////////////////////////////////////
// titleMess
//
/////////////////////////////////////////////////////////
void gemglutwindow :: titleMess(t_symbol* s)
{
  m_title = s->s_name;
  if(makeCurrent()){
    glutSetWindowTitle(m_title);
    glutSetIconTitle(m_title);
  }
}
/////////////////////////////////////////////////////////
// border
//
/////////////////////////////////////////////////////////
void gemglutwindow :: borderMess(bool setting)
{
  m_border=setting;
}
/////////////////////////////////////////////////////////
// dimensionsMess
//
/////////////////////////////////////////////////////////
void gemglutwindow :: dimensionsMess(int width, int height)
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
    glutReshapeWindow(m_width, m_height);
  }
}
/////////////////////////////////////////////////////////
// fullscreenMess
//
/////////////////////////////////////////////////////////
void gemglutwindow :: fullscreenMess(bool on)
{
  m_fullscreen = on;
  if(makeCurrent()){
    if(m_fullscreen)
      glutFullScreen();
    else {
      if(0<m_width&&0<m_height)
        glutReshapeWindow(m_width, m_height);
      if(0<m_xoffset&&0<m_yoffset)
        glutPositionWindow(m_xoffset, m_yoffset);
    }  
  }
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void gemglutwindow :: offsetMess(int x, int y)
{
  m_xoffset = x;
  m_yoffset = y;
  if(makeCurrent()){
    glutPositionWindow(x, y);
  }
}

/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
void gemglutwindow :: createMess(void)
{
  if(m_window) {
    error("window already made!");
    return;
  }
  unsigned int mode=GLUT_RGB | GLUT_DEPTH;
  if(2==m_buffer)
    mode|=GLUT_DOUBLE;
  else
    mode|=GLUT_SINGLE;

  glutInitDisplayMode(mode);

  m_window=glutCreateWindow(m_title);
  list_add(this, m_window);

  glutDisplayFunc   (&gemglutwindow::displayCb);
  glutVisibilityFunc(&gemglutwindow::visibleCb);

  glutCloseFunc     (&gemglutwindow::closeCb);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);


  glutKeyboardFunc(&gemglutwindow::keyboardCb);
  glutSpecialFunc(&gemglutwindow::specialCb);
  glutReshapeFunc(&gemglutwindow::reshapeCb);
  glutMouseFunc(&gemglutwindow::mouseCb);
  glutMotionFunc(&gemglutwindow::motionCb);
  glutPassiveMotionFunc(&gemglutwindow::passivemotionCb);
  glutEntryFunc(&gemglutwindow::entryCb);
  glutKeyboardUpFunc(&gemglutwindow::keyboardupCb);
  glutSpecialUpFunc(&gemglutwindow::specialupCb);
  glutJoystickFunc(&gemglutwindow::joystickCb, 20);

  glutMenuStateFunc(&gemglutwindow::menustateCb);
  glutMenuStatusFunc(&gemglutwindow::menustatusCb);

  glutWindowStatusFunc(&gemglutwindow::windowstatusCb);

  //  glutNameFunc(&gemglutwindow::nameCb);

  if(!create()) {
    destroyMess();
    return;
  }

  titleMess(gensym(m_title));
  fullscreenMess(m_fullscreen);

  glutPostRedisplay();
  glutMainLoopEvent();

  if(m_polltime>0)
    clock_delay(m_clock, m_polltime);

}
/////////////////////////////////////////////////////////
// destroy window
//
/////////////////////////////////////////////////////////
void gemglutwindow :: destroy(void)
{
  GemContext::destroy();
  clock_unset(m_clock);
  m_window=0;
}
void gemglutwindow :: destroyMess(void)
{
  if(makeCurrent()) {
    glutDestroyWindow(m_window);
    glutMainLoopEvent();
    list_del(m_window);
  }
  destroy();
}


/////////////////////////////////////////////////////////
// cursorMess
//
/////////////////////////////////////////////////////////
void gemglutwindow :: cursorMess(bool setting)
{
  m_cursor=setting;
  if(makeCurrent()){
    glutSetCursor(setting?GLUT_CURSOR_INHERIT:GLUT_CURSOR_NONE);
  }
}
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemglutwindow :: obj_setupCallback(t_class *classPtr)
{
  int argc=0;
  char*argv=NULL;

  static bool firsttime=true;

  if(firsttime) {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutInit(&argc,&argv);
  }
  firsttime=false;

  class_addbang(classPtr, reinterpret_cast<t_method>(&gemglutwindow::renderMessCallback));
  
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglutwindow::titleMessCallback),        gensym("title"), A_DEFSYM ,A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglutwindow::createMessCallback),       gensym("create") ,A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglutwindow::bufferMessCallback),       gensym("buffer"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglutwindow::fullscreenMessCallback),   gensym("fullscreen"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglutwindow::dimensionsMessCallback),   gensym("dimen"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglutwindow::offsetMessCallback),       gensym("offset"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglutwindow::cursorMessCallback),       gensym("cursor"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglutwindow::destroyMessCallback),      gensym("destroy"), A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglutwindow::printMessCallback),        gensym("print"), A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglutwindow::borderMessCallback),       gensym("border"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglutwindow::fsaaMessCallback),         gensym("FSAA"), A_FLOAT, A_NULL);
}
void gemglutwindow :: printMessCallback(void *)
{
  //  GemMan::printInfo();
}
void gemglutwindow :: borderMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->borderMess(static_cast<int>(state));
}
void gemglutwindow :: destroyMessCallback(void *data)
{
  GetMyClass(data)->destroyMess();
}
void gemglutwindow :: renderMessCallback(void *data)
{
  GetMyClass(data)->renderMess();
}
void gemglutwindow :: titleMessCallback(void *data, t_symbol* disp)
{
  GetMyClass(data)->titleMess(disp);
}
void gemglutwindow :: createMessCallback(void *data)
{
  GetMyClass(data)->createMess();
}
void gemglutwindow :: bufferMessCallback(void *data, t_floatarg buf)
{
  GetMyClass(data)->bufferMess(static_cast<int>(buf));
}
void gemglutwindow :: fullscreenMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->fullscreenMess(static_cast<int>(on));
}
void gemglutwindow :: dimensionsMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->dimensionsMess(static_cast<int>(width), static_cast<int>(height));
}
void gemglutwindow :: offsetMessCallback(void *data, t_floatarg x, t_floatarg y)
{
  GetMyClass(data)->offsetMess(static_cast<int>(x), static_cast<int>(y));
}
void gemglutwindow :: cursorMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->cursorMess(val);
}
void gemglutwindow :: fsaaMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->fsaaMess(static_cast<int>(val));
}

/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void gemglutwindow :: clock(void)
{
  if(m_window<=0){ 
    return;
  }
  glutSetWindow(m_window);
  
  glutMainLoopEvent();

  if(m_polltime>0)
    clock_delay(m_clock, m_polltime);
}

void gemglutwindow :: clockCallback(void *data)
{
  gemglutwindow*instance=(gemglutwindow*)data;
  instance->clock();
}

#define CALLBACK4WIN gemglutwindow*ggw=list_find(glutGetWindow()); if(!ggw){::error("couldn't find [gemglutwindow] for window#%d", glutGetWindow()); return;} else ggw


void gemglutwindow::displayCb(void) {
  CALLBACK4WIN ->doRender();
}

void gemglutwindow::visibleCb(int state) {
  CALLBACK4WIN->info("visible", state);
}

void gemglutwindow::closeCb(void) {
  CALLBACK4WIN ->destroy();
  ggw->info("window", "closed");
}


static std::string key2symbol(unsigned char c) {
  std::string sym;
  sym+=c;
  return sym;
}

static std::string key2symbol(int c) {
  switch(c) {
  case GLUT_KEY_F1: return std::string("F1");
  case GLUT_KEY_F2: return std::string("F2");
  case GLUT_KEY_F3: return std::string("F3");
  case GLUT_KEY_F4: return std::string("F4");
  case GLUT_KEY_F5: return std::string("F5");
  case GLUT_KEY_F6: return std::string("F6");
  case GLUT_KEY_F7: return std::string("F7");
  case GLUT_KEY_F8: return std::string("F8");
  case GLUT_KEY_F9: return std::string("F9");
  case GLUT_KEY_F10: return std::string("F10");
  case GLUT_KEY_F11: return std::string("F11");
  case GLUT_KEY_F12: return std::string("F12");
  case GLUT_KEY_LEFT: return std::string("Left");
  case GLUT_KEY_UP: return std::string("Up");
  case GLUT_KEY_RIGHT: return std::string("Right");
  case GLUT_KEY_DOWN: return std::string("Down");
  case GLUT_KEY_PAGE_UP: return std::string("PageUp");
  case GLUT_KEY_PAGE_DOWN: return std::string("PageDown");
  case GLUT_KEY_HOME: return std::string("Home");
  case GLUT_KEY_END: return std::string("End");
  case GLUT_KEY_INSERT: return std::string("Insert");
  default:
    break;
  }

  return std::string("<unknown>");
}
void gemglutwindow::keyboardCb(unsigned char c, int x, int y) {
  CALLBACK4WIN->motion(x,y);
  ggw->key(key2symbol(c), c, 1);
}
void gemglutwindow::keyboardupCb(unsigned char c, int x, int y) {
  CALLBACK4WIN->motion(x,y);
  ggw->key(key2symbol(c), c, 0);
}

void gemglutwindow::specialCb(int c, int x, int y) {
  CALLBACK4WIN->motion(x,y);
  ggw->key(key2symbol(c), c, 1);
}

void gemglutwindow::specialupCb(int c, int x, int y) {
  CALLBACK4WIN->motion(x,y);
  ggw->key(key2symbol(c), c, 0);
}

void gemglutwindow::reshapeCb(int x, int y) {
  t_atom ap[2];
  SETFLOAT (ap+0, x);
  SETFLOAT (ap+1, y);

  CALLBACK4WIN->info(gensym("dimen"), 2, ap);
}
void gemglutwindow::mouseCb(int button, int state, int x, int y) {
  CALLBACK4WIN->motion(x,y);
  ggw->button(button, !state);
}
void gemglutwindow::motionCb(int x, int y) {
  CALLBACK4WIN->motion(x,y);
}
void gemglutwindow::passivemotionCb(int x, int y) {
  CALLBACK4WIN->motion(x,y);
}

void gemglutwindow::entryCb(int state) {
  CALLBACK4WIN->info("entry", state);
}
void gemglutwindow::joystickCb(unsigned int a, int x, int y, int z) {
}
void gemglutwindow::menustateCb(int value) {
}
void gemglutwindow::menustatusCb(int x, int y, int z) {
}
void gemglutwindow::windowstatusCb(int value) {
  std::string s;

  switch(value) {
  case GLUT_HIDDEN: s=std::string("hidden"); break;
  case GLUT_FULLY_RETAINED: s=std::string("full"); break;
  case GLUT_PARTIALLY_RETAINED: s=std::string("partial"); break;
  case GLUT_FULLY_COVERED: s=std::string("covered"); break;
  default:
    s=std::string("unknown");
  }

  CALLBACK4WIN->info("window", s);
}
