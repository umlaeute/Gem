/*  -*- mode: ObjC; -*- */
///////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
// megrimm@gmail.com
//
// Implementation file
//
//    Copyright (c) 2011-2018 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2017 me.grimm
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Gem/GemGL.h"
#include "gemmacoswindow.h"

#include <vector>
#include <iostream>
#include <stdio.h>
#include "RTE/MessageCallbacks.h"
#define DEBUGLINE  std::cerr << __FILE__<<":"<<__LINE__<<" ("<<__FUNCTION__<<")" << std::endl;


#include <AvailabilityMacros.h>

#ifndef GEM_NSEVENT_LEGACY
# if defined (MAC_OS_X_VERSION_10_12) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_12
# else
#  define GEM_NSEVENT_LEGACY
# endif
#endif
#ifndef GEM_NSEVENT_LEGACY
# if defined __i386__
#  define GEM_NSEVENT_LEGACY
# endif
#endif


#if defined GEM_NSEVENT_LEGACY
# define NSEventMaskAny NSAnyEventMask

# define NSEventTypeLeftMouseUp NSLeftMouseUp
# define NSEventTypeRightMouseUp NSRightMouseUp
# define NSEventTypeOtherMouseUp NSOtherMouseUp
# define NSEventTypeLeftMouseDown NSLeftMouseDown
# define NSEventTypeRightMouseDown NSRightMouseDown
# define NSEventTypeOtherMouseDown NSOtherMouseDown
# define NSEventTypeMouseMoved NSMouseMoved
# define NSEventTypeLeftMouseDragged NSLeftMouseDragged
# define NSEventTypeRightMouseDragged NSRightMouseDragged
# define NSEventTypeOtherMouseDragged NSOtherMouseDragged
# define NSEventTypeMouseEntered NSMouseEntered
# define NSEventTypeMouseExited NSMouseExited
# define NSEventTypeScrollWheel NSScrollWheel

# define NSEventTypeKeyDown NSKeyDown
# define NSEventTypeKeyUp NSKeyUp

# define NSEventTypeTabletPoint NSTabletPoint
# define NSEventTypeTabletProximity NSTabletProximity

# define NSEventTypeFlagsChanged NSFlagsChanged
# define NSEventModifierFlagCapsLock NSAlphaShiftKeyMask
# define NSEventModifierFlagOption NSAlternateKeyMask
# define NSEventModifierFlagCommand NSCommandKeyMask
# define NSEventModifierFlagControl NSControlKeyMask
# define NSEventModifierFlagFunction NSFunctionKeyMask
# define NSEventModifierFlagNumericPad NSNumericPadKeyMask
# define NSEventModifierFlagShift NSShiftKeyMask
# define NSEventModifierFlagHelp NSHelpKeyMask

# define NSWindowStyleMaskTitled NSTitledWindowMask
# define NSWindowStyleMaskClosable NSClosableWindowMask
# define NSWindowStyleMaskResizable NSResizableWindowMask
# define NSWindowStyleMaskUnifiedTitleAndToolbar NSUnifiedTitleAndToolbarWindowMask
# define NSWindowStyleMaskMiniaturizable NSMiniaturizableWindowMask
# define NSWindowStyleMaskBorderless NSBorderlessWindowMask
#endif

#if defined (MAC_OS_X_VERSION_10_10) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_10
#warning openGL context sharing

static NSDate *distantFuture, *distantPast;

#if 0
// LATER figure out how ObjC's delegation works
@interface WindowResponder
@end
@implementation WindowResponder
- (void)windowDidMove:(NSNotification*)notification
{
  ::post ("window moved");
}
- (void)windowDidResize:(NSNotification*)notification
{
  ::post ("window resized");
}
@end
#endif

@interface GemMacOSView : NSOpenGLView
{
@public gemmacoswindow*parent;
}
@end

typedef GLint oglc_setvalue_t;
@implementation GemMacOSView
- (void) prepareOpenGL
{
#if 0
  oglc_setvalue_t swapInt = 1; // 1==sync to vblank
  [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval]; // set to vbl sync
#endif

  [super prepareOpenGL];
  glEnable(GL_MULTISAMPLE); // Enable multisampling for anti-aliasing (although this may not be necessary)
  oglc_setvalue_t swapRect = 0;
  [[self openGLContext] setValues:&swapRect forParameter:NSOpenGLContextParameterSwapRectangleEnable];
}

- (void)drawRect:(NSRect)rect
{
  parent->renderMess();
}
- (void)reshape
{
  parent->moved();
  [super reshape];
}

