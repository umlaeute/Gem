////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
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
#include <math.h>
#elif defined __APPLE__
# include <stdlib.h>
// or is it spelled Carbon/Carbon.h ??
# include <Carbon/carbon.h>
# include <OpenGL/gl.h>
# include <OpenGL/glext.h>
#include "macosx_math.h"
#elif defined __unix__
# include <GL/gl.h>
# include <GL/glx.h>
# include <X11/Xlib.h>
#include <math.h>
#endif // OS

#include "Base/GemMan.h"
#include "Base/GemWinCreate.h"
#include "Base/GemEvent.h"


/* some statics variables */

/*
 * on windows there might be an option that we only want singleContext (on some stone-age hardware)
 * we leave it here for now
 */
static int s_singleContext = 0; // LATER think about removing
    

////////////////////////
// makeCurrent
static void makeCurrent(WindowInfo wi){
#ifdef unix                 // for Unix
  if (!wi.dpy && !wi.win && !wi.context)return; // do not crash

  glXMakeCurrent(wi.dpy, wi.win, wi.context);   
#elif _WINDOWS              // for Windows

  if (!wi.dc && !wi.context)return; // do not crash ??

  wglMakeCurrent(wi.dc, wi.context);
  m_windowRun = 0;
#elif __APPLE__		// for PPC Macintosh
    ::aglSetDrawable( wi.context, GetWindowPort(wi.pWind) );
    ::aglSetCurrentContext(wi.context);
#else
#error Define OS specific OpenGL context make current
#endif
}

CPPEXTERN_NEW(gemwindow)

/////////////////////////////////////////////////////////
//
// gemwindow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemwindow :: gemwindow() : GemOutput(), 
			   m_width(500), m_height(500),
			   m_fullscreen(0), m_xoffset(0), m_yoffset(0), m_border(1),
			   m_cursor(1), m_topmost(0)
{

#ifdef _WINDOWS
  // can we only have one context?
  if (getenv("GEM_SINGLE_CONTEXT") &&
      !strcmp("1", getenv("GEM_SINGLE_CONTEXT")))
    {
      post("GEM: using GEM_SINGLE_CONTEXT");
      s_singleContext = 1;
      m_width = 640;
      m_height = 480;
    }
#endif

  GemOutput::m_width =m_width;
  GemOutput::m_height=m_height;

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


/////////////////////////////////////////////////////////
// dispatchGemWindowMessages
//
/////////////////////////////////////////////////////////
#ifdef _WINDOWS
static void dispatchGemWindowMessages()
{
  if (!s_windowRun)
    return;

  MSG msg;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == TRUE)
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  clock_delay(s_windowClock, s_windowDelTime);
}
#elif unix 
static void dispatchGemWindowMessages()
{
  WindowInfo win; 
  XEvent event; 
  XButtonEvent* eb = (XButtonEvent*)&event; 
  XKeyEvent* kb  = (XKeyEvent*)&event; 
  XResizeRequestEvent *res = (XResizeRequestEvent*)&event;
  //win = GemMan::getWindowInfo(); 

  while (XCheckWindowEvent(win.dpy,win.win,
			   ResizeRedirectMask | 
			   KeyPressMask | KeyReleaseMask |
			   PointerMotionMask | 
			   ButtonMotionMask |
			   ButtonPressMask | 
			   ButtonReleaseMask,
			   &event))
    {
      switch (event.type)
	{
	case ButtonPress: 
	  triggerButtonEvent(eb->button-1, 1, eb->x, eb->y); 
	  break; 
	case ButtonRelease: 
	  triggerButtonEvent(eb->button-1, 0, eb->x, eb->y); 
	  break; 
	case MotionNotify: 
	  triggerMotionEvent(eb->x, eb->y); 
	  break; 
	case KeyPress:
	  triggerKeyboardEvent(XKeysymToString(XKeycodeToKeysym(win.dpy, kb->keycode, 0)), kb->keycode, 1);
	  break;
	case KeyRelease:
	  triggerKeyboardEvent(XKeysymToString(XKeycodeToKeysym(win.dpy, kb->keycode, 0)), kb->keycode, 0);
	  break;
	case ResizeRequest:
	  triggerResizeEvent(res->width, res->height);
	  XResizeWindow(win.dpy, win.win, res->width, res->height);
	  break;
	default:
	  break; 
	}
    }
  //  clock_delay(s_windowClock, s_windowDelTime);
} 
#elif __APPLE__
static pascal OSStatus dispatchGemWindowMessages()
{
    EventRef	theEvent;
    EventTargetRef theTarget;
    
    theTarget = GetEventDispatcherTarget();
    ReceiveNextEvent( 0, NULL, kEventDurationNoWait, true,
                                &theEvent );
    {
        SendEventToEventTarget( theEvent, theTarget);
        ReleaseEvent( theEvent );
    }
    clock_delay(s_windowClock, s_windowDelTime);
    return noErr;
}
#endif // for Unix

