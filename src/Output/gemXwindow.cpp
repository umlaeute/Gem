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

#include "gemXwindow.h"

#include "Base/GemGL.h"
#include "GL/freeglut.h"

#include <stdio.h>

gemXwindow::t_list *gemXwindow::ggw_list = NULL;

gemXwindow* gemXwindow::list_find(int win)
{
  t_list*mylist=0;

  for(mylist=ggw_list; mylist; mylist=mylist->next) {
    if(mylist->window == win)
      return mylist->object;
  }
  /* not found */
  return 0;
}
void gemXwindow::list_add(gemXwindow*obj, int win)
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

void gemXwindow::list_del(int win)
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
 
CPPEXTERN_NEW(gemXwindow)

/////////////////////////////////////////////////////////
//
// gemXwindow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemXwindow :: gemXwindow(void) :
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

  m_clock=clock_new(this, reinterpret_cast<t_method>(gemXwindow::clockCallback));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemXwindow :: ~gemXwindow()
{
  destroyMess();
  clock_free(m_clock);
}


bool gemXwindow :: makeCurrent(void){
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
void gemXwindow :: renderMess()
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
void gemXwindow :: doRender()
{
  bang();
}


/////////////////////////////////////////////////////////
// bufferMess
//
/////////////////////////////////////////////////////////
void gemXwindow :: bufferMess(int buf)
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
void gemXwindow :: fsaaMess(int value)
{
}

/////////////////////////////////////////////////////////
// titleMess
//
/////////////////////////////////////////////////////////
void gemXwindow :: titleMess(t_symbol* s)
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
void gemXwindow :: borderMess(bool setting)
{
  m_border=setting;
}
/////////////////////////////////////////////////////////
// dimensionsMess
//
/////////////////////////////////////////////////////////
void gemXwindow :: dimensionsMess(int width, int height)
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
void gemXwindow :: fullscreenMess(bool on)
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
void gemXwindow :: offsetMess(int x, int y)
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
void gemXwindow :: createMess(void)
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

  glutDisplayFunc   (&gemXwindow::displayCb);
  glutVisibilityFunc(&gemXwindow::visibleCb);

  glutCloseFunc     (&gemXwindow::closeCb);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);


  glutKeyboardFunc(&gemXwindow::keyboardCb);
  glutSpecialFunc(&gemXwindow::specialCb);
  glutReshapeFunc(&gemXwindow::reshapeCb);
  glutMouseFunc(&gemXwindow::mouseCb);
  glutMotionFunc(&gemXwindow::motionCb);
  glutPassiveMotionFunc(&gemXwindow::passivemotionCb);
  glutEntryFunc(&gemXwindow::entryCb);
  glutKeyboardUpFunc(&gemXwindow::keyboardupCb);
  glutSpecialUpFunc(&gemXwindow::specialupCb);
  glutJoystickFunc(&gemXwindow::joystickCb, 20);

  glutMenuStateFunc(&gemXwindow::menustateCb);
  glutMenuStatusFunc(&gemXwindow::menustatusCb);

  glutWindowStatusFunc(&gemXwindow::windowstatusCb);

  //  glutNameFunc(&gemXwindow::nameCb);

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
void gemXwindow :: destroy(void)
{
  GemContext::destroy();
  clock_unset(m_clock);
  m_window=0;
}
void gemXwindow :: destroyMess(void)
{
  if(makeCurrent()) {
    glutDestroyWindow(m_window);
    glutMainLoopEvent();
    list_del(m_window);
  } else {
    error("unable to destroy current window");
  }
  destroy();
}


/////////////////////////////////////////////////////////
// cursorMess
//
/////////////////////////////////////////////////////////
void gemXwindow :: cursorMess(bool setting)
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
void gemXwindow :: obj_setupCallback(t_class *classPtr)
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

  class_addbang(classPtr, reinterpret_cast<t_method>(&gemXwindow::renderMessCallback));
  
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemXwindow::titleMessCallback),        gensym("title"), A_DEFSYM ,A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemXwindow::createMessCallback),       gensym("create") ,A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemXwindow::bufferMessCallback),       gensym("buffer"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemXwindow::fullscreenMessCallback),   gensym("fullscreen"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemXwindow::dimensionsMessCallback),   gensym("dimen"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemXwindow::offsetMessCallback),       gensym("offset"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemXwindow::cursorMessCallback),       gensym("cursor"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemXwindow::destroyMessCallback),      gensym("destroy"), A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemXwindow::printMessCallback),        gensym("print"), A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemXwindow::borderMessCallback),       gensym("border"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemXwindow::fsaaMessCallback),         gensym("FSAA"), A_FLOAT, A_NULL);
}
void gemXwindow :: printMessCallback(void *)
{
  //  GemMan::printInfo();
}
void gemXwindow :: borderMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->borderMess(static_cast<int>(state));
}
void gemXwindow :: destroyMessCallback(void *data)
{
  GetMyClass(data)->destroyMess();
}
void gemXwindow :: renderMessCallback(void *data)
{
  GetMyClass(data)->renderMess();
}
void gemXwindow :: titleMessCallback(void *data, t_symbol* disp)
{
  GetMyClass(data)->titleMess(disp);
}
void gemXwindow :: createMessCallback(void *data)
{
  GetMyClass(data)->createMess();
}
void gemXwindow :: bufferMessCallback(void *data, t_floatarg buf)
{
  GetMyClass(data)->bufferMess(static_cast<int>(buf));
}
void gemXwindow :: fullscreenMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->fullscreenMess(static_cast<int>(on));
}
void gemXwindow :: dimensionsMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->dimensionsMess(static_cast<int>(width), static_cast<int>(height));
}
void gemXwindow :: offsetMessCallback(void *data, t_floatarg x, t_floatarg y)
{
  GetMyClass(data)->offsetMess(static_cast<int>(x), static_cast<int>(y));
}
void gemXwindow :: cursorMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->cursorMess(val);
}
void gemXwindow :: fsaaMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->fsaaMess(static_cast<int>(val));
}

