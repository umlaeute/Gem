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
#include <stdio.h>


@implementation GemCocoaWindow
- (id)initWithFrame: (NSRect)frameRect parent: (gemcocoawindow*) gcw
{
  gemwin=gcw;
  if(NULL==gemwin)return NULL;

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

  if(gemwin->m_buffer==2)
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

  delete[]attr;
  
  self = [super initWithFrame:frameRect pixelFormat:nsglFormat];
  return self;
}

- (void)dealloc
{
  gemwin=NULL;
  [super dealloc];
}

- (void)prepareOpenGL
{

}

- (void)drawRect:(NSRect)rect
{
  gemwin->doRender();
}


- (BOOL)performKeyEquivalent:(NSEvent *)event
{
#if 0
  if (([event modifierFlags] & NSCommandKeyMask) && ([[event charactersIgnoringModifiers] rangeOfString:@"q"].location != NSNotFound))
    {
      [NSApp terminate:nil];
      return YES;
    }
#endif
  return [super performKeyEquivalent:event];
}
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
  if(NULL==arp)
	arp=[[NSAutoreleasePool alloc] init];

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
  if(m_win) {
    error("window already made!");
    return false;
  }
  NSRect contentRect = NSMakeRect(0.0, 0.0, m_width, m_height);
  NSWindow*window = [[NSWindow alloc] initWithContentRect:contentRect styleMask:NSTitledWindowMask backing:NSBackingStoreBuffered defer:YES];

  NSView *contentView = [window contentView];
  m_win = [[GemCocoaWindow alloc] 
	initWithFrame:[contentView bounds] parent: this];
  [contentView addSubview:m_win];

  [window center];
  [window makeKeyAndOrderFront:nil];
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
  CPPEXTERN_ADDMETHOD(gemcocoawindow, render, render);
  CPPEXTERN_ADDMETHOD(gemcocoawindow, create, create);
  CPPEXTERN_ADDMETHOD(gemcocoawindow, destroy, destroy);

  CPPEXTERN_ADDMETHOD_F(gemcocoawindow, buffer, buffer);
  CPPEXTERN_ADDMETHOD_S(gemcocoawindow, title, title);
  CPPEXTERN_ADDMETHOD_FF(gemcocoawindow, dimensions, dimen);
  CPPEXTERN_ADDMETHOD_FF(gemcocoawindow, offset, offset);
  CPPEXTERN_ADDMETHOD_F(gemcocoawindow, border, border);
  CPPEXTERN_ADDMETHOD_F(gemcocoawindow, fullscreen, fullscreen);
  CPPEXTERN_ADDMETHOD_F(gemcocoawindow, fsaa, FSAA);
  CPPEXTERN_ADDMETHOD_F(gemcocoawindow, cursor, cursor);
}

CPPEXTERN_CALLBACK(gemcocoawindow, render);
CPPEXTERN_CALLBACK(gemcocoawindow, create);
CPPEXTERN_CALLBACK(gemcocoawindow, destroy);
CPPEXTERN_CALLBACK_F(gemcocoawindow, buffer, int);
void gemcocoawindow :: titleMessCallback(void *data, t_symbol* disp)
{
  GetMyClass(data)->titleMess(disp);
}
CPPEXTERN_CALLBACK_FF(gemcocoawindow, dimensions, int);
CPPEXTERN_CALLBACK_FF(gemcocoawindow, offset, int);
CPPEXTERN_CALLBACK_F(gemcocoawindow, border, bool);
CPPEXTERN_CALLBACK_F(gemcocoawindow, fullscreen, int);
CPPEXTERN_CALLBACK_F(gemcocoawindow, fsaa, int);
CPPEXTERN_CALLBACK_F(gemcocoawindow, cursor, bool);
