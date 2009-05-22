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

#include "GL/glut.h"


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
  m_width(500), m_height(500),
  m_fullscreen(false),
  m_xoffset(-1), m_yoffset(-1),
  m_cursor(false)
{
  m_infoOut = outlet_new(this->x_obj, 0);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemglutwindow :: ~gemglutwindow()
{
  destroyMess();
  outlet_free(m_infoOut); m_infoOut=NULL;
}

/////////////////////////////////////////////////////////
// bangMess
//
/////////////////////////////////////////////////////////
void gemglutwindow :: bangMess()
{
  if(!m_window) {
    error("no window made, cannot render!");
    return;
  }
  glutSetWindow(m_window);
  glutPostRedisplay();
  glutSwapBuffers();
}

/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void gemglutwindow :: renderMess()
{
  if(!m_window) {
    error("no window made, cannot render!");
    return;
  }
  glutSetWindow(m_window);
  glutPostRedisplay();
  glutMainLoopEvent();
}
void gemglutwindow :: doRender()
{
  glutSetWindow(m_window);
  outlet_bang(m_infoOut);
  glutSwapBuffers();  
}

/////////////////////////////////////////////////////////
// bufferMess
//
/////////////////////////////////////////////////////////
void gemglutwindow :: bufferMess(int buf)
{
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
  if(m_window) {
    glutSetWindow(m_window);
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
  if(m_window) {
    glutSetWindow(m_window);
    glutPositionWindow(m_width, m_height);
  }
}
/////////////////////////////////////////////////////////
// fullscreenMess
//
/////////////////////////////////////////////////////////
void gemglutwindow :: fullscreenMess(bool on)
{
  m_fullscreen = on;
  if(m_window) {
    glutSetWindow(m_window);
    if(m_fullscreen)
      glutFullScreen();
    else
      glutPositionWindow(m_xoffset, m_yoffset);
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
  if(m_window) {
    glutSetWindow(m_window);
    glutPositionWindow(x, y);
  }
}

/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
void gemglutwindow :: createMess(void)
{
  m_window=glutCreateWindow(m_title);
  list_add(this, m_window);
  glutDisplayFunc   (&gemglutwindow::displayCb);
  glutVisibilityFunc(&gemglutwindow::visibleCb);
}
/////////////////////////////////////////////////////////
// destroy window
//
/////////////////////////////////////////////////////////
void gemglutwindow :: destroyMess(void)
{
  if(m_window) {
    list_del(m_window);
    glutDestroyWindow(m_window);
  }
  m_window=0;
}


/////////////////////////////////////////////////////////
// cursorMess
//
/////////////////////////////////////////////////////////
void gemglutwindow :: cursorMess(bool setting)
{
  m_cursor=setting;
  if(m_window) {
    glutSetWindow(m_window);
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

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500,500);
  glutInit(&argc,&argv);

  class_addbang(classPtr, (t_method)&gemglutwindow::bangMessCallback);
  class_addmethod(classPtr, (t_method)&gemglutwindow::renderMessCallback, gensym("render"), A_NULL);

  class_addmethod(classPtr, (t_method)&gemglutwindow::titleMessCallback,        gensym("title"), A_DEFSYM ,A_NULL);
  class_addmethod(classPtr, (t_method)&gemglutwindow::createMessCallback,       gensym("create") ,A_NULL);
  class_addmethod(classPtr, (t_method)&gemglutwindow::bufferMessCallback,       gensym("buffer"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemglutwindow::fullscreenMessCallback,   gensym("fullscreen"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemglutwindow::dimensionsMessCallback,   gensym("dimen"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemglutwindow::offsetMessCallback,       gensym("offset"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemglutwindow::cursorMessCallback,       gensym("cursor"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemglutwindow::destroyMessCallback,      gensym("destroy"), A_NULL);
  class_addmethod(classPtr, (t_method)&gemglutwindow::printMessCallback,        gensym("print"), A_NULL);
  class_addmethod(classPtr, (t_method)&gemglutwindow::borderMessCallback,       gensym("border"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemglutwindow::fsaaMessCallback,         gensym("FSAA"), A_FLOAT, A_NULL);
}
void gemglutwindow :: printMessCallback(void *)
{
  //  GemMan::printInfo();
}
void gemglutwindow :: borderMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->borderMess((int)state);
}
void gemglutwindow :: destroyMessCallback(void *data)
{
  GetMyClass(data)->destroyMess();
}
void gemglutwindow :: bangMessCallback(void *data)
{
  GetMyClass(data)->bangMess();
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
  GetMyClass(data)->bufferMess((int)buf);
}
void gemglutwindow :: fullscreenMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->fullscreenMess((int)on);
}
void gemglutwindow :: dimensionsMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->dimensionsMess((int)width, (int)height);
}
void gemglutwindow :: offsetMessCallback(void *data, t_floatarg x, t_floatarg y)
{
  GetMyClass(data)->offsetMess((int)x, (int)y);
}
void gemglutwindow :: cursorMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->cursorMess((float)val);
}
void gemglutwindow :: fsaaMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->fsaaMess((int) val);
}


void gemglutwindow::displayCb(void) {
  int window = glutGetWindow();
  if(window) {
    // now we have to find out which [gemglutwindow] belongs to this ID
    gemglutwindow*ggw=NULL;
    ggw=list_find(window);
    if(ggw) {
      ggw->doRender();
    } else {
      ::error("couldn't find [gemglutwindow] for window#%d", window);
    }
  }
}


void gemglutwindow::visibleCb(int state) {
  ::post("visible %d", state);
}

