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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef __APPLE__
#include "gemcocoawindow.h"

#include "Base/GemGL.h"
#import <Cocoa/Cocoa.h>

/*
 * http://www.cocoabuilder.com/archive/cocoa/221350-opengl-renderloop-in-cocoa.html
 */

@interface GemView : NSOpenGLView
{
  gemcocoawindow*gemwin;
}
@end

@implementation GemView 
- (id)initWithFrame:(NSRect)frameRect parent:(gemcocoawindow*)gw
{
  if(NULL==gw)return NULL;
  gemwin=gw;
  NSOpenGLPixelFormatAttribute attr[] = 
    {
      (2==gemwin->m_buffer)?NSOpenGLPFADoubleBuffer:NSOpenGLPFASingleBuffer,
      NSOpenGLPFAAccelerated,
      NSOpenGLPFAColorSize, (NSOpenGLPixelFormatAttribute) 32,
      NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute) 23,
      (NSOpenGLPixelFormatAttribute) 0
    };
  NSOpenGLPixelFormat *nsglFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attr] autorelease];
  
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
  gemwin->bang();
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



struct gemcocoawindow::PIMPL {
  GemView*view;
  gemcocoawindow*gcw;
#define error gcw->error

  int         m_buffer;
  int         m_fsaa;
  std::string m_title;
  bool        m_border;
  unsigned int      m_width, m_height;
  bool              m_fullscreen;
  int      m_xoffset, m_yoffset;
  bool              m_secondscreen;


  PIMPL(gemcocoawindow*gcw_) : 
	view(NULL),
	gcw(gcw_),
	m_buffer(2),
	m_fsaa(0),
	m_title("Gem"),
	m_border(true),
	m_width(500), m_height(500),
	m_xoffset(0), m_yoffset(0),
	m_secondscreen(true)
  {
  }

  ~PIMPL(void) {
    destroy();
    gcw=NULL;
  }
  void destroy(void) {
    if(view) [view dealloc];
    view=NULL;
  }
  bool create(void) {
    if(view) return false;
    view = [[[GemView alloc] initWithFrame:[contentView bounds]] gw:gwc];
    return (NULL!=view);
  }

/////////////////////////////////////////////////////////
// bufferMess
//
/////////////////////////////////////////////////////////
void bufferMess(int buf)
{
  switch(buf) {
  case 1: case 2:
    m_buffer=buf;
    break;
  default:
    error("can only handle single (1) or double (2) buffer");
  }
}

/////////////////////////////////////////////////////////
// fsaaMess
//
/////////////////////////////////////////////////////////
void fsaaMess(int value)
{
  m_fsaa=value;
}

/////////////////////////////////////////////////////////
// titleMess
//
/////////////////////////////////////////////////////////
void titleMess(std::string s)
{
  m_title = s
}
/////////////////////////////////////////////////////////
// border
//
/////////////////////////////////////////////////////////
void borderMess(bool setting)
{
  m_border=setting;
}
/////////////////////////////////////////////////////////
// dimensionsMess
//
/////////////////////////////////////////////////////////
void dimensionsMess(int width, int height)
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
}
/////////////////////////////////////////////////////////
// fullscreenMess
//
/////////////////////////////////////////////////////////
void fullscreenMess(bool on)
{
  m_fullscreen = on;
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void offsetMess(int x, int y)
{
  m_xoffset = x;
  m_yoffset = y;
}

/////////////////////////////////////////////////////////
// secondScreen
//
/////////////////////////////////////////////////////////
void secondscreenMess(bool on)
{
  m_secondscreen = on;
}

/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
void createMess(std::string disp)
{
}
/////////////////////////////////////////////////////////
// destroy window
//
/////////////////////////////////////////////////////////
void destroyMess(void)
{
}
/////////////////////////////////////////////////////////
// cursorMess
//
/////////////////////////////////////////////////////////
void cursorMess(bool setting)
{
}

/////////////////////////////////////////////////////////
// topmostMess
//
/////////////////////////////////////////////////////////
void topmostMess(bool topmost)
{
}


/////////////////////////////////////////////////////////
// menuBarMess		1 = show	-1 = hide, but autoshow
//					0 = hide, but neverShow
/////////////////////////////////////////////////////////
void menuBarMess(int on)
{
  /* LATER move this into the OS-specific part of GemWin */
#ifdef __APPLE__
  if (on == 0) {
    SetSystemUIMode( kUIModeAllHidden, kUIOptionDisableAppleMenu |
									   kUIOptionDisableProcessSwitch |
									   kUIOptionDisableSessionTerminate |
									   kUIOptionDisableForceQuit );
  }else if (on > 0) {
    SetSystemUIMode( kUIModeNormal, 0 );
  }else if (on < 0) {
    SetSystemUIMode( kUIModeAllHidden, kUIOptionAutoShowMenuBar );
  }
#endif
}


}

 
CPPEXTERN_NEW(gemcocoawindow)

