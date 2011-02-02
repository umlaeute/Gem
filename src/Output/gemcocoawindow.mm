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

- (id)initWithFrame:(NSRect)frameRect (gemcocoawindow*)gw
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
  
  self = [super initWithFrame:frameRect pixelFormat:nsglFormat]
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
  PIMPL(gemcocoawindow*gcw_) : gcw(gcw_) {
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
    view = [[[GLView alloc] initWithFrame:[contentView bounds]] gw:gwc];
    return (NULL!=view);
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
void gemcocoawindow :: bangMess()
{

}

/////////////////////////////////////////////////////////
// bufferMess
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: bufferMess(int buf)
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
void gemcocoawindow :: fsaaMess(int value)
{
  m_fsaa=value;
}

/////////////////////////////////////////////////////////
// titleMess
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: titleMess(t_symbol* s)
{
  m_title = s->s_name;
}
/////////////////////////////////////////////////////////
// border
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: borderMess(bool setting)
{
  m_border=setting;
}
/////////////////////////////////////////////////////////
// dimensionsMess
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: dimensionsMess(int width, int height)
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
void gemcocoawindow :: fullscreenMess(bool on)
{
  m_fullscreen = on;
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: offsetMess(int x, int y)
{
  m_xoffset = x;
  m_yoffset = y;
}

/////////////////////////////////////////////////////////
// secondScreen
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: secondscreenMess(bool on)
{
  m_secondscreen = on;
}

/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: createMess(t_symbol* s)
{
}
/////////////////////////////////////////////////////////
// destroy window
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: destroyMess(void)
{
}


/////////////////////////////////////////////////////////
// cursorMess
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: cursorMess(bool setting)
{
}

/////////////////////////////////////////////////////////
// topmostMess
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: topmostMess(bool topmost)
{
}


/////////////////////////////////////////////////////////
// menuBarMess		1 = show	-1 = hide, but autoshow
//					0 = hide, but neverShow
/////////////////////////////////////////////////////////
void gemcocoawindow :: menuBarMess(int on)
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
void gemcocoawindow :: printMessCallback(void *)
{
  GemMan::printInfo();
}
void gemcocoawindow :: borderMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->borderMess((int)state);
}
void gemcocoawindow :: destroyMessCallback(void *data)
{
  GetMyClass(data)->destroyMess();
}
void gemcocoawindow :: bangMessCallback(void *data)
{
  GetMyClass(data)->bangMess();
}
void gemcocoawindow :: titleMessCallback(void *data, t_symbol* disp)
{
  GetMyClass(data)->titleMess(disp);
}
void gemcocoawindow :: createMessCallback(void *data, t_symbol* disp)
{
  GetMyClass(data)->createMess(disp);
}
void gemcocoawindow :: bufferMessCallback(void *data, t_floatarg buf)
{
  GetMyClass(data)->bufferMess(static_cast<int>(buf));
}
void gemcocoawindow :: fullscreenMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->fullscreenMess(static_cast<int>(on));
}
void gemcocoawindow :: menuBarMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->menuBarMess(static_cast<int>(on));
}
void gemcocoawindow :: secondscreenMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->secondscreenMess(static_cast<int>(on));
}
void gemcocoawindow :: dimensionsMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->dimensionsMess(static_cast<int>(width), static_cast<int>(height));
}
void gemcocoawindow :: offsetMessCallback(void *data, t_floatarg x, t_floatarg y)
{
  GetMyClass(data)->offsetMess(static_cast<int>(x), static_cast<int>(y));
}
void gemcocoawindow :: cursorMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->cursorMess(val);
}
void gemcocoawindow :: topmostMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->topmostMess(val);
}
void gemcocoawindow :: fsaaMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->fsaaMess(static_cast<int>(val));
}

#endif
