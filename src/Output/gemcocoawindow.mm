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

@implementation GemCocoaView
#if 0
- (id)initWithFrame: (NSRect)frameRect parent: (gemcocoawindow*) gcw
{

  NSOpenGLPixelFormat *nsglFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attr] autorelease];

  delete[]attr;
  
  self = [super initWithFrame:frameRect pixelFormat:nsglFormat];
  return self;
}

- (void)dealloc
{
  parent=NULL;
  [super dealloc];
}

- (void)prepareOpenGL
{

}
#else
- (id) initWithFrame: (NSRect)rect {
  [super initWithFrame: rect];
  return self;
}

#endif


- (void)drawRect:(NSRect)rect
{
  parent->renderMess();
}
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


@end










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
  m_buffer(2),
  m_fsaa(0),
  m_title("GEM"),
  m_border(false),
  m_width(500), m_height(500),
  m_xoffset(0), m_yoffset(0),
  m_fullscreen(false),
  m_cursor(false),
  m_win(NULL)
{
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemcocoawindow :: ~gemcocoawindow()
{
  destroyMess();
  if(m_win)
    [m_win dealloc];
  m_win=NULL;
}

bool gemcocoawindow :: makeCurrent(){
  return(false);
}
void gemcocoawindow :: swapBuffers() {
}
void gemcocoawindow :: dispatch() {
}
void gemcocoawindow :: renderMess() {
  bang();
}


/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
bool gemcocoawindow :: create(void)
{
  std::cerr << "create: " << (void*)m_win << std::endl;
  if(m_win) {
    error("window already made!");
    return false;
  }
  NSWindow*window=NULL;
  DEBUGLINE;
  NSRect contentRect = NSMakeRect(0, 0, m_width, m_height);
  DEBUGLINE;
  // window = [[NSWindow alloc] initWithContentRect:contentRect styleMask:m_border?(NSTitledWindowMask|NSMiniaturizableWindowMask|NSClosableWindowMask):NSBorderlessWindowMask backing:NSBackingStoreBuffered defer:YES];
  DEBUGLINE;

  window = [NSWindow alloc];
  DEBUGLINE;
  [ window initWithContentRect:contentRect styleMask:m_border?(NSTitledWindowMask|NSMiniaturizableWindowMask|NSClosableWindowMask):NSBorderlessWindowMask backing:NSBackingStoreBuffered defer:YES];

  DEBUGLINE;
  NSView *contentView = [window contentView];
  DEBUGLINE;
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
  NSOpenGLPixelFormatAttribute*attr = new NSOpenGLPixelFormatAttribute[attrvec.size()];
  int i=0;
  for(i=0; i<attrvec.size(); i++) {
    attr[i]=attrvec[i];
  }
  NSOpenGLPixelFormat *nsglFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attr] autorelease];
  DEBUGLINE;
  m_win = [[GemCocoaView alloc] initWithFrame:[contentView bounds] pixelFormat:nsglFormat];
  DEBUGLINE;
  m_win->parent=this;
  [contentView addSubview:m_win];
  DEBUGLINE;
  [window center];
  DEBUGLINE;
  [window makeKeyAndOrderFront:nil];
  DEBUGLINE;
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
void gemcocoawindow :: titleMess(t_symbol* s) {
  m_title = s->s_name;
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
}
void gemcocoawindow :: offsetMess(int x, int y) {
  m_xoffset = x;
  m_yoffset = y;
}
void gemcocoawindow :: borderMess(bool setting) {
  m_border=setting;
}
void gemcocoawindow :: fullscreenMess(bool on) {
  m_fullscreen = on;
}
void gemcocoawindow :: fsaaMess(int value) {
}
void gemcocoawindow :: cursorMess(bool setting) {
  m_cursor=setting;
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG0(classPtr, "render", renderMess);
  CPPEXTERN_MSG0(classPtr, "create", createMess);
  CPPEXTERN_MSG0(classPtr, "destroy", destroyMess);

  CPPEXTERN_MSG1(classPtr, "buffer", bufferMess, t_int);
  CPPEXTERN_MSG1(classPtr, "title", titleMess, t_symbol*);
  CPPEXTERN_MSG2(classPtr, "dimen", dimensionsMess, t_int, t_int);
  CPPEXTERN_MSG2(classPtr, "offset", offsetMess, t_int, t_int);
  CPPEXTERN_MSG1(classPtr, "border", borderMess, bool);
  CPPEXTERN_MSG1(classPtr, "fullscreen", fullscreenMess, t_int);
  CPPEXTERN_MSG1(classPtr, "FSAA", fsaaMess, t_int);
  CPPEXTERN_MSG1(classPtr, "cursor", cursorMess, bool);

DEBUGLINE;

	ProcessSerialNumber proc;
  DEBUGLINE;
	GetCurrentProcess(&proc);
  DEBUGLINE;
	TransformProcessType(&proc, kProcessTransformToForegroundApplication);
  DEBUGLINE;
    if(NULL==arp)
	arp=[[NSAutoreleasePool alloc] init];

  DEBUGLINE;
SetFrontProcess(&proc);


  DEBUGLINE;
	[NSApplication sharedApplication];
  

  DEBUGLINE;
}
