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
#include "Base/config.h"


#include "gemwindow.h"

// I hate Microsoft...I shouldn't have to do this!
#if defined _WINDOWS
# include <stdlib.h>
# include <windows.h>
#elif defined __APPLE__
# include <stdlib.h>
// or is it spelled Carbon/Carbon.h ??
# include <Carbon/carbon.h>
# include <OpenGL/gl.h>
# include <OpenGL/glext.h>
#elif defined __unix__
# include <GL/gl.h>
# include <GL/glx.h>
# include <X11/Xlib.h>
#endif // OS

#include "Base/GemMan.h"
#include "Base/GemWinCreate.h"


/* some statics variables */

/*
 * s_windowRun indicates on windos that we have a "running" window and want to dispatchGemWindowMessages
 */
static int s_windowRun = 0; // LATER think about making this part of the class!!

/*
 * on windows there might be an option that we only want singleContext (on some stone-age hardware)
 * we leave it here for now
 */
static int s_singleContext = 0; // LATER think about removing

//static  WindowInfo gfxInfo;
static  WindowInfo constInfo;
    

CPPEXTERN_NEW(gemwindow)

/////////////////////////////////////////////////////////
//
// gemwindow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemwindow :: gemwindow() :
  m_fullscreen(0), m_width(500), m_height(500), m_xoffset(0), m_yoffset(0), m_border(1),
  m_cursor(1), m_topmost(0),
  m_windowState(0), m_windowContext(0)
{
  m_title="GEM";
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemwindow :: ~gemwindow()
{
  destroyMess();
}






/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////

void gemwindow :: createContext(char* disp)
{
#ifdef unix
  Display *dummyDpy;
  if ( (dummyDpy = XOpenDisplay(disp)) == NULL)
    { 
      error("GEM: could not open X display %s",disp);
      return;
    }
    
  int dummy, dummy2;
  if ( !glXQueryExtension(dummyDpy, &dummy, &dummy2) )
    {
      error("GEM: X server has no OpenGL GLX extension");
      XCloseDisplay(dummyDpy);
      return;
    }
#elif _WINDOWS
  // can we only have one context?
  if (getenv("GEM_SINGLE_CONTEXT") &&
      !strcmp("1", getenv("GEM_SINGLE_CONTEXT")))
    {
      post("GEM: using GEM_SINGLE_CONTEXT");
      s_singleContext = 1;
      m_width = 640;
      m_height = 480;
    }
#elif __APPLE__
    // check existence of OpenGL libraries
    if ((Ptr)kUnresolvedCFragSymbolAddress == (Ptr)aglChoosePixelFormat) {
            error ("GEM : OpenGL is not installed");
            return;
    }
#endif
    //s_windowClock = clock_new(NULL, (t_method)dispatchGemWindowMessages);

    if (!createConstWindow(disp))  {
      error("GEM: A serious error occured creating const Context");
      error("GEM: Continue at your own risk!");
      m_windowContext = 0;
    } else 
      m_windowContext = 1;
    //setResizeCallback(resizeCallback, NULL);
}

int gemwindow :: createConstWindow(char* disp) {
#ifdef _WINDOWS
  // can we only have one context?
  if (s_singleContext) {
    return(createWindow(disp));
  }
#endif

  WindowHints myHints;
  myHints.title    = m_title;
  myHints.border   = 1;
  myHints.buffer   = 1;
  myHints.x_offset = 0;
  myHints.y_offset = 0;
  myHints.width    = m_width;
  myHints.height   = m_height;
#ifdef __APPLE__
  myHints.shared   = constInfo.context;
#else
  myHints.shared   = NULL;
#endif
  myHints.actuallyDisplay = 0;
  myHints.fullscreen      = 0;
  myHints.display         = disp;

  if (!createGemWindow(constInfo, myHints) )  {
    error("GEM: Error creating const context");
    constInfo.have_constContext=0;
    gfxInfo.have_constContext=0;
    return(0);
  } else{
    constInfo.have_constContext=1;
    gfxInfo.have_constContext=1;
  }

  return(1);
}


int gemwindow :: createWindow(char* disp) {
  if ( m_windowState ) return(0);
#ifdef DEBUG
  post("gemwindow: create window");
#endif

  WindowHints myHints;
  myHints.border = m_border;
  myHints.buffer = m_buffer;
  myHints.width = m_width;
  myHints.height = m_height;
  myHints.fullscreen = m_fullscreen;
  myHints.x_offset = m_xoffset;
  myHints.y_offset = m_yoffset;
  myHints.shared = constInfo.context;
  myHints.actuallyDisplay = 1;
  myHints.display = disp;
  myHints.title = m_title;
  myHints.fsaa = m_fsaa;
  
  if (disp) post("GEM: creating gem-window on display %s",disp);
  if (!createGemWindow(gfxInfo, myHints) )  {
    error("GEM: Unable to create window");
    return(0);
  }

  m_w=myHints.real_w;
  m_h=myHints.real_h;

  m_windowState = 1;
  cursorMess(m_cursor);
  topmostMess(m_topmost);
  windowInit(); // IMPORTANT
  //  clock_delay(s_windowClock, s_windowDelTime);

  s_windowRun = 1;
  return(1);
}

/////////////////////////////////////////////////////////
// windowInit
//
/////////////////////////////////////////////////////////
void gemwindow :: windowInit()
{
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_BLEND);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.0);    
  glClearColor(0, 0, 0, 0);//m_clear_color[0], m_clear_color[1], m_clear_color[2], m_clear_color[3]);
 
  #ifdef __APPLE__
  GLint swapInt = 1;
  aglSetInteger ( gfxInfo.context, AGL_SWAP_INTERVAL, &swapInt);
  #endif

  /* i am not really sure whether it is a good idea to enable FSAA by default
   * this might slow down everything a lot
   */