/////////////////////////////////////////////////////////
//
// gemcocoawindow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemcocoawindow :: gemcocoawindow(void) :
  m_pimpl(new PIMPL(this))
{
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemcocoawindow :: ~gemcocoawindow()
{
  destroyMess();
}



bool gemcocoawindow :: makeCurrent(void){
  if(NULL==m_pimpl->view)
    return false;
  return GemContext::makeCurrent();
}


/////////////////////////////////////////////////////////
// bangMess
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: bang()
{
  GemContext::bang();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: obj_setupCallback(t_class *classPtr)
{
  class_addbang(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::bangMessCallback));

  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::titleMessCallback),        gensym("title"), A_DEFSYM ,A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::createMessCallback),       gensym("create"), A_DEFSYM ,A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::bufferMessCallback),       gensym("buffer"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::fullscreenMessCallback),   gensym("fullscreen"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::menuBarMessCallback),      gensym("menubar"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::secondscreenMessCallback), gensym("secondscreen"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::dimensionsMessCallback),   gensym("dimen"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::offsetMessCallback),       gensym("offset"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::cursorMessCallback),       gensym("cursor"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::destroyMessCallback),      gensym("destroy"), A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::printMessCallback),        gensym("print"), A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::borderMessCallback),       gensym("border"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::topmostMessCallback),      gensym("topmost"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::fsaaMessCallback),         gensym("FSAA"), A_FLOAT, A_NULL);
}
#define CALLPIMPL(data) if(NULL!=GetMyClass(data)->m_pimpl)GetMyClass(data)->m_pimpl
void gemcocoawindow :: printMessCallback(void *)
{
}
void gemcocoawindow :: borderMessCallback(void *data, t_floatarg state)
{
  CALLPIMPL(data)->borderMess((int)state);
}
void gemcocoawindow :: destroyMessCallback(void *data)
{
  CALLPIMPL(data)->destroyMess();
}
void gemcocoawindow :: bangMessCallback(void *data)
{
  GetMyClass(data)->bangMess();
}
void gemcocoawindow :: titleMessCallback(void *data, t_symbol* disp)
{
  CALLPIMPL(data)->titleMess(disp->s_name);
}
void gemcocoawindow :: createMessCallback(void *data, t_symbol* disp)
{
  CALLPIMPL(data)->createMess(disp->s_name);
}
void gemcocoawindow :: bufferMessCallback(void *data, t_floatarg buf)
{
  CALLPIMPL(data)->bufferMess(static_cast<int>(buf));
}
void gemcocoawindow :: fullscreenMessCallback(void *data, t_floatarg on)
{
  CALLPIMPL(data)->fullscreenMess(static_cast<int>(on));
}
void gemcocoawindow :: menuBarMessCallback(void *data, t_floatarg on)
{
  CALLPIMPL(data)->menuBarMess(static_cast<int>(on));
}
void gemcocoawindow :: secondscreenMessCallback(void *data, t_floatarg on)
{
  CALLPIMPL(data)->secondscreenMess(static_cast<int>(on));
}
void gemcocoawindow :: dimensionsMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  CALLPIMPL(data)->dimensionsMess(static_cast<int>(width), static_cast<int>(height));
}
void gemcocoawindow :: offsetMessCallback(void *data, t_floatarg x, t_floatarg y)
{
  CALLPIMPL(data)->offsetMess(static_cast<int>(x), static_cast<int>(y));
}
void gemcocoawindow :: cursorMessCallback(void *data, t_floatarg val)
{
  CALLPIMPL(data)->cursorMess(val);
}
void gemcocoawindow :: topmostMessCallback(void *data, t_floatarg val)
{
  CALLPIMPL(data)->topmostMess(val);
}
void gemcocoawindow :: fsaaMessCallback(void *data, t_floatarg val)
{
  CALLPIMPL(data)->fsaaMess(static_cast<int>(val));
}

#endif
