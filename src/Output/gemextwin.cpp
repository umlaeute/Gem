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



///////////////////////////////
// LATER:
//        move platform-specific code into GemWinCreate* to avoid duplication
//
///////////////////////////////


#include "gemextwin.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
# include <OpenGL/glu.h>
#else
# include <GL/glu.h>
#endif


#ifdef unix
static int snglBuf24[] = {GLX_RGBA, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_DEPTH_SIZE, 16, None};
static int dblBuf24[] = {GLX_RGBA, GLX_RED_SIZE, 4, GLX_GREEN_SIZE, 4, GLX_BLUE_SIZE, 4, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};
static int snglBuf8[] = {GLX_RGBA, GLX_RED_SIZE, 3, GLX_GREEN_SIZE, 3, GLX_BLUE_SIZE, 2, GLX_DEPTH_SIZE, 16, None};
static int dblBuf8[] = {GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 2, GLX_BLUE_SIZE, 1, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};
#elif defined _WINDOWS
typedef struct EnumWindowsParm {void*me; char*pcWinID; };
#endif


CPPEXTERN_NEW(gemextwin)

/////////////////////////////////////////////////////////
//
// gemextwin
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemextwin :: gemextwin() : GemOutput()
{
	post("gemextwin %X", this);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemextwin :: ~gemextwin()
{
  closeWindow();
}

void gemextwin :: closeWindow(){
#ifdef unix
  if(m_info.dpy){
    if (m_info.context) {
      if (!glXMakeCurrent(m_info.dpy, None, NULL))  {
	printf("Could not release drawing context.\n");
      }
      glXDestroyContext(m_info.dpy, m_info.context);
      m_info.context = NULL;
    }
    XCloseDisplay(m_info.dpy);
  }
#elif defined _WINDOWS

#endif
}

void gemextwin :: windowMess(char*cWin){
#ifdef unix
  int base=10;
  if(cWin[0]=='0'){
    base=8;
    if (cWin[1]=='x')base=16;
  }
  int WinID=strtol(cWin, 0, base);

  if(WinID==m_info.win)return;

  m_outputState=0;
  if(WinID<=0){
    error("GEM: gemextwin: context invalid! 0x%X", WinID);
    return;
  }
  XVisualInfo *vi;
  int x, y;
  unsigned int d;
  Atom wmDelete;
  Window winDummy;
  unsigned int borderDummy;
  
  /* get a connection */
  m_info.dpy = XOpenDisplay(0);
  m_info.screen = DefaultScreen(m_info.dpy);

  /* get an appropriate visual */
  vi = glXChooseVisual(m_info.dpy, m_info.screen, dblBuf24);
  if (vi == NULL)  {
    vi = glXChooseVisual(m_info.dpy, m_info.screen, snglBuf24);
    m_buffer = 1;
  }  else    {
    m_buffer = 2;
  }
  /* create a GLX context */
  m_info.context = glXCreateContext(m_info.dpy, vi, 0, GL_TRUE);

  /* an external window ! */
  m_info.win = WinID;
  post("window ID = 0x%x", m_info.win);
  
  XSetStandardProperties(m_info.dpy, m_info.win, "Gem-X",
			   "Gem-X", None, NULL, 0, NULL);
  XMapRaised(m_info.dpy, m_info.win);

  /* connect the glx-context to the window */
  glXMakeCurrent(m_info.dpy, m_info.win, m_info.context);
  XGetGeometry(m_info.dpy, m_info.win, &winDummy, &x, &y,
	       &m_width, &m_height, &borderDummy, &d);
  m_outputState=1;

#elif defined _WINDOWS
  m_outputState=0;

  EnumWindowsParm p;
//  p.me = (void*)(this->x_obj);
  p.me = (void*)(this);
  p.pcWinID = cWin;
  EnumWindows(windowMessCB, (LPARAM)&p); /* try to get a window with this window-ID */
#endif /* OS */
}

#ifdef  _WINDOWS
bool gemextwin :: windowMess(HWND hWnd){
  // setup this window to render to
	post("i (%X) am getting window %X", this, hWnd);
   // create the device context
  m_info.dc = GetDC(m_info.win);
  if (!m_info.dc)  {

    error("GEM: Unable to create device context %d", GetLastError());
    //DestroyWindow(m_info.win);
    return(0);
  }
  /*
  // set the pixel format for the window
  if (!bSetupPixelFormat(m_info.dc, hints))  {
    error("GEM: Unable to set window pixel format");
    ReleaseDC(m_info.win, m_info.dc);
    //DestroyWindow(m_info.win);
    return(0);
  }
  */
  post("now creating device context!");
  // create the OpenGL context
  m_info.context = wglCreateContext(m_info.dc);
  if (!m_info.context)  {
    error("GEM: Unable to create OpenGL context %d", GetLastError());
    ReleaseDC(m_info.win, m_info.dc);
    //DestroyWindow(m_info.win);
    return(0);
  }

  // do we share display lists?
  //if (hints.shared) wglShareLists(hints.shared, m_info.context);

  // make the context the current rendering context
  if (!wglMakeCurrent(m_info.dc, m_info.context))   {
    error("GEM: Unable to make OpenGL context current");
    wglDeleteContext(m_info.context);
    ReleaseDC(m_info.win, m_info.dc);
    //DestroyWindow(m_info.win);
	post("couldn't make context current %d", GetLastError());
    return(0);
  }

  // show and update main window
  ShowWindow(m_info.win, SW_SHOWNORMAL);

  UpdateWindow(m_info.win);
  return true;
}
#endif /* NT */


/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void gemextwin :: makeCurrent(){
#ifdef unix
  glXMakeCurrent(m_info.dpy, m_info.win, m_info.context);   
#elif defined _WINDOWS
#endif /* OS */
}

void gemextwin :: postRender(GemState){
  if (!m_outputState) return;
  if (m_buffer == 2)
#ifdef unix             // for Unix
    glXSwapBuffers(m_info.dpy, m_info.win);
#elif _WINDOWS          // for WinNT
  SwapBuffers(m_info.dc);
#elif __APPLE__		// for Macintosh
  ::aglSwapBuffers(m_info.context);
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
      float xDivy = (float)m_width / (float)m_height;

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

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemextwin :: obj_setupCallback(t_class *classPtr){
  class_addmethod(classPtr, (t_method)&gemextwin::windowMessCallback,
		  gensym("window"), A_SYMBOL, A_NULL);
}

void gemextwin :: windowMessCallback(void *data, t_symbol*s){
  GetMyClass(data)->windowMess(s->s_name);
}

#ifdef _WINDOWS
BOOL CALLBACK gemextwin :: windowMessCB(HWND hWnd, LPARAM lParam){
//	post("trying hwnd %X", hWnd);
  EnumWindowsParm*e=(EnumWindowsParm*)lParam;
  const char* cWin = e->pcWinID;
  // 1.st check whether this is "our" window

  if(GetProp(hWnd, cWin)==NULL)return true; // no, this is another window

  // we have found it, make it our current
  return !(((gemextwin*)(e->me))->windowMess(hWnd));
}
#endif /* NT */