#if defined GL_MULTISAMPLE_ARB && defined GL_MULTISAMPLE_FILTER_HINT_NV
  glEnable (GL_MULTISAMPLE_ARB);
  glHint (GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
#endif

  //resetValues();
}


void gemwindow :: createMess(t_symbol* s)
{
  char* disp = (s==&s_)?NULL:s->s_name;

  createContext(disp);
  if ( !createWindow(disp) )  {
    error("GEM: gemwindow: no window made");
    return;
  }
  //GemMan::swapBuffers();
  //GemMan::swapBuffers();
}
/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
void gemwindow :: destroyMess()
{
#ifdef _WINDOWS
  // don't want to get rid of this
  if (s_singleContext)
    return;
#endif

  if (!m_windowState) return;

  //clock_unset(s_windowClock);
  //s_windowClock = NULL;

  glFlush();
  glFinish();

  destroyGemWindow(gfxInfo);

  m_windowState = 0;
    
  // reestablish the const glxContext
#ifdef unix                 // for Unix
  //post("dpy=%x\twin=%x\tcontext=%x", constInfo.dpy, constInfo.win, constInfo.context);

  if (!constInfo.dpy && !constInfo.win && !constInfo.context)return; // do not crash

  glXMakeCurrent(constInfo.dpy, constInfo.win, constInfo.context);   
#elif _WINDOWS              // for Windows

  if (!constInfo.dc && !constInfo.context)return; // do not crash ??

  wglMakeCurrent(constInfo.dc, constInfo.context);
  s_windowRun = 0;
#elif __APPLE__		// for PPC Macintosh
    ::aglSetDrawable( constInfo.context, GetWindowPort(constInfo.pWind) );
    ::aglSetCurrentContext(constInfo.context);
#else
#error Define OS specific OpenGL context make current
#endif
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
// fullscreenMess
//
/////////////////////////////////////////////////////////
void gemwindow :: fullscreenMess(int on)
{
  m_fullscreen = on;
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
// borderMess
//
/////////////////////////////////////////////////////////
void gemwindow :: borderMess(bool on)
{
  m_border = on;
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
// cursorMess
//
/////////////////////////////////////////////////////////
void gemwindow :: cursorMess(bool on)
{
  if (m_windowState)
    cursorGemWindow(gfxInfo, (int)on);
}

/////////////////////////////////////////////////////////
// topmostMess
//
/////////////////////////////////////////////////////////
void gemwindow :: topmostMess(bool on)
{
  if (m_windowState)
    topmostGemWindow(gfxInfo, (int)on);
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
void gemwindow :: destroyMessCallback(void *data)
{
  GetMyClass(data)->destroyMess();
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
