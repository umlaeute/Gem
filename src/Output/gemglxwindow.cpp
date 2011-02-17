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
//    Copyright (c) 2001-2011 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Base/GemConfig.h"

#ifndef HAVE_GL_GLX_H
#include "gemglxwindow.h"
#include "Base/GemGL.h"
#include <stdio.h>
#include <stdlib.h>

#include "RTE/MessageCallbacks.h"


#ifdef HAVE_LIBXXF86VM
#  include <X11/extensions/xf86vmode.h>
#endif
#include <X11/cursorfont.h>

CPPEXTERN_NEW(gemglxwindow);

#define EVENT_MASK                                                      \
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

static int xerr;
static int ErrorHandler (Display *dpy, XErrorEvent *event)
{
  // we don't really care about the error
  // let's hope for the best
  if(event)
    xerr=event->error_code;  

  if ( event->error_code != BadWindow ) {
    char buf[256];
    XGetErrorText (dpy, event->error_code, buf, sizeof(buf));
    error("Xwin: %s\n", buf);
  } else
    error("Xwin: BadWindow (%d)\n", xerr);
  return (0);
}

static Bool WaitForNotify(Display *, XEvent *e, char *arg)
{
  return (e->type == MapNotify) && (e->xmap.window == (Window)arg);
}


 

struct gemglxwindow::Info {
  int         fs;                 // FullScreen
  bool        have_constContext;  // 1 if we have a constant context

  Display     *dpy;               // X Display
  Window      win;                // X Window
  int         screen;             // X Screen
  Colormap    cmap;               // X color map
  GLXContext  context;            // OpenGL context

#warning sharedContext in Info
  GLXContext  shared;// The GLXcontext to share rendering with

  Atom        delete_atom;
  
#ifdef HAVE_LIBXXF86VM
  XF86VidModeModeInfo deskMode; // originale ModeLine of the Desktop
#endif

  XIM inputMethod;
  XIC inputContext;


  bool        have_border;

  bool doDispatch;

  Info(void) : 
    fs(0),
    have_constContext(false),
    dpy(NULL), 
    win(0), 
    cmap(0), 
    context(NULL), 
    shared(NULL),
    delete_atom(0),
#ifdef HAVE_LIBXXF86VM
    //    deskMode(0),
#endif
    inputMethod(NULL),
    inputContext(NULL),

    have_border(false),
    doDispatch(false)
  {
  }
  ~Info(void) {
  }

  std::string key2string(XKeyEvent* kb) {
#define KEYSTRING_SIZE 10
    char keystring[KEYSTRING_SIZE];
    KeySym keysym_return;
    int len=0;

    if(inputContext) {
      len=Xutf8LookupString(inputContext, kb,keystring,KEYSTRING_SIZE,&keysym_return,NULL);
    }
    if(len<1) {
      len=XLookupString(kb,keystring,2,&keysym_return,NULL);
    }

    if ( (keysym_return & 0xff00)== 0xff00 ) {
      //non alphanumeric key: use keysym
      return std::string(XKeysymToString(keysym_return));
    }
    
    if (len==0) {
      //modifier key:use keysym
      //triggerKeyboardEvent(XKeysymToString(keysym_return), kb->keycode, 1);
    } else if(len<KEYSTRING_SIZE) {
      keystring[len]=0;
    } else {
      keystring[KEYSTRING_SIZE-1]=0;
    }

    return std::string(keystring);
  }
};

