////////////////////////////////////////////////////////
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

#include "gemwindow.h"

// I hate Microsoft...I shouldn't have to do this!
#ifdef _WINDOWS
#include <windows.h>
#endif
 
#ifdef __APPLE__
#include <Carbon/carbon.h>
#include <OpenGL/gl.h>
#include "macosx_math.h"
#else
#include <GL/gl.h>
#include <math.h>
#endif // __APPLE__

#include "Base/GemMan.h"

CPPEXTERN_NEW(gemwindow)

/////////////////////////////////////////////////////////
//
// gemwindow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemwindow :: gemwindow()
{}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemwindow :: ~gemwindow()
{
  destroyMess();
}

/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void gemwindow :: preRender(gem_control gc)
{
}

void gemwindow :: postRender(gem_control gc)
{
}

/////////////////////////////////////////////////////////
// titleMess
//
/////////////////////////////////////////////////////////
void gemwindow :: titleMess(t_symbol* s)
{
  m_title = s->s_name;
}
/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
void gemwindow :: createMess(t_symbol* s)
{
  char* disp = NULL;

  if (s != &s_) 
    disp = s->s_name;
#if 0
  GemMan::createContext(disp);
  if ( !GemMan::createWindow(disp) )  {
    error("GEM: gemwindow: no window made");
    return;
  }
  GemMan::swapBuffers();
  GemMan::swapBuffers();
#endif
}
/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
void gemwindow :: destroyMess()
{
}
/////////////////////////////////////////////////////////
// fullscreenMess
//
/////////////////////////////////////////////////////////
void gemwindow :: fullscreenMess(int on)
{
  m_fullscreen = on;
}

/////////////////////////////////////////////////////////
// dimensionsMess
//
/////////////////////////////////////////////////////////
void gemwindow :: dimensionsMess(int width, int height)
{
  if (width <= 0) {
    error("GEM: gemwindow: width must be greater than 0");
    return;
  }
    
  if (height <= 0 ) {
    error ("GEM: gemwindow: height must be greater than 0");
    return;
  }
  m_width  = width;
  m_height = height;
}
/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void gemwindow :: offsetMess(int x, int y)
{
  m_xoffset = x;
  m_yoffset = y;
}

/////////////////////////////////////////////////////////
// cursorMess
//
/////////////////////////////////////////////////////////
void gemwindow :: cursorMess(bool on)
{
  //  GemMan :: cursorOnOff((int)setting);
}

/////////////////////////////////////////////////////////
// topmostMess
//
/////////////////////////////////////////////////////////
void gemwindow :: topmostMess(bool on)
{
  GemMan::topmostOnOff((int) on);
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemwindow :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&gemwindow::createMessCallback,
		  gensym("create"), A_DEFSYM ,A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::destroyMessCallback,
		  gensym("destroy"), A_NULL);

  class_addmethod(classPtr, (t_method)&gemwindow::fullscreenMessCallback,
		  gensym("fullscreen"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::dimensionsMessCallback,
		  gensym("dimen"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::offsetMessCallback,
		  gensym("offset"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::borderMessCallback, 
		  gensym("border"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::titleMessCallback,
		  gensym("title"), A_DEFSYM ,A_NULL);

  class_addmethod(classPtr, (t_method)&gemwindow::cursorMessCallback,
		  gensym("cursor"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::topmostMessCallback,
		  gensym("topmost"), A_FLOAT, A_NULL);

}

void gemwindow :: createMessCallback(void *data, t_symbol* disp)
{
  GetMyClass(data)->createMess(disp);
}
void gemwindow :: destroyMessCallback(void *)
{
  GemMan::destroyWindow();
}


void gemwindow :: dimensionsMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->dimensionsMess((int)width, (int)height);
}
void gemwindow :: offsetMessCallback(void *data, t_floatarg x, t_floatarg y)
{
  GetMyClass(data)->offsetMess((int)x, (int)y);
}
void gemwindow :: fullscreenMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->fullscreenMess((int)on);
}
void gemwindow :: borderMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->borderMess((int)on);
}
void gemwindow :: titleMessCallback(void *data, t_symbol* disp)
{
  GetMyClass(data)->titleMess(disp);
}

void gemwindow :: cursorMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->cursorMess((int)val);
}
void gemwindow :: topmostMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->topmostMess((int)val);
}