/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void gemXwindow :: clock(void)
{
  if(m_window<=0){ 
    return;
  }
  glutSetWindow(m_window);
  
  glutMainLoopEvent();

  if(m_polltime>0)
    clock_delay(m_clock, m_polltime);
}

void gemXwindow :: clockCallback(void *data)
{
  gemXwindow*instance=(gemXwindow*)data;
  instance->clock();
}

#define CALLBACK4WIN gemXwindow*ggw=list_find(glutGetWindow()); if(!ggw){::error("couldn't find [gemXwindow] for window#%d", glutGetWindow()); return;} else ggw


void gemXwindow::displayCb(void) {
  CALLBACK4WIN ->doRender();
}

void gemXwindow::visibleCb(int state) {
  CALLBACK4WIN->info(gensym("visible"), state);
}

void gemXwindow::closeCb(void) {
  CALLBACK4WIN ->destroy();
  ggw->info(gensym("window"), gensym("closed"));
}


static t_symbol*key2symbol(unsigned char c) {
  t_symbol*sym=NULL;
  char s[2];
  switch(c) {
  default:
    sprintf(s, "%c", c);
    s[1]=0;
    sym=gensym(s);
  }
  return sym;
}

static t_symbol*key2symbol(int c) {
  t_symbol*s=NULL;

  switch(c) {
  case GLUT_KEY_F1: s=gensym("F1"); break;
  case GLUT_KEY_F2: s=gensym("F2"); break;
  case GLUT_KEY_F3: s=gensym("F3"); break;
  case GLUT_KEY_F4: s=gensym("F4"); break;
  case GLUT_KEY_F5: s=gensym("F5"); break;
  case GLUT_KEY_F6: s=gensym("F6"); break;
  case GLUT_KEY_F7: s=gensym("F7"); break;
  case GLUT_KEY_F8: s=gensym("F8"); break;
  case GLUT_KEY_F9: s=gensym("F9"); break;
  case GLUT_KEY_F10: s=gensym("F10"); break;
  case GLUT_KEY_F11: s=gensym("F11"); break;
  case GLUT_KEY_F12: s=gensym("F12"); break;
  case GLUT_KEY_LEFT: s=gensym("Left"); break;
  case GLUT_KEY_UP: s=gensym("Up"); break;
  case GLUT_KEY_RIGHT: s=gensym("Right"); break;
  case GLUT_KEY_DOWN: s=gensym("Down"); break;
  case GLUT_KEY_PAGE_UP: s=gensym("PageUp"); break;
  case GLUT_KEY_PAGE_DOWN: s=gensym("PageDown"); break;
  case GLUT_KEY_HOME: s=gensym("Home"); break;
  case GLUT_KEY_END: s=gensym("End"); break;
  case GLUT_KEY_INSERT: s=gensym("Insert"); break;
  default:
    s=gensym("unknown");
  }

  return s;
}
void gemXwindow::keyboardCb(unsigned char c, int x, int y) {
  CALLBACK4WIN->motion(x,y);
  ggw->key(key2symbol(c), 1);
}
void gemXwindow::keyboardupCb(unsigned char c, int x, int y) {
  CALLBACK4WIN->motion(x,y);
  ggw->key(key2symbol(c), 0);
}

void gemXwindow::specialCb(int c, int x, int y) {
  CALLBACK4WIN->motion(x,y);
  ggw->key(key2symbol(c), 1);
}

void gemXwindow::specialupCb(int c, int x, int y) {
  CALLBACK4WIN->motion(x,y);
  ggw->key(key2symbol(c), 0);
}

void gemXwindow::reshapeCb(int x, int y) {
  t_atom ap[2];
  SETFLOAT (ap+0, x);
  SETFLOAT (ap+1, y);

  CALLBACK4WIN->info(gensym("dimen"), 2, ap);
}
void gemXwindow::mouseCb(int button, int state, int x, int y) {
  CALLBACK4WIN->motion(x,y);
  ggw->button(button, !state);
}
void gemXwindow::motionCb(int x, int y) {
  CALLBACK4WIN->motion(x,y);
}
void gemXwindow::passivemotionCb(int x, int y) {
  CALLBACK4WIN->motion(x,y);
}

void gemXwindow::entryCb(int state) {
  CALLBACK4WIN->info(gensym("entry"), state);
}
void gemXwindow::joystickCb(unsigned int a, int x, int y, int z) {
}
void gemXwindow::menustateCb(int value) {
}
void gemXwindow::menustatusCb(int x, int y, int z) {
}
void gemXwindow::windowstatusCb(int value) {
  t_symbol*s=NULL;

  switch(value) {
  case GLUT_HIDDEN: s=gensym("hidden"); break;
  case GLUT_FULLY_RETAINED: s=gensym("full"); break;
  case GLUT_PARTIALLY_RETAINED: s=gensym("partial"); break;
  case GLUT_FULLY_COVERED: s=gensym("covered"); break;
  default:
    s=gensym("unknown");
  }

  CALLBACK4WIN->info(gensym("window"), s);
}