#define RESIZE_CB_STATIC
#ifndef RESIZE_CB_STATIC
void gemwindow::resizeCallback(int xSize, int ySize, void *)
#else
static void resizeCallbackStatic(int xSize, int ySize, void *)
#endif
{
  /*
  if (ySize==0)ySize=1;

  float xDivy = (float)xSize / (float)ySize;
  m_height = ySize;
  m_width = xSize;
  m_h = m_height;
  m_w = m_width;

  // setup the viewpoint
  glViewport(0, 0, m_width, m_height);
  // setup the matrices
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(m_perspect[0] * xDivy, m_perspect[1] * xDivy,	// left, right
	    m_perspect[2], m_perspect[3],			// bottom, top
	    m_perspect[4], m_perspect[5]);			// front, back

  glMatrixMode(GL_MODELVIEW);
  */
}

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
      m_outputContext = false;
    } else 
      m_outputContext = true;
#ifdef RESIZE_CB_STATIC
    setResizeCallback(resizeCallbackStatic, NULL);
#else
    setResizeCallback((this->resizeCallback), NULL);
#endif
}

int gemwindow :: createConstWindow(char* disp) {
  // can we only have one context?
  if (s_singleContext) {
    return(createWindow(disp));
  }

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
  if ( m_outputState ) return(0);
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

  GemOutput::m_width =myHints.real_w;
  GemOutput::m_height=myHints.real_h;

  m_outputState = 1;
  cursorMess(m_cursor);
  topmostMess(m_topmost);
  windowInit();
  //  clock_delay(s_windowClock, s_windowDelTime);

  m_windowRun = 1;
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
  glClearColor(m_clear_color[0], m_clear_color[1], m_clear_color[2], m_clear_color[3]);
 
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

  resetState();
}



/////////////////////////////////////////////////////////
// swapBuffers
//
/////////////////////////////////////////////////////////
void gemwindow :: swapBuffers()
{
  if (!m_outputState) return;
  if (m_buffer == 2)
#ifdef unix             // for Unix
    glXSwapBuffers(gfxInfo.dpy, gfxInfo.win);
#elif _WINDOWS          // for WinNT
  SwapBuffers(gfxInfo.dc);
#elif __APPLE__		// for Macintosh
  ::aglSwapBuffers(gfxInfo.context);
#else                   // everyone else
#error Define OS specific swap buffer
#endif
  else glFlush();

  glClear(m_clear_mask);
  glColor3f(1.0, 1.0, 1.0);
  glLoadIdentity();

  if (m_buffer == 1)
    {
      glFlush();
      // setup the transformation matrices
      float xDivy = (float)GemOutput::m_width / (float)GemOutput::m_height;

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glFrustum(m_perspect[0] * xDivy, m_perspect[1] * xDivy,	// left, right
		m_perspect[2], m_perspect[3],			// bottom, top
		m_perspect[4], m_perspect[5]);			// front, back
    
      glMatrixMode(GL_MODELVIEW);
      gluLookAt(m_lookat[0], m_lookat[1], m_lookat[2], m_lookat[3], m_lookat[4],
		m_lookat[5], m_lookat[6], m_lookat[7], m_lookat[8]);
    }
}


void gemwindow :: createMess(t_symbol* s)
{
  if(m_outputState){
    error("GEM: gemwindow: window already made");
    return;
  }
  char* disp = (s==&s_)?NULL:s->s_name;

  createContext(disp);
  if ( !createWindow(disp) )  {
    error("GEM: gemwindow: no window made");
    return;
  }
  swapBuffers();
  swapBuffers();
}
/////////////////////////////////////////////////////////
// destroyMess
//
/////////////////////////////////////////////////////////
void gemwindow :: destroyMess()
{
  // don't want to get rid of this - but why ???
  if (s_singleContext)
    return;

  if (!m_outputState) return;

  //clock_unset(s_windowClock);
  //s_windowClock = NULL;

  glFlush();
  glFinish();

  destroyGemWindow(gfxInfo);

  m_outputState = 0;
    
  // reestablish the const glxContext
  makeCurrent(constInfo);
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
  if (m_outputState)
    cursorGemWindow(gfxInfo, (int)on);
}

/////////////////////////////////////////////////////////
// topmostMess
//
/////////////////////////////////////////////////////////
void gemwindow :: topmostMess(bool on)
{
  if (m_outputState)
    topmostGemWindow(gfxInfo, (int)on);
}


/////////////////////////////////////////////////////////
// resetValues
//
/////////////////////////////////////////////////////////
void gemwindow :: resetValues()
{
#ifdef DEBUG
  post("gemwindow::resetValues entered");
#endif
  GemOutput::resetValues();

  // window hints
  dimensionsMess(500, 500);
  GemOutput::m_width=m_width;
  GemOutput::m_height=m_height;
  
  offsetMess(0,0);
  fullscreenMess(0);
  m_title = "GEM";

  // turn on the cursor
  m_cursor = 1;
  m_topmost = 0;
}



/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void gemwindow :: preRender(GemState){
  makeCurrent(gfxInfo);
  resetState();
}

void gemwindow :: postRender(GemState){
  if(m_buffer==2)swapBuffers();
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
void gemwindow :: bangMessCallback(void *data)
{
  GetMyClass(data)->swapBuffers();
}
