///////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2014 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Gem/GemConfig.h"

#ifdef HAVE_GL_GLX_H
#include "gemglxwindow.h"
#include "Gem/GemGL.h"
#include <stdlib.h>
#include <string.h>
#include <map>

#include "RTE/MessageCallbacks.h"
#include "Gem/Exception.h"


#ifdef HAVE_LIBXXF86VM
#  include <X11/extensions/xf86vmode.h>
#endif
#include <X11/cursorfont.h>

#ifdef HAVE_LIBXRENDER
#include <X11/extensions/Xrender.h>
#endif

// for printf() debugging
#include <stdio.h>

CPPEXTERN_NEW(gemglxwindow);

#define EVENT_MASK                                                      \
  ExposureMask|StructureNotifyMask|PointerMotionMask|ButtonMotionMask | \
  ButtonReleaseMask | ButtonPressMask | KeyPressMask | KeyReleaseMask | DestroyNotify

#ifdef HAVE_LIBXRENDER
static int fbDbl32[] =       {GLX_RENDER_TYPE, GLX_RGBA_BIT,
                              GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                              GLX_DOUBLEBUFFER, True,
                              GLX_RED_SIZE, 8,
                              GLX_GREEN_SIZE, 8,
                              GLX_BLUE_SIZE, 8,
                              GLX_ALPHA_SIZE, 8,
                              GLX_DEPTH_SIZE, 16,
                              None
                             };

static int fbDbl32Stereo[] = {GLX_RENDER_TYPE, GLX_RGBA_BIT,
                              GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                              GLX_DOUBLEBUFFER, True,
                              GLX_STEREO, True,
                              GLX_RED_SIZE, 8,
                              GLX_GREEN_SIZE, 8,
                              GLX_BLUE_SIZE, 8,
                              GLX_ALPHA_SIZE, 8,
                              GLX_DEPTH_SIZE, 16,
                              None
                             };

static int fbDbl24[] =       {GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                              GLX_DOUBLEBUFFER, True,
                              GLX_RED_SIZE, 8,
                              GLX_GREEN_SIZE, 8,
                              GLX_BLUE_SIZE, 8,
                              GLX_DEPTH_SIZE, 16,
                              None
                             };

static int fbDbl24Stereo[] = {GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                              GLX_DOUBLEBUFFER, True,
                              GLX_STEREO, True,
                              GLX_RED_SIZE, 8,
                              GLX_GREEN_SIZE, 8,
                              GLX_BLUE_SIZE, 8,
                              GLX_DEPTH_SIZE, 16,
                              None
                             };

static int fbDbl8[] =       {GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                             GLX_DOUBLEBUFFER, True,
                             GLX_RED_SIZE, 3,
                             GLX_GREEN_SIZE, 3,
                             GLX_BLUE_SIZE, 2,
                             GLX_DEPTH_SIZE, 16,
                             None
                            };

static int fbDbl8Stereo[] = {GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                             GLX_DOUBLEBUFFER, True,
                             GLX_STEREO, True,
                             GLX_RED_SIZE, 3,
                             GLX_GREEN_SIZE, 3,
                             GLX_BLUE_SIZE, 2,
                             GLX_DEPTH_SIZE, 16,
                             None
                            };

static int *dblBufFbCfg[] = {fbDbl32,
                             fbDbl32Stereo,
                             fbDbl24,
                             fbDbl24Stereo,
                             fbDbl8,
                             fbDbl8Stereo,
                             0
                            };

static int fbSngl32[] =       {GLX_RENDER_TYPE, GLX_RGBA_BIT,
                               GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                               GLX_DOUBLEBUFFER, False,
                               GLX_RED_SIZE, 8,
                               GLX_GREEN_SIZE, 8,
                               GLX_BLUE_SIZE, 8,
                               GLX_ALPHA_SIZE, 8,
                               GLX_DEPTH_SIZE, 16,
                               None
                              };