#if 0
// since we do the event-scheduling ourselves, we don't really need these
- (BOOL) acceptsFirstResponder {return YES;}
- (void)mouseDown:         (NSEvent*)e { std::cerr << "mouseDown" << std::endl; }
- (void)mouseUp:           (NSEvent*)e { std::cerr << "mouseUp" << std::endl; }
- (void)rightMouseDown:    (NSEvent*)e { std::cerr << "rightMouseDown" << std::endl; }
- (void)rightMouseUp:      (NSEvent*)e { std::cerr << "rightMouseUp" << std::endl; }
- (void)otherMouseDown:    (NSEvent*)e { std::cerr << "otherMouseDown" << std::endl; }
- (void)otherMouseUp:      (NSEvent*)e { std::cerr << "otherMouseUp" << std::endl; }
- (void)mouseMoved:        (NSEvent*)e { std::cerr << "mouseMoved" << std::endl; }
- (void)mouseDragged:      (NSEvent*)e { std::cerr << "mouseDragged" << std::endl; }
- (void)rightMouseDragged: (NSEvent*)e { std::cerr << "rightMouseDragged" << std::endl; }
- (void)otherMouseDragged: (NSEvent*)e { std::cerr << "otherMouseDragged" << std::endl; }
- (void)scrollWheel:       (NSEvent*)e { std::cerr << "scrollWheel" << std::endl; }
- (void)keyDown:           (NSEvent*)e { std::cerr << "keyDown" << std::endl; }
- (void)keyUp:             (NSEvent*)e { std::cerr << "keyUp" << std::endl; }
#endif

@end

struct gemmacoswindow :: PIMPL {
  GemMacOSView*view;
  NSWindow*window;
  //  WindowResponder*delegate;
  float titleBarHeight, menuBarHeight;
  unsigned long modifierFlags;

  PIMPL(void) :
    view(NULL),
    window(NULL),
    //	delegate(NULL),
    titleBarHeight(0.), menuBarHeight(22.),
    modifierFlags(0)
  {}

  ~PIMPL(void)
  {
    cleanup();
  }
  void cleanup(void)
  {
    if(window) {
      [window autorelease];
      [window setReleasedWhenClosed: YES];
      [window close];
    }
    window=NULL;

    if(view) {
      [view release];
    }
    view=NULL;
#if 0
    if(delegate) {
      [window setDelegate: nil];
      [delegate release];
    }
    delegate = NULL;
#endif
  }
};


CPPEXTERN_NEW(gemmacoswindow);

