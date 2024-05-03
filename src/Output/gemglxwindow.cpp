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

#include "gemglxwindow.h"
#include "Gem/GemGL.h"

#include "RTE/MessageCallbacks.h"
#include "Gem/Exception.h"

#ifdef HAVE_LIBXXF86VM
# include <X11/extensions/xf86vmode.h>
#endif
#include <X11/cursorfont.h>

#ifdef HAVE_LIBXRENDER
#include <X11/extensions/Xrender.h>
#endif

#include <iostream>
#include <map>

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <climits>

CPPEXTERN_NEW(gemglxwindow);

#define EVENT_MASK                                                      \
  ExposureMask|StructureNotifyMask|PointerMotionMask|ButtonMotionMask | \
  ButtonReleaseMask | ButtonPressMask | KeyPressMask | KeyReleaseMask | DestroyNotify



#define GETGLXFUN(type, name) type name##Fn = (type) glXGetProcAddress((const GLubyte*) #name )


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
      pd_error(0, "Xwin: %s\n", buf);
    } else {
      pd_error(0, "Xwin: BadWindow (%d)\n", xerr);
    }
  }
  return (0);
}

static Bool WaitForNotify(Display *, XEvent *e, char *arg)
{
  return (e->type == MapNotify) && (e->xmap.window == (Window)arg);
}


namespace {
  struct glxFB {
    Display*dpy;
    GLXFBConfig fb;
    PFNGLXGETFBCONFIGATTRIBPROC _configattrib;
    PFNGLXGETVISUALFROMFBCONFIGPROC _getvisual;
    glxFB(Display*_dpy, GLXFBConfig _fb)
      : dpy(_dpy)
      , fb(_fb)
      {
        GETGLXFUN(PFNGLXGETFBCONFIGATTRIBPROC, glXGetFBConfigAttrib);
        GETGLXFUN(PFNGLXGETVISUALFROMFBCONFIGPROC, glXGetVisualFromFBConfig);
        _configattrib = glXGetFBConfigAttribFn;
        _getvisual = glXGetVisualFromFBConfigFn;
      }

    int ConfigAttrib(int attrib) {
      int value = -1;
      if(_configattrib)
        _configattrib(dpy, fb, attrib, &value);
      return value;
    }

    XVisualInfo*Visual() {
      if(_getvisual)
        return (XVisualInfo*) _getvisual(dpy, fb);
      return 0;
    }
  };

  struct FBconfig {
    Display *dpy;
    GLXFBConfig fb;
    int redBits, greenBits, blueBits, alphaBits;
    int depthBits, stencilBits;
    int accumRedBits, accumGreenBits, accumBlueBits, accumAlphaBits;
    int auxBuffers;
    int samples, depth;
    bool stereo, sRGB, doublebuffer, transparency;
    int colormode;
    int renderType, drawableType;

    FBconfig(int colorBits, int accumBits)
      : dpy(0), fb(0)
      , redBits(colorBits), greenBits(colorBits), blueBits(colorBits), alphaBits(colorBits)
      , depthBits(-1), stencilBits(-1)
      , accumRedBits(accumBits), accumGreenBits(accumBits), accumBlueBits(accumBits), accumAlphaBits(accumBits)
      , auxBuffers(-1)
      , samples(-1), depth(-1)
      , stereo(false), sRGB(false), doublebuffer(true), transparency(false)
      , colormode(TrueColor)
      , renderType(GLX_RGBA_BIT), drawableType(GLX_WINDOW_BIT)
      { }

    FBconfig(Display *_dpy, GLXFBConfig _fb)
      : dpy(_dpy), fb(_fb)
      , redBits(0), greenBits(0), blueBits(0), alphaBits(0)
      , depthBits(0), stencilBits(0)
      , accumRedBits(0), accumGreenBits(0), accumBlueBits(0), accumAlphaBits(0)
      , auxBuffers(0)
      , samples(0), depth(0)
      , stereo(false), sRGB(false), doublebuffer(false), transparency(false)
      , colormode(0)
      , renderType(0), drawableType(0)
      {
        glxFB gfb (dpy, fb);
        redBits = gfb.ConfigAttrib(GLX_RED_SIZE);
        greenBits = gfb.ConfigAttrib(GLX_GREEN_SIZE);
        blueBits = gfb.ConfigAttrib(GLX_BLUE_SIZE);
        alphaBits = gfb.ConfigAttrib(GLX_ALPHA_SIZE);

        depthBits = gfb.ConfigAttrib(GLX_DEPTH_SIZE);
        stencilBits = gfb.ConfigAttrib(GLX_STENCIL_SIZE);

        accumRedBits = gfb.ConfigAttrib(GLX_ACCUM_RED_SIZE);
        accumGreenBits = gfb.ConfigAttrib(GLX_ACCUM_GREEN_SIZE);
        accumBlueBits = gfb.ConfigAttrib(GLX_ACCUM_BLUE_SIZE);
        accumAlphaBits = gfb.ConfigAttrib(GLX_ACCUM_ALPHA_SIZE);

        auxBuffers = gfb.ConfigAttrib(GLX_AUX_BUFFERS);

        stereo = gfb.ConfigAttrib(GLX_STEREO);

//        if (GLXEW_ARB_framebuffer_sRGB)
        sRGB = gfb.ConfigAttrib(GLX_FRAMEBUFFER_SRGB_CAPABLE_ARB) > 0;

        doublebuffer = gfb.ConfigAttrib(GLX_DOUBLEBUFFER) > 0;

        XVisualInfo *vi = gfb.Visual();
        if(vi) {
          depth = vi->depth;
          colormode = vi->c_class;
#ifdef HAVE_LIBXRENDER
          XRenderPictFormat *pf = XRenderFindVisualFormat(dpy, vi->visual);
          transparency = pf && pf->direct.alphaMask>0;
#endif
          XFree(vi);
        }

//        if (GLXEW_ARB_multisample)
        samples = gfb.ConfigAttrib(GLX_SAMPLES);

        renderType = gfb.ConfigAttrib(GLX_RENDER_TYPE);
        drawableType = gfb.ConfigAttrib(GLX_DRAWABLE_TYPE);
      }