static int fbSngl32Stereo[] = {GLX_RENDER_TYPE, GLX_RGBA_BIT,
                               GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                               GLX_DOUBLEBUFFER, False,
                               GLX_STEREO, True,
                               GLX_RED_SIZE, 8,
                               GLX_GREEN_SIZE, 8,
                               GLX_BLUE_SIZE, 8,
                               GLX_ALPHA_SIZE, 8,
                               GLX_DEPTH_SIZE, 16,
                               None
                              };

static int fbSngl24[] =       {GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                               GLX_DOUBLEBUFFER, False,
                               GLX_RED_SIZE, 8,
                               GLX_GREEN_SIZE, 8,
                               GLX_BLUE_SIZE, 8,
                               GLX_DEPTH_SIZE, 16,
                               None
                              };

static int fbSngl24Stereo[] = {GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                               GLX_DOUBLEBUFFER, False,
                               GLX_STEREO, True,
                               GLX_RED_SIZE, 8,
                               GLX_GREEN_SIZE, 8,
                               GLX_BLUE_SIZE, 8,
                               GLX_DEPTH_SIZE, 16,
                               None
                              };
static int fbSngl8[] =       {GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                              GLX_DOUBLEBUFFER, False,
                              GLX_RED_SIZE, 3,
                              GLX_GREEN_SIZE, 3,
                              GLX_BLUE_SIZE, 2,
                              GLX_DEPTH_SIZE, 16,
                              None
                             };

static int fbSngl8Stereo[] = {GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                              GLX_DOUBLEBUFFER, False,
                              GLX_STEREO, True,
                              GLX_RED_SIZE, 3,
                              GLX_GREEN_SIZE, 3,
                              GLX_BLUE_SIZE, 2,
                              GLX_DEPTH_SIZE, 16,
                              None
                             };

static int *snglBufFbCfg[] = {fbSngl32,
                              fbSngl32Stereo,
                              fbSngl24,
                              fbSngl24Stereo,
                              fbSngl8,
                              fbSngl8Stereo,
                              0
                             };

#endif // HAVE_LIBXRENDER

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
                          None
                         };
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
                                None
                               };
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
                          None
                         };
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
                                None
                               };
static int snglBuf8[] =  {GLX_RGBA,
                          GLX_RED_SIZE, 3,
                          GLX_GREEN_SIZE, 3,
                          GLX_BLUE_SIZE, 2,
                          GLX_DEPTH_SIZE, 16,
                          None
                         };
static int snglBuf8Stereo[] =  {GLX_RGBA,
                                GLX_RED_SIZE, 3,
                                GLX_GREEN_SIZE, 3,
                                GLX_BLUE_SIZE, 2,
                                GLX_DEPTH_SIZE, 16,
                                GLX_STEREO,
                                None
                               };
static int dblBuf8[] =   {GLX_RGBA,
                          GLX_RED_SIZE, 1,
                          GLX_GREEN_SIZE, 2,
                          GLX_BLUE_SIZE, 1,
                          GLX_DEPTH_SIZE, 16,
                          GLX_DOUBLEBUFFER,
                          None
                         };

static int dblBuf8Stereo[] =   {GLX_RGBA,
                                GLX_RED_SIZE, 1,
                                GLX_GREEN_SIZE, 2,
                                GLX_BLUE_SIZE, 1,
                                GLX_DEPTH_SIZE, 16,
                                GLX_DOUBLEBUFFER,
                                GLX_STEREO,
                                None
                               };

static int*dblBufs[]= {
  dblBuf24Stereo,
  dblBuf24,
  dblBuf8Stereo,
  dblBuf8,
  0
};
static int*snglBufs[]= {
  snglBuf24Stereo,
  snglBuf24,
  snglBuf8Stereo,
  snglBuf8,
  0
};