/////////////////////////////////////////////////////////
//
// gemmacoswindow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
static NSAutoreleasePool*arp=NULL;
gemmacoswindow :: gemmacoswindow(void) :
  m_pimpl(new PIMPL())
{
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemmacoswindow :: ~gemmacoswindow()
{
  destroyMess();
  delete m_pimpl;
  m_pimpl=NULL;
}

bool gemmacoswindow :: makeCurrent()
{
  if(!m_pimpl->view) {
    return false;
  }
  [[m_pimpl->view openGLContext] makeCurrentContext];
  return(true);
}
void gemmacoswindow :: swapBuffers()
{
  [[ m_pimpl->view openGLContext ] flushBuffer ];
}
void gemmacoswindow :: renderMess()
{
  bang();
  swapBuffers();
};

void gemmacoswindow :: render()
{
#if 1
  GemWindow::render();
#else
  if(assertCurrentContext()) {
    [ m_pimpl->view setNeedsDisplay: YES ] ;
  }
#endif
}

void gemmacoswindow :: dispatch()
{
  NSEvent *e = NULL;
  if(!m_pimpl->window) {
    return;
  }
  while (( e = [NSApp nextEventMatchingMask: NSEventMaskAny
                      // untilDate: distantFuture // blocking
                      untilDate: distantPast      // nonblocking
                      inMode: NSDefaultRunLoopMode
                      dequeue: YES])) {
    if ([e window] == m_pimpl->window) {
      dispatchEvent(e);
    }
    [NSApp sendEvent: e];
  }
  [NSApp updateWindows];
  [m_pimpl->window flushWindowIfNeeded];
}

static std::string key2name(NSString*s, unsigned short keycode)
{
  std::string keysym = std::string([s UTF8String]);
  switch (keycode) {
  case 36:
    keysym = "Return";
    break;
  case 51:
    keysym = "BackSpace";
    break;
  case 53:
    keysym = "Escape";
    break;
  case 76:
    keysym = "KP_Return";
    break;
  case 18:
    keysym = "D1";
    break;
  case 19:
    keysym = "D2";
    break;
  case 20:
    keysym = "D3";
    break;
  case 21:
    keysym = "D4";
    break;
  case 23:
    keysym = "D5";
    break;
  case 22:
    keysym = "D6";
    break;
  case 26:
    keysym = "D7";
    break;
  case 28:
    keysym = "D8";
    break;
  case 25:
    keysym = "D9";
    break;
  case 29:
    keysym = "D0";
    break;
  case 123:
    keysym = "Left";
    break;
  case 124:
    keysym = "Right";
    break;
  case 126:
    keysym = "Up";
    break;
  case 125:
    keysym = "Down";
    break;
  case 116:
    keysym = "Prior";
    break;
  case 121:
    keysym = "Next";
    break;
  case 115:
    keysym = "Home";
    break;
  case 119:
    keysym = "End";
    break;
  case 122:
    keysym = "F1";
    break;
  case 120:
    keysym = "F2";
    break;
  case 99:
    keysym = "F3";
    break;
  case 118:
    keysym = "F4";
    break;
  case 96:
    keysym = "F5";
    break;
  case 97:
    keysym = "F6";
    break;
  case 98:
    keysym = "F7";
    break;
  case 48:
    keysym = "Tab";
    break;
  default:
    break;
  }

  return keysym;
}

void gemmacoswindow :: dispatchEvent(NSEvent*e)
{
  if(!e) {
    return;
  }
  NSEventType type = [e type];
  int devID=0;

  switch(type) {
  case(NSEventTypeLeftMouseUp):
  case(NSEventTypeRightMouseUp):
  case(NSEventTypeOtherMouseUp):
    button(devID, [e buttonNumber], false);
    break;
  //case(NSLeftMouseDown):
  case(NSEventTypeLeftMouseDown):
  case(NSEventTypeRightMouseDown):
  case(NSEventTypeOtherMouseDown):
    button(devID, [e buttonNumber], [e pressure]);
    break;
  case(NSEventTypeMouseMoved):
  case(NSEventTypeLeftMouseDragged):
  case(NSEventTypeRightMouseDragged):
  case(NSEventTypeOtherMouseDragged): {
    NSPoint p=[e locationInWindow];
    motion(devID, static_cast<int>(p.x), static_cast<int>(p.y));
  }
  break;
  break;
  break;
  case(NSEventTypeMouseEntered):
    info("mouse", "entered");
    break;
  case(NSEventTypeMouseExited):
    info("mouse", "left");
    break;
  case(NSEventTypeScrollWheel):
    // TODO
    break;

  case(NSEventTypeKeyDown):
    if (![e isARepeat]) {
      // how to get names of special keys? e.g. PageUp
      key(devID, key2name([e characters], [e keyCode]), [e keyCode], true);
    }
    break;
  case(NSEventTypeKeyUp):
    key(devID, key2name([e characters], [e keyCode]), [e keyCode], false);
    break;
  case(NSEventTypeFlagsChanged):
    do {
      unsigned long newflags = [e modifierFlags];
      unsigned long oldflags = m_pimpl->modifierFlags;
      if(newflags != oldflags) {
        unsigned long modified = newflags ^ oldflags;
        m_pimpl->modifierFlags = newflags;
#define MODFLAGS2KEY(mask, name) if(modified & mask) key(devID, name, [e keyCode], static_cast<bool>(mask & newflags))
        MODFLAGS2KEY(NSEventModifierFlagCapsLock, "Caps_Lock"); // Caps_Lock
        MODFLAGS2KEY(NSEventModifierFlagShift, "Shift");     // Shift_L
        MODFLAGS2KEY(NSEventModifierFlagControl, "Control"); // Control_L
        MODFLAGS2KEY(NSEventModifierFlagOption, "Option"); // Option
        MODFLAGS2KEY(NSEventModifierFlagCommand, "Command"); // Command
        MODFLAGS2KEY(NSEventModifierFlagNumericPad, "NumPad");
        MODFLAGS2KEY(NSEventModifierFlagHelp, "Help");
        MODFLAGS2KEY(NSEventModifierFlagFunction, "Function");
        //MODFLAGS2KEY(NSEventModifierFlagDeviceIndependentFlagsMask, "Meta_L"); // Meta_L
      }
    } while(false);
    break;

  case(NSEventTypeTabletPoint):
    // TODO (later)
    break;
  case(NSEventTypeTabletProximity):
    // TODO (later)
    break;

#if 0
  // these are only supported starting with OSX-10.6 (or .5?)
  case(NSEventTypeGesture):
    break;
  case(NSEventTypeMagnify):
    break;
  case(NSEventTypeSwipe):
    break;
  case(NSEventTypeRotate):
    break;
  case(NSEventTypeBeginGesture):
    break;
  case(NSEventTypeEndGesture):
    break;
#endif
  }
}


/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
bool gemmacoswindow :: create(void)
{
  NSRect screenRect = [[NSScreen mainScreen] frame];

  NSRect titleframe = NSMakeRect (0, 0, 100, 100);
  NSRect notitleframe = [NSWindow contentRectForFrameRect: titleframe styleMask: NSWindowStyleMaskTitled];
  m_pimpl->titleBarHeight = (titleframe.size.height - notitleframe.size.height);


  int xoffset=m_xoffset;
  // NSWindow is bottom/left, but our offset is top/left
  int yoffset=screenRect.size.height-m_yoffset-m_height;

  NSWindow*window=NULL;
  NSRect contentRect = NSMakeRect(xoffset, yoffset, m_width, m_height);
  if(m_fullscreen) {
    contentRect=screenRect;
  }
  window = [[NSWindow alloc] initWithContentRect:contentRect
                             styleMask:m_border?(
                               NSWindowStyleMaskTitled |
                               NSWindowStyleMaskResizable |
                               NSWindowStyleMaskMiniaturizable |
                               NSWindowStyleMaskClosable )
                             :NSWindowStyleMaskBorderless
                             backing:NSBackingStoreBuffered defer:YES];

  NSView *contentView = [window contentView];
  std::vector<NSOpenGLPixelFormatAttribute>attrvec;

  if(m_buffer==2) {
    attrvec.push_back(NSOpenGLPFADoubleBuffer);
  }
  attrvec.push_back(NSOpenGLPFAAccelerated);
  attrvec.push_back(NSOpenGLPFAColorSize);
  attrvec.push_back(static_cast<NSOpenGLPixelFormatAttribute>(32));
  if (m_fsaa) {
    attrvec.push_back(NSOpenGLPFADepthSize);
    attrvec.push_back(static_cast<NSOpenGLPixelFormatAttribute>(32));
    attrvec.push_back(NSOpenGLPFASampleBuffers);
    attrvec.push_back(static_cast<NSOpenGLPixelFormatAttribute>(8));
    attrvec.push_back(NSOpenGLPFASamples);
    attrvec.push_back(static_cast<NSOpenGLPixelFormatAttribute>(32));
    attrvec.push_back(NSOpenGLPFANoRecovery);
    attrvec.push_back(NSOpenGLPFAMultisample);
  } else {
    attrvec.push_back(NSOpenGLPFADepthSize);
    attrvec.push_back(static_cast<NSOpenGLPixelFormatAttribute>(23));
  }
  if(m_fullscreen) {
    // [window setContentView: m_pimpl->view]; // maybe this? doesn't seem to work though
    //attrvec.push_back(NSOpenGLPFAFullScreen); // deprecated
  }

  attrvec.push_back(static_cast<NSOpenGLPixelFormatAttribute>(0)); // last
  NSOpenGLPixelFormatAttribute*attr = new NSOpenGLPixelFormatAttribute[attrvec.size()];
  int i=0;
  for(i=0; i<attrvec.size(); i++) {
    attr[i]=attrvec[i];
  }

  NSOpenGLPixelFormat *nsglFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attr] autorelease];
  m_pimpl->view = [[GemMacOSView alloc] initWithFrame:[contentView bounds] pixelFormat:nsglFormat];
  m_pimpl->view->parent=this;
  m_pimpl->window=window;
  [contentView addSubview:m_pimpl->view];
  m_pimpl->menuBarHeight = [[m_pimpl->view menu] menuBarHeight];

