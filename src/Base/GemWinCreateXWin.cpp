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

#include "GemWinCreate.h"
#include <m_pd.h>
#include <X11/cursorfont.h>

#define EVENT_MASK  \
 ExposureMask|StructureNotifyMask|PointerMotionMask|ButtonMotionMask | \
 ButtonReleaseMask | ButtonPressMask | KeyPressMask | KeyReleaseMask


// window creation variables
static int snglBuf24[] = {GLX_RGBA, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_DEPTH_SIZE, 16, None};
static int dblBuf24[] = {GLX_RGBA, GLX_RED_SIZE, 4, GLX_GREEN_SIZE, 4, GLX_BLUE_SIZE, 4, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};
static int snglBuf8[] = {GLX_RGBA, GLX_RED_SIZE, 3, GLX_GREEN_SIZE, 3, GLX_BLUE_SIZE, 2, GLX_DEPTH_SIZE, 16, None};
static int dblBuf8[] = {GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 2, GLX_BLUE_SIZE, 1, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};


Bool WaitForNotify(Display *, XEvent *e, char *arg)
{
  return (e->type == MapNotify) && (e->xmap.window == (Window)arg);
}

int createGemWindow(WindowInfo &info, WindowHints &hints)
{
  XF86VidModeModeInfo **modes;
  int modeNum=4;
  int bestMode=0;

  if ( (info.dpy = XOpenDisplay(hints.display)) == NULL)
    { 
      error("GEM: Could not open display %s",hints.display);
      return(0);
    }

  info.screen  = DefaultScreen(info.dpy);

  XF86VidModeGetAllModeLines(info.dpy, info.screen, &modeNum, &modes);
  info.deskMode = *modes[0];

  XVisualInfo *vi;
  // the user wants double buffer
  if (hints.buffer == 2) {
    // try for a double-buffered on 24bit machine
    vi = glXChooseVisual(info.dpy, info.screen, dblBuf24);
    if (vi == NULL) {
      // try for a double buffered on a 8bit machine
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
    // try for a single buffered on a 24bit machine
    vi = glXChooseVisual(info.dpy, info.screen, snglBuf24);
    if (vi == NULL) {
      // try for a single buffered on a 8bit machine
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

  if (vi->c_class != TrueColor) {
    error("GEM: TrueColor visual required for this program");
    destroyGemWindow(info);
    return(0);
  }
    
  // create the rendering context
  info.context = glXCreateContext(info.dpy, vi, hints.shared, GL_TRUE);
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

  if (hints.fullscreen){
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
  } else {
    if (hints.border){
      swa.override_redirect = False;
      flags=CWBorderPixel|CWColormap|CWEventMask;
    } else {
      swa.override_redirect = True;
      flags=CWBorderPixel|CWColormap|CWEventMask|CWOverrideRedirect;
    }
  }

  info.fs = hints.fullscreen;
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
  XSetStandardProperties(info.dpy, info.win,
			 hints.title, "gem", 
			 None, 0, 0, NULL);
  glXMakeCurrent(info.dpy, info.win, info.context);   

  if (!hints.actuallyDisplay) return(1);

  XMapWindow(info.dpy, info.win);
  XEvent report;
  XIfEvent(info.dpy, &report, WaitForNotify, (char*)info.win);

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
}

void destroyGemWindow(WindowInfo &info)
{
  if (info.dpy)
    {
      if (info.win)
	XDestroyWindow(info.dpy, info.win);
      if (info.context)
	glXDestroyContext(info.dpy, info.context);
      if (info.cmap)
	XFreeColormap(info.dpy, info.cmap);
      if (info.fs){
	XF86VidModeSwitchToMode(info.dpy, info.screen, &info.deskMode);
	XF86VidModeSetViewPort(info.dpy, info.screen, 0, 0);
	info.fs=0;
      }


      XCloseDisplay(info.dpy);
    }

  info.dpy = NULL;
  info.win = 0;
  info.cmap = 0;
  info.context = NULL;
}
