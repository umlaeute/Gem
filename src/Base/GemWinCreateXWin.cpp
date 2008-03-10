////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef __unix__

#include "GemWinCreate.h"
#include <m_pd.h>
#include <X11/cursorfont.h>
#include <stdio.h>
#include <stdlib.h>

#define EVENT_MASK  \
 ExposureMask|StructureNotifyMask|PointerMotionMask|ButtonMotionMask | \
 ButtonReleaseMask | ButtonPressMask | KeyPressMask | KeyReleaseMask | ResizeRedirectMask | DestroyNotify


// window creation variables
static int snglBuf24[] = {GLX_RGBA, 
                          GLX_RED_SIZE, 8, 
                          GLX_GREEN_SIZE, 8, 
                          GLX_BLUE_SIZE, 8, 
                          GLX_DEPTH_SIZE, 16, 
                          GLX_STENCIL_SIZE, 8, 
                          GLX_ACCUM_RED_SIZE, 8,
                          GLX_ACCUM_GREEN_SIZE, 8,
                          GLX_ACCUM_BLUE_SIZE, 8,
                          None};
static int snglBuf24Stereo[] = {GLX_RGBA, 
                          GLX_RED_SIZE, 8, 
                          GLX_GREEN_SIZE, 8, 
                          GLX_BLUE_SIZE, 8, 
                          GLX_DEPTH_SIZE, 16, 
                          GLX_STENCIL_SIZE, 8, 
                          GLX_ACCUM_RED_SIZE, 8,
                          GLX_ACCUM_GREEN_SIZE, 8,
                          GLX_ACCUM_BLUE_SIZE, 8,
								  GLX_STEREO,
                          None};
static int dblBuf24[] =  {GLX_RGBA, 
                          GLX_RED_SIZE, 4, 
                          GLX_GREEN_SIZE, 4, 
                          GLX_BLUE_SIZE, 4, 
                          GLX_DEPTH_SIZE, 16, 
                          GLX_STENCIL_SIZE, 8, 
                          GLX_ACCUM_RED_SIZE, 8,
                          GLX_ACCUM_GREEN_SIZE, 8,
                          GLX_ACCUM_BLUE_SIZE, 8,
                          GLX_DOUBLEBUFFER, 
                          None};
static int dblBuf24Stereo[] =  {GLX_RGBA, 
                          GLX_RED_SIZE, 4, 
                          GLX_GREEN_SIZE, 4, 
                          GLX_BLUE_SIZE, 4, 
                          GLX_DEPTH_SIZE, 16, 
                          GLX_STENCIL_SIZE, 8, 
                          GLX_ACCUM_RED_SIZE, 8,
                          GLX_ACCUM_GREEN_SIZE, 8,
                          GLX_ACCUM_BLUE_SIZE, 8,
                          GLX_DOUBLEBUFFER, 
								  GLX_STEREO,
                          None};
static int snglBuf8[] =  {GLX_RGBA, 
                          GLX_RED_SIZE, 3, 
                          GLX_GREEN_SIZE, 3, 
                          GLX_BLUE_SIZE, 2, 
                          GLX_DEPTH_SIZE, 16, 
                          None};
static int snglBuf8Stereo[] =  {GLX_RGBA, 
                          GLX_RED_SIZE, 3, 
                          GLX_GREEN_SIZE, 3, 
                          GLX_BLUE_SIZE, 2, 
                          GLX_DEPTH_SIZE, 16, 
								  GLX_STEREO,
                          None};
static int dblBuf8[] =   {GLX_RGBA, 
                          GLX_RED_SIZE, 1, 
                          GLX_GREEN_SIZE, 2, 
                          GLX_BLUE_SIZE, 1, 
                          GLX_DEPTH_SIZE, 16, 
                          GLX_DOUBLEBUFFER, 
                          None};

static int dblBuf8Stereo[] =   {GLX_RGBA, 
                          GLX_RED_SIZE, 1, 
                          GLX_GREEN_SIZE, 2, 
                          GLX_BLUE_SIZE, 1, 
                          GLX_DEPTH_SIZE, 16, 
                          GLX_DOUBLEBUFFER, 
								  GLX_STEREO,
                          None};

int ErrorHandler (Display *dpy, XErrorEvent *event)
{
  // we don't really care about the error
  // let's hope for the best
  printf("error handler: please report to the Gem-dev's!!\n");
  if ( event->error_code != BadWindow ) {
    char buf[256];
    XGetErrorText (dpy, event->error_code, buf, sizeof(buf));
    error("GEM-Xwin: %s", buf);
  }
  return (0);
}

Bool WaitForNotify(Display *, XEvent *e, char *arg)
{
  return (e->type == MapNotify) && (e->xmap.window == (Window)arg);
}