#if 0
  m_pimpl->delegate = [[WindowResponder alloc] init];
  [window setDelegate: m_pimpl->delegate];
#endif
  [window setAcceptsMouseMovedEvents:YES];
  [window makeKeyAndOrderFront:nil];
  [window setLevel:NSMainMenuWindowLevel+1]; //Set the window level to be above the menu bar.:

  titleMess(m_title);

  bool cgw=  createGemWindow();
  return cgw;
}
void gemmacoswindow :: createMess(const std::string&s)
{
  if(m_pimpl->view) {
    error("window already made!");
    return;
  }

  if (!create()) {
    destroyMess();
  }
}
/////////////////////////////////////////////////////////
// destroy window
//
/////////////////////////////////////////////////////////
void gemmacoswindow :: destroy(void)
{
  m_pimpl->cleanup();
  destroyGemWindow();
}
void gemmacoswindow :: destroyMess(void)
{
  destroy();
}

/////////////////////////////////////////////////////////
// messages
void gemmacoswindow :: titleMess(const std::string&s)
{
  m_title = s;
  if(m_pimpl->window) {
    [m_pimpl->window setTitle:[NSString stringWithUTF8String:m_title.c_str()]];
  }
}
void gemmacoswindow :: dimensionsMess(unsigned int width, unsigned int height)
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
  move();
}
void gemmacoswindow :: move(void)
{
  if(m_pimpl->window) {
    NSRect  screenRect = [[NSScreen mainScreen] frame];
    // NSWindow is bottom/left, but our offset is top/left
    int xoffset=m_xoffset;
    int yoffset=screenRect.size.height-m_yoffset-m_height+m_pimpl->titleBarHeight;
    NSRect frame = NSMakeRect(xoffset, yoffset, m_width, m_height+m_pimpl->titleBarHeight);
    [m_pimpl->window setFrame: frame display: YES];
  }
}
void gemmacoswindow :: moved(void)
{
  if(!m_pimpl->window) {
    return;
  }

  NSRect  screenRect = [[NSScreen mainScreen] frame];
  NSRect            bounds = [m_pimpl->window frame];
  const unsigned width=bounds.size.width;
  const unsigned height=bounds.size.height-m_pimpl->titleBarHeight;
  const int xoffset=bounds.origin.x;
  const int yoffset=screenRect.size.height-bounds.origin.y-height;

  bool doDimen=(width!=m_width || height != m_height);
  bool doOffset=(xoffset!=m_xoffset || yoffset != m_yoffset);

  doDimen=true;
  doOffset=true;

  m_width=width;
  m_height=height;
  m_xoffset=xoffset;
  m_yoffset=yoffset;

  if(doDimen) {
    dimension(m_width, m_height);
  }
  if(doDimen) {
    framebuffersize(m_width, m_height);
  }
  if(doOffset) {
    position(m_xoffset, m_yoffset);
  }
}
void gemmacoswindow :: offsetMess(int x, int y)
{
  m_xoffset = x;
  m_yoffset = y;
  move();
}
void gemmacoswindow :: fullscreenMess(int on)
{
#warning fullscreen
  m_fullscreen = on;
  if(m_pimpl->view) {
    if (m_fullscreen) {
      [[m_pimpl->view openGLContext] setFullScreen];  // replace deprecated setFullScreen with toggleFullScreen?
      //[[m_pimpl->view openGLContext] setView:[window contentView]];

    } else {
      [[m_pimpl->view openGLContext] clearDrawable];
    }
  }
}
void gemmacoswindow :: cursorMess(bool setting)
{
  m_cursor=setting;
  if(m_cursor) {
    [NSCursor unhide];
  } else {
    [NSCursor hide];
  }
}
void gemmacoswindow :: menubarMess(int state)
{
#if 0
  // seems to not work anymore on recent OSX
  if (state == 0)
    SetSystemUIMode( kUIModeAllHidden, kUIOptionDisableAppleMenu | kUIOptionDisableProcessSwitch |
                     kUIOptionDisableSessionTerminate | kUIOptionDisableForceQuit );
  else if (state > 0) {
    SetSystemUIMode( kUIModeNormal, 0 );
  } else {
    SetSystemUIMode( kUIModeAllHidden, kUIOptionAutoShowMenuBar );
  }
#else
  // plain Cocoa, but how to AutoHide?
  switch(state) {
  case 0:
    [NSMenu setMenuBarVisible:NO];
    break;
  default:
  case 1:
    [NSMenu setMenuBarVisible:YES];
    break;
  }
#endif

}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemmacoswindow :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "menubar", menubarMess, int);

  // An attempt to replace deprecated SetFrontProcess
  NSRunningApplication *proc = [NSRunningApplication currentApplication];
  [proc  activateWithOptions: (NSApplicationActivateAllWindows | NSApplicationActivateIgnoringOtherApps)];

  //ProcessSerialNumber proc;
  //GetCurrentProcess(&proc); // Deprecated ... now what?
  //TransformProcessType(&proc, kProcessTransformToForegroundApplication);
  //SetFrontProcess(&proc); // Deprecated ... now what?

  if(NULL==arp) {
    arp=[[NSAutoreleasePool alloc] init];
  }

  distantFuture = [NSDate distantFuture];
  distantPast = [NSDate distantPast];

  [NSApplication sharedApplication];
}

#else
# warning gemmacoswindow requires OSX-10.10
#endif
