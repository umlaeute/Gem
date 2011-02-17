///////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2011 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Base/GemGL.h"
#include "gemcocoawindow.h"

#include <vector>
#include <iostream>
#include <stdio.h>
#include "RTE/MessageCallbacks.h"
#define DEBUGLINE  std::cerr << __FILE__<<":"<<__LINE__<<" ("<<__FUNCTION__<<")" << std::endl;



static NSDate *distantFuture, *distantPast;

#if 0
@interface WindowResponder 
@end
@implementation WindowResponder
- (void)windowDidMove:(NSNotification*)notification { 
  ::post ("window moved");
}
- (void)windowDidResize:(NSNotification*)notification { 
  ::post ("window resized");
}
@end
#endif

@interface GemCocoaView : NSOpenGLView
{
  @public gemcocoawindow*parent;
}
@end

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
typedef GLint oglc_setvalue_t;
#else
typedef long int oglc_setvalue_t;
#endif
@implementation GemCocoaView
- (void) prepareOpenGL
{
#if 0
  oglc_setvalue_t swapInt = 1; // 1==sync to vblank
  [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval]; // set to vbl sync
#endif
  oglc_setvalue_t swapRect = 0;
  [[self openGLContext] setValues:&swapRect forParameter:NSOpenGLCPSwapRectangleEnable];
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

struct gemcocoawindow :: PIMPL {
  GemCocoaView*view;
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

  ~PIMPL(void) {
   cleanup();
  }
  void cleanup(void) {
  if(window) {
   [window autorelease];
   [window setReleasedWhenClosed: YES];
   [window close];
  }
  window=NULL;

  if(view) {
   [view release]; 
  }
#if 0
  if(delegate) {
   [window setDelegate: nil];
   [delegate release];
  }
#endif
  }
};


CPPEXTERN_NEW(gemcocoawindow)

/////////////////////////////////////////////////////////
//
// gemcocoawindow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
static NSAutoreleasePool*arp=NULL;
gemcocoawindow :: gemcocoawindow(void) :
  m_fsaa(0),
  m_title("GEM"),
  m_border(true),
  m_xoffset(0), m_yoffset(0),
  m_fullscreen(false),
  m_cursor(false),
  m_pimpl(new PIMPL())
{
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemcocoawindow :: ~gemcocoawindow()
{
  destroyMess();
  delete m_pimpl;
  m_pimpl=NULL;
}

bool gemcocoawindow :: makeCurrent(){
 if(!m_pimpl->view)return false;
 [[m_pimpl->view openGLContext] makeCurrentContext];
 return(true);
}
void gemcocoawindow :: swapBuffers() {
  [[ m_pimpl->view openGLContext ] flushBuffer ];
}
void gemcocoawindow :: renderMess() {
  bang();
  swapBuffers();
};

void gemcocoawindow :: render() {
 if(assertCurrentContext()) {
  [ m_pimpl->view setNeedsDisplay: YES ] ;
 }
}

void gemcocoawindow :: dispatch() {
  NSEvent *e = NULL;
  if(!m_pimpl->window)return;
  while (( e = [NSApp nextEventMatchingMask: NSAnyEventMask
         // untilDate: distantFuture // blocking
         untilDate: distantPast      // nonblocking
         inMode: NSDefaultRunLoopMode
         dequeue: YES])) {
    if ([e window] == m_pimpl->window) dispatchEvent(e);
    [NSApp sendEvent: e];
  }
  [NSApp updateWindows];
  [m_pimpl->window flushWindowIfNeeded];
}
void gemcocoawindow :: dispatchEvent(NSEvent*e) {
 if(!e)return;
 NSEventType type = [e type];
 switch(type) {
case(NSLeftMouseUp): 
case(NSRightMouseUp):
case(NSOtherMouseUp):
button([e buttonNumber], false);
break;
case(NSLeftMouseDown): 
case(NSRightMouseDown):
case(NSOtherMouseDown):
button([e buttonNumber], [e pressure]);
break;
case(NSMouseMoved):
case(NSLeftMouseDragged):
case(NSRightMouseDragged):
case(NSOtherMouseDragged):
{
NSPoint p=[e locationInWindow];
motion(static_cast<int>(p.x), static_cast<int>(p.y));
}
break;
break;
break;
case(NSMouseEntered):
info("mouse", "entered");
break;
case(NSMouseExited):
info("mouse", "left");
break;
case(NSScrollWheel):
break;

case(NSKeyDown):
if (![e isARepeat]) {
// how to get names of special keys? e.g. PageUp
 key(std::string([[e characters] UTF8String]), [e keyCode], true);
}
break;
case(NSKeyUp):
 key(std::string([[e characters] UTF8String]), [e keyCode], false);
break;
case(NSFlagsChanged):
  do {
   unsigned long newflags = [e modifierFlags];
   unsigned long oldflags = m_pimpl->modifierFlags;
  if(newflags != oldflags) {
   unsigned long modified = newflags ^ oldflags;
   m_pimpl->modifierFlags = newflags;
#define MODFLAGS2KEY(mask, name) if(modified & mask) key(name, [e keyCode], static_cast<bool>(mask & newflags))
   MODFLAGS2KEY(NSAlphaShiftKeyMask, "AlphaShift");
   MODFLAGS2KEY(NSShiftKeyMask, "Shift");
   MODFLAGS2KEY(NSControlKeyMask, "Control");
   MODFLAGS2KEY(NSCommandKeyMask, "Command");
   MODFLAGS2KEY(NSNumericPadKeyMask, "NumPad");
   MODFLAGS2KEY(NSHelpKeyMask, "Help");
   MODFLAGS2KEY(NSFunctionKeyMask, "Function");
   MODFLAGS2KEY(NSAlternateKeyMask, "Alt");
  }
  } while(false);
break;

case(NSTabletPoint):
break;
case(NSTabletProximity):
break;

#if 0
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
bool gemcocoawindow :: create(void)
{
  if(m_pimpl->view) {
    error("window already made!");
    return false;
  }
  NSRect  screenRect = [[NSScreen mainScreen] frame];

  NSRect titleframe = NSMakeRect (0, 0, 100, 100);
  NSRect notitleframe = [NSWindow contentRectForFrameRect: titleframe styleMask: NSTitledWindowMask];
  m_pimpl->titleBarHeight = (titleframe.size.height - notitleframe.size.height);


  int xoffset=m_xoffset;
  // NSWindow is bottom/left, but our offset is top/left
  int yoffset=screenRect.size.height-m_yoffset-m_height;

  NSWindow*window=NULL;
  NSRect contentRect = NSMakeRect(xoffset, yoffset, m_width, m_height);
if(m_fullscreen) {
  contentRect=screenRect;
}
  window = [[NSWindow alloc] initWithContentRect:contentRect styleMask:m_border?(NSTitledWindowMask|NSMiniaturizableWindowMask|NSClosableWindowMask):NSBorderlessWindowMask backing:NSBackingStoreBuffered defer:YES];
 
  NSView *contentView = [window contentView];
  std::vector<NSOpenGLPixelFormatAttribute>attrvec;
#if 0
  NSOpenGLPixelFormatAttribute attr[] = 
    {
      NSOpenGLPFADoubleBuffer,
      NSOpenGLPFAAccelerated,
      NSOpenGLPFAColorSize, (NSOpenGLPixelFormatAttribute) 32,
      NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute) 23,
      (NSOpenGLPixelFormatAttribute) 0
    };
#endif

  if(m_buffer==2)
    attrvec.push_back(NSOpenGLPFADoubleBuffer);
  attrvec.push_back(NSOpenGLPFAAccelerated);
  attrvec.push_back(NSOpenGLPFAColorSize);
  attrvec.push_back(static_cast<NSOpenGLPixelFormatAttribute>(32));
  attrvec.push_back(NSOpenGLPFADepthSize);
  attrvec.push_back(static_cast<NSOpenGLPixelFormatAttribute>(23));
  attrvec.push_back(static_cast<NSOpenGLPixelFormatAttribute>(0)); // last
  if(m_fullscreen) {
    attrvec.push_back(NSOpenGLPFAFullScreen);
  }

  NSOpenGLPixelFormatAttribute*attr = new NSOpenGLPixelFormatAttribute[attrvec.size()];
  int i=0;
  for(i=0; i<attrvec.size(); i++) {
    attr[i]=attrvec[i];
  }
  NSOpenGLPixelFormat *nsglFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attr] autorelease];
  m_pimpl->view = [[GemCocoaView alloc] initWithFrame:[contentView bounds] pixelFormat:nsglFormat];
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

  titleMess(m_title);

  return createContext();
}
void gemcocoawindow :: createMess(void) {
 if (!create()) {
	destroyMess();
 }
}
/////////////////////////////////////////////////////////
// destroy window
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: destroy(void)
{
  m_pimpl->cleanup();
  destroyContext();
}
void gemcocoawindow :: destroyMess(void)
{
  destroy();
}

/////////////////////////////////////////////////////////
// messages
void gemcocoawindow :: bufferMess(int buf) {
  switch(buf) {
  case 1: case 2:
    m_buffer=buf;
    break;
  default:
    error("buffer can only be '1' (single) or '2' (double) buffered");
    break;
  }
}
void gemcocoawindow :: titleMess(std::string s) {
  m_title = s;
  if(m_pimpl->window) {
    [m_pimpl->window setTitle:[NSString stringWithUTF8String:m_title.c_str()]];
  }
}
void gemcocoawindow :: dimensionsMess(int width, int height) {
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
void gemcocoawindow :: move(void) {
  if(m_pimpl->window) {
    NSRect  screenRect = [[NSScreen mainScreen] frame];
    // NSWindow is bottom/left, but our offset is top/left
    int xoffset=m_xoffset;
    int yoffset=screenRect.size.height-m_yoffset-m_height+m_pimpl->titleBarHeight;
    NSRect frame = NSMakeRect(xoffset, yoffset, m_width, m_height+m_pimpl->titleBarHeight);
    [m_pimpl->window setFrame: frame display: YES];
  }
}
void gemcocoawindow :: moved(void) {
  if(!m_pimpl->window) return;

  NSRect  screenRect = [[NSScreen mainScreen] frame];
  NSRect            bounds = [m_pimpl->window frame];
  const unsigned width=bounds.size.width;
  const unsigned height=bounds.size.height-m_pimpl->titleBarHeight;
  const int xoffset=bounds.origin.x;
  const int yoffset=screenRect.size.height-bounds.origin.y-height;

  startpost("%dx%d%+d%+d ",   width,   height,   xoffset,   yoffset);
  startpost("%dx%d%+d%+d ", m_width, m_height, m_xoffset, m_yoffset);
  endpost();

  bool doDimen=(width!=m_width || height != m_height);
  bool doOffset=(xoffset!=m_xoffset || yoffset != m_yoffset);

  doDimen=true; doOffset=true;

  m_width=width;
  m_height=height;
  m_xoffset=xoffset;
  m_yoffset=yoffset;

  if(doDimen)dimension(m_width, m_height);
  if(doOffset)position(m_xoffset, m_yoffset);
}
void gemcocoawindow :: offsetMess(int x, int y) {
  m_xoffset = x;
  m_yoffset = y;
  move();
}
void gemcocoawindow :: borderMess(bool setting) {
  m_border=setting;
}
void gemcocoawindow :: fullscreenMess(bool on) {
#warning fullscreen
  m_fullscreen = on;
  if(m_pimpl->view) {
   if (m_fullscreen) {
    [[m_pimpl->view openGLContext] setFullScreen];
   } else {
    [[m_pimpl->view openGLContext] clearDrawable];
   }
 }
}
void gemcocoawindow :: fsaaMess(int value) {
}
void gemcocoawindow :: cursorMess(bool setting) {
  m_cursor=setting;
  if(m_cursor) {
    [NSCursor unhide];
  } else {
    [NSCursor hide];
  }
}
void gemcocoawindow :: menubarMess(int state) {
#if 0
        if (state == 0)
                SetSystemUIMode( kUIModeAllHidden, kUIOptionDisableAppleMenu | kUIOptionDisableProcessSwitch |
                                                   kUIOptionDisableSessionTerminate | kUIOptionDisableForceQuit );
        else if (state > 0)
                SetSystemUIMode( kUIModeNormal, 0 );
        else
                SetSystemUIMode( kUIModeAllHidden, kUIOptionAutoShowMenuBar );
#else
  switch(state) {
     case 0: [NSMenu setMenuBarVisible:NO]; break;
     default:
     case 1: [NSMenu setMenuBarVisible:YES]; break;
  }
#endif

}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG0(classPtr, "bang", render);
  CPPEXTERN_MSG0(classPtr, "create", createMess);
  CPPEXTERN_MSG0(classPtr, "destroy", destroyMess);

  CPPEXTERN_MSG1(classPtr, "buffer", bufferMess, t_int);
  CPPEXTERN_MSG1(classPtr, "title", titleMess, std::string);
  CPPEXTERN_MSG2(classPtr, "dimen", dimensionsMess, t_int, t_int);
  CPPEXTERN_MSG2(classPtr, "offset", offsetMess, t_int, t_int);
  CPPEXTERN_MSG1(classPtr, "border", borderMess, bool);
  CPPEXTERN_MSG1(classPtr, "fullscreen", fullscreenMess, t_int);
  CPPEXTERN_MSG1(classPtr, "FSAA", fsaaMess, t_int);
  CPPEXTERN_MSG1(classPtr, "cursor", cursorMess, bool);
  CPPEXTERN_MSG1(classPtr, "menubar", menubarMess, int);

	ProcessSerialNumber proc;
	GetCurrentProcess(&proc);
	TransformProcessType(&proc, kProcessTransformToForegroundApplication);
    SetFrontProcess(&proc);

    if(NULL==arp) {
	arp=[[NSAutoreleasePool alloc] init];
    }

        distantFuture = [NSDate distantFuture];
        distantPast = [NSDate distantPast];

    [NSApplication sharedApplication];
}