int createGemWindow(WindowInfo &info, WindowHints &hints)
{
#ifdef HAVE_LIBXXF86VM
  XF86VidModeModeInfo **modes;
#endif
  int modeNum=4;
  int bestMode=0;
  int fullscreen=hints.fullscreen;

  char svalue[3];
  int fsaa=(int)hints.fsaa;
  sprintf(svalue, "%d", fsaa);
  setenv("__GL_FSAA_MODE", svalue, 1); // this works only for NVIDIA-cards

  XSetErrorHandler (ErrorHandler);

  if ( (info.dpy = XOpenDisplay(hints.display)) == NULL)
    { 
      error("GEM: Could not open display %s",hints.display);
      return(0);
    }

  info.screen  = DefaultScreen(info.dpy);

  /* this used to be in GemMan::createContext()
   * and produced a number of XDisplays that were not closed
   * i really think that it fits better in here;
   * however, i have currently no way to test what really happens
   * if the X-server has no glx extension
   */
  if ( !glXQueryExtension(info.dpy, NULL, NULL) )
    {
      error("GEM: X server has no OpenGL GLX extension");
      destroyGemWindow(info);
      return 0;
    } 

  if (fullscreen){
    if (hints.display){
      error("GEM: fullscreen not available on remote display");
      fullscreen=0;
    } else {
#ifdef HAVE_LIBXXF86VM
      XF86VidModeGetAllModeLines(info.dpy, info.screen, &modeNum, &modes);
      info.deskMode = *modes[0];
#else
      error("GEM: no xxf86vm-support: cannot switch to fullscreen");
#endif
    }
  }

  XVisualInfo *vi;
  // the user wants double buffer
  if (hints.buffer == 2) {
    // try for a double-buffered on 24bit machine (try stereo first)
    vi = glXChooseVisual(info.dpy, info.screen, dblBuf24Stereo);
	 if (vi == NULL)
		 vi = glXChooseVisual(info.dpy, info.screen, dblBuf24);
    if (vi == NULL) {
      // try for a double buffered on a 8bit machine (try stereo first)
      vi = glXChooseVisual(info.dpy, info.screen, dblBuf8Stereo);
		if(vi == NULL)
			vi = glXChooseVisual(info.dpy, info.screen, dblBuf8);
      if (vi == NULL) {
	error("GEM: Unable to create double buffer window");
	destroyGemWindow(info);
	return(0);
      }
      post("GEM: Only using 8 color bits");
    }
  }
  // the user wants single buffer
  else {
    // try for a single buffered on a 24bit machine (try stereo first)
    vi = glXChooseVisual(info.dpy, info.screen, snglBuf24Stereo);
	 if (vi == NULL)
		 vi = glXChooseVisual(info.dpy, info.screen, snglBuf24);
    if (vi == NULL) {
      // try for a single buffered on a 8bit machine (try stereo first)
      vi = glXChooseVisual(info.dpy, info.screen, snglBuf8Stereo);
		if (vi == NULL)
			vi = glXChooseVisual(info.dpy, info.screen, snglBuf8);
      if (vi == NULL) {
	error("GEM: Unable to create single buffer window");
	destroyGemWindow(info);
	return(0);
      }
      post("GEM: Only using 8 color bits");
    }
    hints.buffer = 1;
  }

  if (vi->c_class != TrueColor && vi->c_class != DirectColor) {
    error("GEM: TrueColor visual required for this program (got %d)", vi->c_class);
    destroyGemWindow(info);
    return(0);
  }
  // create the rendering context
  try {
    info.context = glXCreateContext(info.dpy, vi, hints.shared, GL_TRUE);
  } catch(void*e){
    info.context=NULL;
  }
  if (info.context == NULL) {
    error("GEM: Could not create rendering context");
    destroyGemWindow(info);
    return(0);
  }
  // create the X color map
  info.cmap = XCreateColormap(info.dpy, RootWindow(info.dpy, vi->screen), 
			      vi->visual, AllocNone);
  if (!info.cmap) {
    error("GEM: Could not create X colormap");
    destroyGemWindow(info);
    return(0);
  }

  XSetWindowAttributes swa;
  swa.colormap = info.cmap;
  swa.border_pixel = 0;
  // event_mask creates signal that window has been created
  swa.event_mask = EVENT_MASK;

  hints.real_w = hints.width;
  hints.real_h = hints.height;

  int flags;
  int x = hints.x_offset;
  int y = hints.y_offset;
#ifdef HAVE_LIBXXF86VM
  if (fullscreen){
    /* look for mode with requested resolution */
    for (int i = 0; i < modeNum; i++) {
      if ((modes[i]->hdisplay == hints.width) && (modes[i]->vdisplay == hints.height)) {
	bestMode = i;
      }
    }

    XF86VidModeSwitchToMode(info.dpy, info.screen, modes[bestMode]);
    XF86VidModeSetViewPort(info.dpy, info.screen, 0, 0);
    hints.real_w = modes[bestMode]->hdisplay;
    hints.real_h = modes[bestMode]->vdisplay;
    XFree(modes);

    swa.override_redirect = True;
    flags=CWBorderPixel|CWColormap|CWEventMask|CWOverrideRedirect;
    x=y=0;
  } else
#endif
  { // !fullscren
    if (hints.border){
      swa.override_redirect = False;
      flags=CWBorderPixel|CWColormap|CWEventMask|CWOverrideRedirect;
    } else {
      swa.override_redirect = True;
      flags=CWBorderPixel|CWColormap|CWEventMask|CWOverrideRedirect;
    }
  }

  info.fs = fullscreen;
  info.win = XCreateWindow(info.dpy, RootWindow(info.dpy, vi->screen),
			   x, y, hints.real_w, hints.real_h,
			   0, vi->depth, InputOutput, 
			   vi->visual, flags, &swa);
  if (!info.win)
    {
      error("GEM: Could not create X window");
      destroyGemWindow(info);
      return(0);
    }

  XSelectInput(info.dpy, info.win, EVENT_MASK);

  /* found a bit at
   * http://biology.ncsa.uiuc.edu/library/SGI_bookshelves/SGI_Developer/books/OpenGL_Porting/sgi_html/apf.html
   * LATER think about reacting on this event...
   */

  if ((info.delete_atom = XInternAtom(info.dpy, "WM_DELETE_WINDOW",
                              True)) != None)
    XSetWMProtocols(info.dpy, info.win, &info.delete_atom,1);

  XSetStandardProperties(info.dpy, info.win,
			 hints.title, "gem", 
			 None, 0, 0, NULL);
  try{
    glXMakeCurrent(info.dpy, info.win, info.context);   
  }catch(void*e){
    error("GEM: Could not make glX-context current");
    destroyGemWindow(info);
    return(0);
  }

  if (!hints.actuallyDisplay) return(1);
  XMapRaised(info.dpy, info.win);
  //  XMapWindow(info.dpy, info.win);
  XEvent report;
  XIfEvent(info.dpy, &report, WaitForNotify, (char*)info.win);
  if (glXIsDirect(info.dpy, info.context))post("Direct Rendering enabled!");

  return(1);
}