static int xerr;
static int ErrorHandler (Display *dpy, XErrorEvent *event)
{
  // we don't really care about the error
  // let's hope for the best
  if(event) {
    xerr=event->error_code;

    if ( xerr != BadWindow ) {
      char buf[256];
      XGetErrorText (dpy, xerr, buf, sizeof(buf));
      error("Xwin: %s\n", buf);
    } else {
      error("Xwin: BadWindow (%d)\n", xerr);
    }
  }
  return (0);
}

static Bool WaitForNotify(Display *, XEvent *e, char *arg)
{
  return (e->type == MapNotify) && (e->xmap.window == (Window)arg);
}


struct gemglxwindow::PIMPL {
  int         fs;                 // FullScreen

  Display     *dpy;               // X Display
  Window      win;                // X Window
  int         screen;             // X Screen
  Colormap    cmap;               // X color map
  GLXContext  glxcontext;         // OpenGL context
  gem::Context*gemcontext;        // Gem Context (for sharing)

  Atom        delete_atom;

#ifdef HAVE_LIBXXF86VM
  XF86VidModeModeInfo deskMode; // originale ModeLine of the Desktop
#endif

  XIM inputMethod;
  XIC inputContext;


  bool        have_border;

  bool doDispatch;

  PIMPL(void) :
    fs(0),
    dpy(NULL),
    win(0),
    screen(0),
    cmap(0),
    glxcontext(NULL), gemcontext(NULL),
    delete_atom(0),
    inputMethod(NULL),
    inputContext(NULL),

    have_border(false),
    doDispatch(false)
  {
#ifdef HAVE_LIBXXF86VM
    memset(&deskMode, 0, sizeof(deskMode));
#endif
  }
  ~PIMPL(void)
  {
  }