    int bufferDifference(const FBconfig&cur) {
      int missing = 0;
      if (alphaBits > 0 && !cur.alphaBits) missing++;
      if (depthBits > 0 && !cur.depthBits) missing++;
      if (stencilBits > 0 && !cur.stencilBits) missing++;
      if (auxBuffers > 0 && auxBuffers > cur.auxBuffers) missing += auxBuffers - cur.auxBuffers;
      if (samples > 0 && !cur.samples) missing++;
      if (transparency != cur.transparency) missing++;
      return missing;
    }
#define CHANNEL_DIFFERENCE(member) \
    (member >= 0)?((member-cur.member)*(member-cur.member) + !cur.member):0
    int colorChannelDifference(const FBconfig&cur) {
      int diff = 0;
      diff += CHANNEL_DIFFERENCE(redBits);
      diff += CHANNEL_DIFFERENCE(greenBits);
      diff += CHANNEL_DIFFERENCE(blueBits);
      return diff;
    }
    int extraChannelDifference(const FBconfig&cur) {
      int diff = 0;

      diff += CHANNEL_DIFFERENCE(alphaBits);
      diff += CHANNEL_DIFFERENCE(depthBits);
      diff += CHANNEL_DIFFERENCE(stencilBits);
      diff += CHANNEL_DIFFERENCE(accumRedBits);
      diff += CHANNEL_DIFFERENCE(accumGreenBits);
      diff += CHANNEL_DIFFERENCE(accumBlueBits);
      diff += CHANNEL_DIFFERENCE(accumAlphaBits);
      diff += CHANNEL_DIFFERENCE(samples);

      if (sRGB != cur.sRGB) diff++;
      return diff;
    }


  };

  std::ostream& operator<<(std::ostream& out, const FBconfig& fb) {
    out
      << "FBconfig("
      << "rgba=["
      << fb.redBits << ","
      << fb.greenBits << ","
      << fb.blueBits << ","
      << fb.alphaBits << "],"
      << "accum=["
      << fb.accumRedBits << ","
      << fb.accumGreenBits << ","
      << fb.accumBlueBits << ","
      << fb.accumAlphaBits << "],"
      << "depth=" << fb.depthBits << "/" << fb.depth << ","
      << "stencil=" << fb.stencilBits << ","
      << "samples=" << fb.samples << ","
      << "aux=" << fb.auxBuffers;

    if(fb.doublebuffer) out << ",doublebuffer";
    if(fb.stereo) out << ",stereo";
    if(fb.sRGB) out << ",sRGB";
    if(fb.transparency) out << ",transparency";
    switch(fb.colormode) {
    case StaticGray:  out << ",StaticGray"  ; break;
    case GrayScale:   out << ",GrayScale"   ; break;
    case StaticColor: out << ",StaticColor" ; break;
    case PseudoColor: out << ",PseudoColor" ; break;
    case TrueColor:   out << ",TrueColor"   ; break;
    case DirectColor: out << ",DirectColor" ; break;
    default:          out << ",UnknownColor"; break;
    }
    return out << ")";
  }
};


struct gemglxwindow::PIMPL {
  CPPExtern   *parent;
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

  PIMPL() :
    parent(0),
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

    if ( !len || (keysym_return & 0xff00)== 0xff00 ) {
      //non alphanumeric key: use keysym
      return std::string(XKeysymToString(keysym_return));
    }