int cursorGemWindow(WindowInfo &info, int state)
{
  if (!state) {
    static char data[1] = {0};
	 
    Cursor cursor;
    Pixmap blank;
    XColor dummy;
	 
    blank = XCreateBitmapFromData(info.dpy, info.win,
				  data, 1, 1);
	 
    cursor = XCreatePixmapCursor(info.dpy, blank, blank,
				 &dummy, &dummy, 0, 0);
    XFreePixmap(info.dpy, blank);
    XDefineCursor(info.dpy, info.win,cursor);
  }
  else
    XUndefineCursor(info.dpy, info.win);
  return 0; //?
}

int topmostGemWindow(WindowInfo &info, int state){
  /* we don't give a warning to not be annoying */
  return 1;
}

void destroyGemWindow(WindowInfo &info)
{
  /* both glXMakeCurrent() and XCloseDisplay() will crash the application
   * if the handler of the display (info.dpy) is invalid, e.g. because
   * somebody closed the Gem-window with xkill or by clicking on the "x" of the window
   */
  if (info.dpy)
    {
      int err=0;
      /* patch by cesare marilungo to prevent the crash "on my laptop" */
      glXMakeCurrent(info.dpy, None, NULL); /* this crashes if no window is there! */

      if (info.win)
	err=XDestroyWindow(info.dpy, info.win);
      if (info.have_constContext && info.context) {
        // this crashes sometimes on my laptop:
	glXDestroyContext(info.dpy, info.context);
      }
      if (info.cmap)
	err=XFreeColormap(info.dpy, info.cmap);

#ifdef HAVE_LIBXXF86VM
      if (info.fs){
	XF86VidModeSwitchToMode(info.dpy, info.screen, &info.deskMode);
	XF86VidModeSetViewPort(info.dpy, info.screen, 0, 0);
	info.fs=0;
      }
#endif

      err=XCloseDisplay(info.dpy); /* this crashes if no window is there */
    }
  info.dpy = NULL;
  info.win = 0;
  info.cmap = 0;
  info.context = NULL;
  if(info.delete_atom)info.delete_atom=0; /* not very sophisticated destruction...*/
}

void gemWinSwapBuffers(WindowInfo&nfo)
{
  glXSwapBuffers(nfo.dpy, nfo.win);
}

void gemWinMakeCurrent(WindowInfo&nfo) 
{
  if (!nfo.dpy && !nfo.win && !nfo.context)return; // do not crash
  glXMakeCurrent(nfo.dpy, nfo.win, nfo.context);   
}


bool initGemWin(void) {
  /* nothing to be done here... */
  return 1;
}

#endif // unix