  std::string key2string(XKeyEvent* kb)
  {
#define KEYSTRING_SIZE 10
    char keystring[KEYSTRING_SIZE];
    KeySym keysym_return;
    int len=0;

    if(inputContext) {
      len=Xutf8LookupString(inputContext, kb,keystring,KEYSTRING_SIZE,
                            &keysym_return,NULL);
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

  bool create(std::string display, int buffer, bool fullscreen, bool border,
              int&x, int&y, unsigned int&w, unsigned int&h, bool transparent)
  {
    int modeNum=4;
#ifdef HAVE_LIBXXF86VM
    XF86VidModeModeInfo **modes;
#endif

    XSetErrorHandler (ErrorHandler);

    if ( (dpy = XOpenDisplay(display.c_str())) == NULL) {
      ::error("Could not open display %s",display.c_str());
      return false;
    }
    screen  = DefaultScreen(dpy);

    if ( !glXQueryExtension(dpy, NULL, NULL) ) {
      throw(GemException("X server has no OpenGL GLX extension"));
      return false;
    }

    if (fullscreen) {
      if (!display.empty()) {
        fullscreen=false;
        throw(GemException("fullscreen not available on remote display"));
      } else {
#ifdef HAVE_LIBXXF86VM
        XF86VidModeGetAllModeLines(dpy, screen, &modeNum, &modes);
        deskMode = *modes[0];
#else
        throw(GemException("no xxf86vm-support: cannot switch to fullscreen"));
#endif
      }
    }

    XVisualInfo *vi=0;

#ifdef HAVE_LIBXRENDER
    if (transparent) {

      static GLXFBConfig *fbconfigs, fbconfig;
      static int numfbconfigs;

      // need to get some function pointer at runtime
      typedef GLXFBConfig*(*glXChooseFBConfigProc)(Display* dpy, int screen,
          const int* attribList, int* nitems);
      glXChooseFBConfigProc glXChooseFBConfigFn = (glXChooseFBConfigProc)
          glXGetProcAddress((const GLubyte*)"glXChooseFBConfig");

      typedef XVisualInfo*(*glXGetVisualFromFBConfigProc)(Display* dpy,
          GLXFBConfig fbconfig);
      glXGetVisualFromFBConfigProc glXGetVisualFromFBConfigFn =
        (glXGetVisualFromFBConfigProc)glXGetProcAddress((const GLubyte*)
            "glXGetVisualFromFBConfig");

      if (glXChooseFBConfigFn && glXGetVisualFromFBConfigFn) {
        static int**fbbuf=0;
        switch(buffer) {
        default: ::error("only single/double buffer supported; defaulting to double");
        case 2:
          fbbuf=dblBufFbCfg;
          break;
        case 1:
          fbbuf=snglBufFbCfg;
          break;
        }
        bool breakme=false;
        for(; *fbbuf; fbbuf++) {

          fbconfigs =  glXChooseFBConfigFn(dpy, screen, *fbbuf, &numfbconfigs);

          fbconfig = 0;
          for(int i = 0; i<numfbconfigs; i++) {
            vi = (XVisualInfo*) glXGetVisualFromFBConfigFn(dpy, fbconfigs[i]);
            if(!vi) {
              continue;
            }

            XRenderPictFormat *pict_format = XRenderFindVisualFormat(dpy, vi->visual);
            if(!pict_format) {
              continue;
            }

            fbconfig = fbconfigs[i];
            if(pict_format->direct.alphaMask > 0) {
              ::verbose(0,"choose fbconfig : %d", i);
              breakme = true;
              break;
            }
          }
          if ( breakme ) {
            break;
          }
        }

        if(!fbconfig) {
          ::error("Can't find valid framebuffer configuration, try again with legacy method.");
        } else {
          typedef void(*glXGetFBConfigAttribProc)(Display* dpy,GLXFBConfig fbconfig,
                                                  int attr, int* val);
          glXGetFBConfigAttribProc glXGetFBConfigAttribFn =
            (glXGetFBConfigAttribProc)glXGetProcAddress((const GLubyte*)
                "glXGetFBConfigAttrib");
          if ( glXGetFBConfigAttribFn ) {
            int doublebuffer;
            int red_bits, green_bits, blue_bits, alpha_bits, depth_bits;

            glXGetFBConfigAttribFn(dpy, fbconfig, GLX_DOUBLEBUFFER, &doublebuffer);
            glXGetFBConfigAttribFn(dpy, fbconfig, GLX_RED_SIZE, &red_bits);
            glXGetFBConfigAttribFn(dpy, fbconfig, GLX_GREEN_SIZE, &green_bits);
            glXGetFBConfigAttribFn(dpy, fbconfig, GLX_BLUE_SIZE, &blue_bits);
            glXGetFBConfigAttribFn(dpy, fbconfig, GLX_ALPHA_SIZE, &alpha_bits);
            glXGetFBConfigAttribFn(dpy, fbconfig, GLX_DEPTH_SIZE, &depth_bits);

            ::verbose(0, "FBConfig selected:");
            ::verbose(0, " Doublebuffer: %s", doublebuffer == True ? "Yes" : "No");
            ::verbose(0,
                      " Red Bits: %d, Green Bits: %d, Blue Bits: %d, Alpha Bits: %d, Depth Bits: %d",
                      red_bits, green_bits, blue_bits, alpha_bits, depth_bits);
          } else {
            ::error("can't get glXGetFBConfigAttrib function pointer");
          }
        }
      }
    }

#endif // HAVE_LIBXRENDER

    if (vi == NULL) { // if Xrender method doesn't work try legacy
      static int**buf=0;
      switch(buffer) {
      default: ::error("only single/double buffer supported; defaulting to double");
      case 2:
        buf=dblBufs;
        break;
      case 1:
        buf=snglBufs;
        break;
      }
      // the user wants double buffer
      for(; *buf; buf++) {
        vi = glXChooseVisual(dpy, screen, *buf);
        if(vi) {
          break;
        }
      }
    }

    if (vi == NULL) {
      std::string errstr="Unable to create ";
      switch(buffer) {
      default:
        errstr+="???";
        break;
      case 1:
        errstr+"single";
        break;
      case 2:
        errstr+"double";
        break;
      }
      errstr+=" buffer window";
      throw(GemException(errstr));
      return false;
    }

    if(vi->depth<24) {
      ::verbose(0, "Only using %d color bits", vi->depth);
    }
    if (vi->c_class != TrueColor && vi->c_class != DirectColor) {
      ::error("TrueColor visual required for this program (got %d)",
              vi->c_class);
      return false;
    }
    // create the rendering context
    try {
      // first check whether we have a shared context for 'display'
      GLXContext sharedContext=0;
      if(s_shared.count(display)>0) {
        sharedContext=s_shared[display].glxcontext;;
      }
      glxcontext = glXCreateContext(dpy, vi, sharedContext, GL_TRUE);
    } catch(void*e) {
      glxcontext=NULL;
    }
    if (glxcontext == NULL) {
      throw(GemException("Could not create rendering context"));
      return false;
    }
    // create the X color map
    cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen),
                           vi->visual, AllocNone);
    if (!cmap) {
      throw(GemException("Could not create X colormap"));
      return false;
    }

    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.border_pixel = 0;
    // event_mask creates signal that window has been created
    swa.event_mask = EVENT_MASK;

    int flags;
#ifdef HAVE_LIBXXF86VM
    if (fullscreen) {
      int bestMode=0;
      /* look for mode with requested resolution */
      for (int i = 0; i < modeNum; i++) {
        if ((modes[i]->hdisplay == w) && (modes[i]->vdisplay == w)) {
          bestMode = i;
        }
      }

      XF86VidModeSwitchToMode(dpy, screen, modes[bestMode]);
      XF86VidModeSetViewPort(dpy, screen, 0, 0);
      w = modes[bestMode]->hdisplay;
      h = modes[bestMode]->vdisplay;
      x=y=0;
      XFree(modes);

      swa.override_redirect = True;
      flags=CWBorderPixel|CWColormap|CWEventMask|CWOverrideRedirect;
    } else
#endif
    {
      // !fullscren
      if (border) {
        swa.override_redirect = False;
        flags=CWBorderPixel|CWColormap|CWEventMask|CWOverrideRedirect;
      } else {
        swa.override_redirect = True;
        flags=CWBorderPixel|CWColormap|CWEventMask|CWOverrideRedirect;
      }
    }
    fs = fullscreen;

    win = XCreateWindow(dpy, RootWindow(dpy, vi->screen),
                        x, y, w, h,
                        0, vi->depth, InputOutput,
                        vi->visual, flags, &swa);
    XFree( vi );

    if (!win) {
      throw(GemException("Could not create X window"));
      return false;
    }

    have_border=(True==swa.override_redirect);

    XSelectInput(dpy, win, EVENT_MASK);

    inputMethod = XOpenIM(dpy, NULL, NULL, NULL);
    if(inputMethod) {
      XIMStyle style=0;
      XIMStyles *stylePtr=NULL;
      const char *preedit_attname = NULL;
      XVaNestedList preedit_attlist = NULL;

      if ((XGetIMValues(inputMethod, XNQueryInputStyle, &stylePtr,
                        NULL) != NULL)) {
        stylePtr=NULL;
      }


      /*
       * Select the best input style supported by both the IM and Tk.
       */
      if(stylePtr) {
        for (int i = 0; i < stylePtr->count_styles; i++) {
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
        memset(&inputXfs, 0, sizeof(inputXfs));
        preedit_attname = XNPreeditAttributes;
        preedit_attlist = XVaCreateNestedList(0,
                                              XNSpotLocation, &spot,
                                              XNFontSet, inputXfs,
                                              NULL);
      }

      inputContext=XCreateIC(inputMethod,
                             XNInputStyle, style,
                             XNClientWindow, win,
                             XNFocusWindow, win,
                             preedit_attname, preedit_attlist,
                             NULL);
    }



    /* found a bit at
     * http://biology.ncsa.uiuc.edu/library/SGI_bookshelves/SGI_Developer/books/OpenGL_Porting/sgi_html/apf.html
     * LATER think about reacting on this event...
     */
    delete_atom = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
    if (delete_atom != None) {
      XSetWMProtocols(dpy, win, &delete_atom,1);
    }

    try {
      xerr=0;
      glXMakeCurrent(dpy, win, glxcontext);

      if(xerr!=0) {
        /* seems like the error-handler was called; so something did not work the way it should
         * should we really prevent window-creation in this case?
         * LATER re-think the entire dual-context thing
         */

        throw(GemException("problems making glX-context current: refusing to continue\ntry setting the environment variable GEM_SINGLE_CONTEXT=1"));
        return false;
      }
      Window winDummy;
      unsigned int depthDummy;
      unsigned int borderDummy;
      int x, y;
      XGetGeometry(dpy, win,
                   &winDummy,
                   &x, &y,
                   &w, &h,
                   &borderDummy, &depthDummy);
    } catch(void*e) {
      throw(GemException("Could not make glX-context current"));
      return false;
    }
    return true;
  }
  static std::map<std::string,gemglxwindow::PIMPL>s_shared;
};
std::map<std::string,gemglxwindow::PIMPL>gemglxwindow::PIMPL::s_shared;



/////////////////////////////////////////////////////////
//
// gemglxwindow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemglxwindow :: gemglxwindow(void) :
  m_display(std::string("")),
  m_pimpl(new PIMPL())
{
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemglxwindow :: ~gemglxwindow()
{
  if(m_pimpl->win) {
    destroyMess();
  }

  delete m_pimpl;
}


bool gemglxwindow :: makeCurrent(void)
{
  if(!m_pimpl->dpy || !m_pimpl->win || !m_pimpl->glxcontext) {
    return false;
  }

  xerr=0;
  glXMakeCurrent(m_pimpl->dpy, m_pimpl->win, m_pimpl->glxcontext);
  if(xerr!=0) {
    return false;
  }
  return true;
}

void gemglxwindow :: swapBuffers(void)
{
  glXSwapBuffers(m_pimpl->dpy, m_pimpl->win);
}

void gemglxwindow::dispatch(void)
{
  if(!m_pimpl->doDispatch) {
    return;
  }
  XEvent event;
  XButtonEvent* eb = (XButtonEvent*)&event;
  XKeyEvent* kb  = (XKeyEvent*)&event;
  char keystring[2];
  KeySym keysym_return;
  unsigned long devID=0;

  while (XCheckWindowEvent(m_pimpl->dpy,m_pimpl->win,
                           StructureNotifyMask |
                           KeyPressMask | KeyReleaseMask |
                           PointerMotionMask |
                           ButtonMotionMask |
                           ButtonPressMask |
                           ButtonReleaseMask,
                           &event)) {
    switch (event.type) {
    case ButtonPress:
      button(devID, eb->button-1, 1);
      motion(devID, eb->x, eb->y);
      break;
    case ButtonRelease:
      button(devID, eb->button-1, 0);
      motion(devID, eb->x, eb->y);
      break;
    case MotionNotify:
      motion(devID, eb->x, eb->y);
      if(!m_pimpl->have_border) {
        int err=XSetInputFocus(m_pimpl->dpy, m_pimpl->win, RevertToParent,
                               CurrentTime);
        err=0;
      }
      break;
    case KeyPress:
      key(devID, m_pimpl->key2string(kb), kb->keycode, 1);
      break;
    case KeyRelease:
      key(devID, m_pimpl->key2string(kb), kb->keycode, 0);
      break;
    case ConfigureNotify:
      if ((event.xconfigure.width != m_width) ||
          (event.xconfigure.height != m_height)) {
        m_width=event.xconfigure.width;
        m_height=event.xconfigure.height;
        XResizeWindow(m_pimpl->dpy, m_pimpl->win, m_width, m_height);
        dimension(m_width, m_height);
      }
      if ((event.xconfigure.send_event) &&
          ((event.xconfigure.x != m_xoffset) ||
           (event.xconfigure.y != m_yoffset))) {
        m_xoffset=event.xconfigure.x;
        m_yoffset=event.xconfigure.y;
        position(m_xoffset, m_yoffset);
      }
      break;
    default:
      // post("event %d", event.type);
      break;
    }
  }

  if (XCheckTypedEvent(m_pimpl->dpy,  ClientMessage, &event)) {
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
  case 1:
  case 2:
    m_buffer=buf;
    break;
  default:
    error("buffer can only be '1' (single) or '2' (double) buffered");
    break;
  }
}

/////////////////////////////////////////////////////////
// titleMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: titleMess(const std::string&s)
{
  m_title=s;
  if(m_pimpl->dpy && m_pimpl->win) {
    XSetStandardProperties(m_pimpl->dpy, m_pimpl->win,
                           m_title.c_str(), "gem",
                           None, 0, 0, NULL);
  }

}
/////////////////////////////////////////////////////////
// dimensionsMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: dimensionsMess(unsigned int width,
                                    unsigned int height)
{
  if (width < 1) {
    error("width must be greater than 0");
    return;
  }

  if (height < 1) {
    error ("height must be greater than 0");
    return;
  }

  m_width=width;
  m_height=height;
}

/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
bool gemglxwindow :: create(void)
{
  bool success=true;
  /*
   * hmm, this crashes when enabled
   * when disabled, we don't get textures on two screens
   */
  //~#warning context-sharing disabled
  bool context_sharing=true;
  if(!m_context
      && context_sharing) { /* gemglxwindow::PIMPL::s_shared.count(m_display)>0 */

    gemglxwindow::PIMPL*sharedPimpl=&gemglxwindow::PIMPL::s_shared[m_display];
    if(!sharedPimpl->glxcontext) {
      try {
        int x=0, y=0;
        unsigned int w=1, h=1;
        success=sharedPimpl->create(m_display, 2, false, false, x, y, w, h,
                                    m_transparent);
      } catch (GemException&x) {
        error("creation of shared glxcontext failed: %s", x.what());
        verbose(0, "continuing at your own risk!");
      }
      if(!sharedPimpl->gemcontext) {
        try {
          sharedPimpl->gemcontext = createContext();
        } catch (GemException&x) {
          sharedPimpl->gemcontext = NULL;
          error("creation of shared gem::context failed: %s", x.what());
        }
      }
    }

    m_context=sharedPimpl->gemcontext;
  } else { // no context sharing
    /* creation of gem::Context is deferred until *after* window creation */
  }

  int modeNum=4;
#ifdef HAVE_LIBXXF86VM
  XF86VidModeModeInfo **modes;
#endif
  int fullscreen=m_fullscreen;

  char svalue[3];
  snprintf(svalue, 3, "%d", m_fsaa);
  svalue[2]=0;
  if (m_fsaa!=0) {
    setenv("__GL_FSAA_MODE", svalue, 1);  // this works only for NVIDIA-cards
  }


  try {
    success=m_pimpl->create(m_display, m_buffer, m_fullscreen, m_border,
                            m_xoffset, m_yoffset, m_width, m_height, m_transparent);
  } catch (GemException&x) {
    x.report();
    success=false;
  }
  if(!success) {
    return false;
  }

  /* create a gem::context if we don't already have (a shared) one */
  if(!m_context) {
    try {
      m_context = createContext();
    } catch (GemException&x) {
      m_context = NULL;
      error("creationg of gem::context failed: %s", x.what());
    }
  }


  XMapRaised(m_pimpl->dpy, m_pimpl->win);
  //  XMapWindow(m_pimpl->dpy, m_pimpl->win);
  XEvent report;
  XIfEvent(m_pimpl->dpy, &report, WaitForNotify, (char*)m_pimpl->win);
  if (glXIsDirect(m_pimpl->dpy, m_pimpl->glxcontext)) {
    post("Direct Rendering enabled!");
  }

  cursorMess(m_cursor);
  titleMess(m_title);
  return createGemWindow();
}
void gemglxwindow :: createMess(const std::string&display)
{
  if(m_pimpl->win) {
    error("window already made");
    return;
  }

  m_display=display;
  if(!create()) {
    destroyMess();
    return;
  }
  dimension(m_width, m_height);
  m_pimpl->doDispatch=true;
}
/////////////////////////////////////////////////////////
// destroy window
//
/////////////////////////////////////////////////////////
void gemglxwindow :: destroy(void)
{
  /* both glXMakeCurrent() and XCloseDisplay() will crash the application
   * if the handler of the display (m_pimpl->dpy) is invalid, e.g. because
   * somebody closed the Gem-window with xkill or by clicking on the "x" of the window
   */
  if (m_pimpl->dpy) {
    int err=0;

#ifdef HAVE_LIBXXF86VM
    if (m_pimpl->fs) {
      XF86VidModeSwitchToMode(m_pimpl->dpy, m_pimpl->screen, &m_pimpl->deskMode);
      XF86VidModeSetViewPort(m_pimpl->dpy, m_pimpl->screen, 0, 0);
      m_pimpl->fs=0;
    }
#endif

    /* patch by cesare marilungo to prevent the crash "on my laptop" */
    glXMakeCurrent(m_pimpl->dpy, None,
                   NULL); /* this crashes if no window is there! */
    if (m_pimpl->glxcontext) {
      // this crashes sometimes on my laptop:
      glXDestroyContext(m_pimpl->dpy, m_pimpl->glxcontext);
    }

    if (m_pimpl->win) {
      XUnmapWindow      (m_pimpl->dpy, m_pimpl->win);
      err=XDestroyWindow(m_pimpl->dpy, m_pimpl->win);
      if(err) {
        verbose(1, "XDestroyWindow returned %d", err);
      }
    }

    if (m_pimpl->cmap) {
      err=XFreeColormap(m_pimpl->dpy, m_pimpl->cmap);
      if(err) {
        verbose(1, "XFreeColormap returned %d", err);
      }
    }

    XFlush( m_pimpl->dpy );
    err=XCloseDisplay(m_pimpl->dpy); /* this crashes if no window is there */
    if(err) {
      verbose(1, "XCloseDisplay returned %d", err);
    }
  }
  m_pimpl->dpy = NULL;
  m_pimpl->win = 0;
  m_pimpl->cmap = 0;
  m_pimpl->glxcontext = NULL;
  if(m_pimpl->delete_atom) {
    m_pimpl->delete_atom=None;  /* not very sophisticated destruction...*/
  }

  destroyGemWindow();
}
void gemglxwindow :: destroyMess(void)
{
  m_pimpl->doDispatch=false;
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
void gemglxwindow :: cursorMess(bool state)
{
  m_cursor=state;
  if(!m_pimpl->dpy || !m_pimpl->win) {
    return;
  }

  if (!state) {
    static char data[1] = {0};
    XColor dummy;

    Pixmap blank = XCreateBitmapFromData(m_pimpl->dpy, m_pimpl->win,
                                         data, 1, 1);
    Cursor cursor = XCreatePixmapCursor(m_pimpl->dpy, blank, blank,
                                        &dummy, &dummy, 0, 0);
    XFreePixmap(m_pimpl->dpy, blank);
    XDefineCursor(m_pimpl->dpy, m_pimpl->win, cursor);
  } else {
    XUndefineCursor(m_pimpl->dpy, m_pimpl->win);
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemglxwindow :: obj_setupCallback(t_class *classPtr)
{
}

#endif /* HAVE_GL_GLX_H */