    if (!len) {
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
              int&x, int&y, unsigned int&w, unsigned int&h, bool transparent,
              int msaa)
  {
    GETGLXFUN(PFNGLXGETFBCONFIGSPROC, glXGetFBConfigs);
    GETGLXFUN(PFNGLXGETVISUALFROMFBCONFIGPROC, glXGetVisualFromFBConfig);

    switch(buffer) {
    default:
      pd_error(parent, "only single/double buffer supported; defaulting to double");
    case 2:
      buffer = 2;
      break;
    case 1:
      break;
    }

#ifdef HAVE_LIBXXF86VM
    int modeNum=4;
    XF86VidModeModeInfo **modes;
#endif

    XSetErrorHandler (ErrorHandler);

    if ( (dpy = XOpenDisplay(display.c_str())) == NULL) {
      ::pd_error(parent, "Could not open display %s",display.c_str());
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

    std::vector<FBconfig>fbconfs;
    int nativeCount;
    GLXFBConfig* nativeConfigs = glXGetFBConfigsFn(dpy, screen, &nativeCount);
    for (int i=0; i<nativeCount; i++) {
      FBconfig fb(dpy, nativeConfigs[i]);

      if (!(fb.drawableType & GLX_WINDOW_BIT)) {
        /* not a window */
        continue;
      }
      if (!(fb.renderType & GLX_RGBA_BIT)) {
        /* does not allow RGBA rendering */
        continue;
      }
      if (buffer != (fb.doublebuffer?2:1)) {
        /* requested doublebuffer but got singlebuffer, or vice versa */
        continue;
      }

      if (fb.colormode != TrueColor && fb.colormode != DirectColor) {
        /* TrueColor visual required for this program... */
        continue;
      }
      if (fb.depthBits < 16) {
        /* too shallow */
        continue;
      }

      //std::cerr << fb << std::endl;

      fbconfs.push_back(fb);
    }

    /* now that we got a number of possible configurations, find the best one */
    FBconfig desired(8, 8);
    desired.doublebuffer = (buffer != 1);
    desired.depthBits = 16;
    desired.stencilBits = 8;
    desired.transparency = transparent;
    desired.samples = msaa;

    unsigned int leastBufferDiff = UINT_MAX;
    unsigned int leastColorDiff = UINT_MAX;
    unsigned int leastExtraDiff = UINT_MAX;

    const FBconfig*best = &desired;
    for (auto it = fbconfs.begin(); it != fbconfs.end(); it++) {
      const FBconfig&current = *it;
      unsigned int bufferDiff = desired.bufferDifference(current);
      unsigned int colorDiff = desired.colorChannelDifference(current);
      unsigned int extraDiff = desired.extraChannelDifference(current);

      if (bufferDiff < leastBufferDiff)
        best = &current;
      else if (bufferDiff == leastBufferDiff)
      {
        if ((colorDiff < leastColorDiff) ||
            (colorDiff == leastColorDiff && extraDiff < leastExtraDiff))
        {
          best = &current;
        }
      }
      if (&current == best)
      {
        leastBufferDiff = bufferDiff;
        leastColorDiff = colorDiff;
        leastExtraDiff = extraDiff;
      }
    }
    XVisualInfo *vi = 0;

    //std::cerr << desired << " -> " << *best << std::endl;

    if (best->dpy && best->fb) {
      vi = (XVisualInfo*) glXGetVisualFromFBConfigFn(best->dpy, best->fb);
    }

    if (vi == NULL) {
      std::string errstr="Unable to create ";
      switch(buffer) {
      default:
        errstr+="???";
        break;
      case 1:
        errstr+="single";
        break;
      case 2:
        errstr+="double";
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
      pd_error(parent, "TrueColor visual required for this program (got %d)",
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
      //int x, y;

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
  m_pimpl->parent = this;
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
        if(err)
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
        framebuffersize(m_width, m_height);
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
  bool context_sharing=false;
  if(!m_context
      && context_sharing) { /* gemglxwindow::PIMPL::s_shared.count(m_display)>0 */

    gemglxwindow::PIMPL*sharedPimpl=&gemglxwindow::PIMPL::s_shared[m_display];
    if(!sharedPimpl->glxcontext) {
      try {
        int x=0, y=0;
        unsigned int w=1, h=1;
        success=sharedPimpl->create(m_display, 2, false, false, x, y, w, h,
                                    m_transparent, m_fsaa);
      } catch (GemException&ex) {
        error("creation of shared glxcontext failed: %s", ex.what());
        verbose(0, "continuing at your own risk!");
      }
      if(!sharedPimpl->gemcontext) {
        try {
          sharedPimpl->gemcontext = createContext();
        } catch (GemException&ex) {
          sharedPimpl->gemcontext = NULL;
          error("creation of shared gem::context failed: %s", ex.what());
        }
      }
    }

    m_context=sharedPimpl->gemcontext;
  } else { // no context sharing
    /* creation of gem::Context is deferred until *after* window creation */
  }

  if(m_fsaa>=0 && m_fsaa<100) {
    char svalue[3];
    snprintf(svalue, 3, "%d", m_fsaa);
    svalue[2]=0;
    if (m_fsaa!=0) {
      setenv("__GL_FSAA_MODE", svalue, 1);  // this works only for NVIDIA-cards
    }
  }

  try {
    success=m_pimpl->create(m_display, m_buffer, m_fullscreen, m_border,
                            m_xoffset, m_yoffset, m_width, m_height, m_transparent, m_fsaa);
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
      error("creation of gem::context failed: %s", x.what());
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
  framebuffersize(m_width, m_height);
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