/////////////////////////////////////////////////////////
//
// gemglxwindow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemglxwindow :: gemglxwindow(void) :
  m_fsaa(0),
  m_title(std::string("GEM")),
  m_border(true),
  m_fullscreen(false),
  m_xoffset(0), m_yoffset(0),
  m_cursor(false),
  real_w(0), real_h(0), real_x(0), real_y(0),
  m_display(std::string("")),
  m_actuallyDisplay(true),
  m_info(new Info())
{
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemglxwindow :: ~gemglxwindow()
{
  destroyMess();
}


bool gemglxwindow :: makeCurrent(void){
  if(!m_info->dpy || !m_info->win || !m_info->context)
    return false;

  xerr=0;
  glXMakeCurrent(m_info->dpy, m_info->win, m_info->context);
  if(xerr!=0) {
    return false;
  }
  return true;
}

void gemglxwindow :: swapBuffers(void) {
  glXSwapBuffers(m_info->dpy, m_info->win);
}

/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: renderMess(void)
{
  render();
}

void gemglxwindow::dispatch(void) {
  if(!m_info->doDispatch)return;
  XEvent event; 
  XButtonEvent* eb = (XButtonEvent*)&event; 
  XKeyEvent* kb  = (XKeyEvent*)&event; 
  XResizeRequestEvent *res = (XResizeRequestEvent*)&event;
  char keystring[2];
  KeySym keysym_return;

  while (XCheckWindowEvent(m_info->dpy,m_info->win,
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
          button(eb->button-1, 1);
          motion(eb->x, eb->y);
          break; 
        case ButtonRelease: 
          button(eb->button-1, 0);
          motion(eb->x, eb->y);
          break; 
        case MotionNotify: 
          motion(eb->x, eb->y);
          if(!m_info->have_border) {
            int err=XSetInputFocus(m_info->dpy, m_info->win, RevertToParent, CurrentTime);
            err=0;
          }
          break; 
        case KeyPress:
          key(m_info->key2string(kb), kb->keycode, 1);
          break;
        case KeyRelease:
          key(m_info->key2string(kb), kb->keycode, 0);
          break;

        case ResizeRequest:
          XResizeWindow(m_info->dpy, m_info->win, res->width, res->height);
          dimension(res->width, res->height);
          break;
        default:
          break; 
        }
    }
  
  if (XCheckTypedEvent(m_info->dpy,  ClientMessage, &event)) {
    info("window", "destroy");
    //    GemMan::destroyWindowSoon();
  }
}


/////////////////////////////////////////////////////////
// bufferMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: bufferMess(int buf)
{
  switch(buf) {
  case 1: case 2:
    m_buffer=buf;
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
void gemglxwindow :: fsaaMess(int value)
{
  m_fsaa=value;
}

/////////////////////////////////////////////////////////
// titleMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: titleMess(t_symbol* s)
{
  m_title=s->s_name;
}
/////////////////////////////////////////////////////////
// border
//
/////////////////////////////////////////////////////////
void gemglxwindow :: borderMess(bool setting)
{
  m_border=setting;
}
/////////////////////////////////////////////////////////
// dimensionsMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: dimensionsMess(int width, int height)
{
  if (width <= 0) {
    error("width must be greater than 0");
    return;
  }
    
  if (height <= 0 ) {
    error ("height must be greater than 0");
    return;
  }

  m_width=width;
  m_height=height;
}
/////////////////////////////////////////////////////////
// fullscreenMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: fullscreenMess(bool on)
{
  m_fullscreen = on;
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: offsetMess(int x, int y)
{
  m_xoffset=x;
  m_yoffset=y;
}

/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
bool gemglxwindow :: create(void)
{
  int modeNum=4;
  int bestMode=0;
#ifdef HAVE_LIBXXF86VM
  XF86VidModeModeInfo **modes;
#endif
  int fullscreen=m_fullscreen;

  char svalue[3];
  snprintf(svalue, 3, "%d", m_fsaa);
  svalue[2]=0;
  if (m_fsaa!=0) setenv("__GL_FSAA_MODE", svalue, 1); // this works only for NVIDIA-cards

  XSetErrorHandler (ErrorHandler);

  if ( (m_info->dpy = XOpenDisplay(m_display.c_str())) == NULL) { 
    error("Could not open display %s",m_display.c_str());
    return false;
  }
  m_info->screen  = DefaultScreen(m_info->dpy);

  if ( !glXQueryExtension(m_info->dpy, NULL, NULL) ) {
    error("X server has no OpenGL GLX extension");
    return false;
  } 

  if (fullscreen){
    if (!m_display.empty()){
      error("fullscreen not available on remote display");
      fullscreen=false;
    } else {
#ifdef HAVE_LIBXXF86VM
      XF86VidModeGetAllModeLines(m_info->dpy, m_info->screen, &modeNum, &modes);
      m_info->deskMode = *modes[0];
#else
      error("no xxf86vm-support: cannot switch to fullscreen");
#endif
    }
  }
  XVisualInfo *vi;
  // the user wants double buffer
  if (m_buffer == 2) {
    // try for a double-buffered on 24bit machine (try stereo first)
    vi = glXChooseVisual(m_info->dpy, m_info->screen, dblBuf24Stereo);
    if (vi == NULL)
      vi = glXChooseVisual(m_info->dpy, m_info->screen, dblBuf24);
    if (vi == NULL) {
      // try for a double buffered on a 8bit machine (try stereo first)
      vi = glXChooseVisual(m_info->dpy, m_info->screen, dblBuf8Stereo);
      if(vi == NULL)
        vi = glXChooseVisual(m_info->dpy, m_info->screen, dblBuf8);
      if (vi == NULL) {
        error("Unable to create double buffer window");
        return false;
      }
      post("Only using 8 color bits");
    }
  }
  // the user wants single buffer
  else {
    // try for a single buffered on a 24bit machine (try stereo first)
    vi = glXChooseVisual(m_info->dpy, m_info->screen, snglBuf24Stereo);
    if (vi == NULL)
      vi = glXChooseVisual(m_info->dpy, m_info->screen, snglBuf24);
    if (vi == NULL) {
      // try for a single buffered on a 8bit machine (try stereo first)
      vi = glXChooseVisual(m_info->dpy, m_info->screen, snglBuf8Stereo);
      if (vi == NULL)
        vi = glXChooseVisual(m_info->dpy, m_info->screen, snglBuf8);
      if (vi == NULL) {
        error("Unable to create single buffer window");
        return false;
      }
      post("Only using 8 color bits");
    }
    m_buffer = 1;
  }

  if (vi->c_class != TrueColor && vi->c_class != DirectColor) {
    error("TrueColor visual required for this program (got %d)", vi->c_class);
    return false;
  }
  // create the rendering context
  try {
    m_info->context = glXCreateContext(m_info->dpy, vi, m_info->shared, GL_TRUE);
  } catch(void*e){
    m_info->context=NULL;
  }
  if (m_info->context == NULL) {
    error("Could not create rendering context");
    return false;
  }
  // create the X color map
  m_info->cmap = XCreateColormap(m_info->dpy, RootWindow(m_info->dpy, vi->screen), 
                                 vi->visual, AllocNone);
  if (!m_info->cmap) {
    error("Could not create X colormap");
    return false;
  }

  XSetWindowAttributes swa;
  swa.colormap = m_info->cmap;
  swa.border_pixel = 0;
  // event_mask creates signal that window has been created
  swa.event_mask = EVENT_MASK;

  real_w=m_width;
  real_h=m_height;

  real_x=m_xoffset;
  real_y=m_yoffset;

  int flags;
#ifdef HAVE_LIBXXF86VM
  if (fullscreen){
    /* look for mode with requested resolution */
    for (int i = 0; i < modeNum; i++) {
      if ((modes[i]->hdisplay == m_width) && (modes[i]->vdisplay == m_height)) {
        bestMode = i;
      }
    }
    
    XF86VidModeSwitchToMode(m_info->dpy, m_info->screen, modes[bestMode]);
    XF86VidModeSetViewPort(m_info->dpy, m_info->screen, 0, 0);
    real_w = modes[bestMode]->hdisplay;
    real_h = modes[bestMode]->vdisplay;
    real_x=real_y=0;
    XFree(modes);

    swa.override_redirect = True;
    flags=CWBorderPixel|CWColormap|CWEventMask|CWOverrideRedirect;
  } else
#endif
    { // !fullscren
      if (m_border){
        swa.override_redirect = False;
        flags=CWBorderPixel|CWColormap|CWEventMask|CWOverrideRedirect;
      } else {
        swa.override_redirect = True;
        flags=CWBorderPixel|CWColormap|CWEventMask|CWOverrideRedirect;
      }
    }
  m_info->fs = fullscreen;

  m_info->win = XCreateWindow(m_info->dpy, RootWindow(m_info->dpy, vi->screen),
                              m_xoffset, m_yoffset, real_w, real_h,
                              0, vi->depth, InputOutput, 
                              vi->visual, flags, &swa);
  if (!m_info->win) {
    error("Could not create X window");
    return false;
  }

  m_info->have_border=(True==swa.override_redirect);

  XSelectInput(m_info->dpy, m_info->win, EVENT_MASK);

  m_info->inputMethod = XOpenIM(m_info->dpy, NULL, NULL, NULL);
  if(m_info->inputMethod) {
    XIMStyle style=NULL;
    XIMStyles *stylePtr=NULL;
    const char *preedit_attname = NULL;
    XVaNestedList preedit_attlist = NULL;

    if ((XGetIMValues(m_info->inputMethod, XNQueryInputStyle, &stylePtr, NULL) != NULL)) {
      stylePtr=NULL;
    }


    /*
     * Select the best input style supported by both the IM and Tk.
     */
    int i=0;
    if(stylePtr) {
      for (i = 0; i < stylePtr->count_styles; i++) {
        XIMStyle thisStyle = stylePtr->supported_styles[i];
        if (thisStyle == (XIMPreeditPosition | XIMStatusNothing)) {
          style = thisStyle;
          break;
        } else if (thisStyle == (XIMPreeditNothing | XIMStatusNothing)) {
          style = thisStyle;
        }
      }
      XFree(stylePtr);
    }


    if (style & XIMPreeditPosition) {
      XPoint spot = {0, 0};
      XFontSet inputXfs;
      preedit_attname = XNPreeditAttributes;
      preedit_attlist = XVaCreateNestedList(0,
                                            XNSpotLocation, &spot,
                                            XNFontSet, inputXfs,
                                            NULL);
    }


    m_info->inputContext=XCreateIC(m_info->inputMethod,
                                   XNInputStyle, style,
                                   XNClientWindow, m_info->win,
                                   XNFocusWindow, m_info->win,
                                   preedit_attname, preedit_attlist,
                                   NULL);
  }



  /* found a bit at
   * http://biology.ncsa.uiuc.edu/library/SGI_bookshelves/SGI_Developer/books/OpenGL_Porting/sgi_html/apf.html
   * LATER think about reacting on this event...
   */
  m_info->delete_atom = XInternAtom(m_info->dpy, "WM_DELETE_WINDOW", True);
  if (m_info->delete_atom != None)
    XSetWMProtocols(m_info->dpy, m_info->win, &m_info->delete_atom,1);

  XSetStandardProperties(m_info->dpy, m_info->win,
                         m_title.c_str(), "gem", 
                         None, 0, 0, NULL);

  try{
    xerr=0;
    glXMakeCurrent(m_info->dpy, m_info->win, m_info->context);

    if(xerr!=0) {
      /* seems like the error-handler was called; so something did not work the way it should
       * should we really prevent window-creation in this case?
       * LATER re-think the entire dual-context thing
       */

      error("problems making glX-context current: refusing to continue");
      error("try setting the environment variable GEM_SINGLE_CONTEXT=1");
      return false;
    }
  }catch(void*e){
    error("Could not make glX-context current");
    return false;
  }

  if (m_actuallyDisplay) {
    XMapRaised(m_info->dpy, m_info->win);
    //  XMapWindow(m_info->dpy, m_info->win);
    XEvent report;
    XIfEvent(m_info->dpy, &report, WaitForNotify, (char*)m_info->win);
    if (glXIsDirect(m_info->dpy, m_info->context))
      post("Direct Rendering enabled!");
  }
  return createContext();
}
void gemglxwindow :: createMess(std::string display)
{
  m_display=display;
  if(!create()) {
    destroyMess();
    return;
  }
  m_info->doDispatch=true;
}
/////////////////////////////////////////////////////////
// destroy window
//
/////////////////////////////////////////////////////////
void gemglxwindow :: destroy(void)
{
  /* both glXMakeCurrent() and XCloseDisplay() will crash the application
   * if the handler of the display (m_info->dpy) is invalid, e.g. because
   * somebody closed the Gem-window with xkill or by clicking on the "x" of the window
   */
  if (m_info->dpy) {
    int err=0;
    /* patch by cesare marilungo to prevent the crash "on my laptop" */
    glXMakeCurrent(m_info->dpy, None, NULL); /* this crashes if no window is there! */
    
    if (m_info->win)
      err=XDestroyWindow(m_info->dpy, m_info->win);
    if (m_info->have_constContext && m_info->context) {
      // this crashes sometimes on my laptop:
      glXDestroyContext(m_info->dpy, m_info->context);
    }
    if (m_info->cmap)
      err=XFreeColormap(m_info->dpy, m_info->cmap);
    
#ifdef HAVE_LIBXXF86VM
    if (m_info->fs){
      XF86VidModeSwitchToMode(m_info->dpy, m_info->screen, &m_info->deskMode);
      XF86VidModeSetViewPort(m_info->dpy, m_info->screen, 0, 0);
      m_info->fs=0;
    }
#endif
    
    err=XCloseDisplay(m_info->dpy); /* this crashes if no window is there */
  }
  m_info->dpy = NULL;
  m_info->win = 0;
  m_info->cmap = 0;
  m_info->context = NULL;
  if(m_info->delete_atom)m_info->delete_atom=None; /* not very sophisticated destruction...*/
  
  destroyContext();
}
void gemglxwindow :: destroyMess(void)
{
  m_info->doDispatch=false;
  if(makeCurrent()) {
    destroy();
  } else {
    error("unable to destroy current window");
  }
}


/////////////////////////////////////////////////////////
// cursorMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: cursorMess(bool setting)
{

}


/////////////////////////////////////////////////////////
// print info
//
/////////////////////////////////////////////////////////
void gemglxwindow :: print(void)
{
  //  GemMan::printInfo();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemglxwindow :: obj_setupCallback(t_class *classPtr)
{
  //  class_addbang(classPtr, reinterpret_cast<t_method>(&gemglxwindow::renderMessCallback));
  CPPEXTERN_MSG0(classPtr, "bang", renderMess);

  CPPEXTERN_MSG1(classPtr, "title", titleMess, t_symbol*);
  CPPEXTERN_MSG1(classPtr, "create", createMess, std::string);
  CPPEXTERN_MSG0(classPtr, "destroy", destroyMess);
  CPPEXTERN_MSG1(classPtr, "buffer", bufferMess, int);
  CPPEXTERN_MSG1(classPtr, "fullscreen", fullscreenMess, bool);
  CPPEXTERN_MSG2(classPtr, "dimen", dimensionsMess, int, int);
  CPPEXTERN_MSG2(classPtr, "offset", offsetMess, int, int);
  CPPEXTERN_MSG1(classPtr, "cursor", cursorMess, bool);
  CPPEXTERN_MSG1(classPtr, "border", borderMess, bool);
  CPPEXTERN_MSG1(classPtr, "FSAA", fsaaMess, int);

  CPPEXTERN_MSG0(classPtr, "print", print);
  
}

#endif /* HAVE_GL_GLX_H */
