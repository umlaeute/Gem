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
- (id)initWithFrame:(NSRect)frameRect parent:(gemcocoawindow*)gw
{
  if(NULL==gw)return NULL;
  gemwin=gw;

  std::vector<NSOpenGLPixelFormatAttribute>attr;
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
    attr.push_back(NSOpenGLPFADoubleBuffer);

  attr.push_back(NSOpenGLPFAAccelerated);

  attr.push_back(NSOpenGLPFAColorSize);
  attr.push_back(static_cast<NSOpenGLPixelFormatAttribute>(32));

  attr.push_back(NSOpenGLPFADepthSize);
  attr.push_back(static_cast<NSOpenGLPixelFormatAttribute>(23));

  NSOpenGLPixelFormat *nsglFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attr.data()] autorelease];
  
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
gemcocoawindow :: gemcocoawindow(void) :
  m_buffer(2),
  m_fsaa(0),
  m_title("GEM"),
  m_border(false),
  m_fullscreen(false),
  m_xoffset(-1), m_yoffset(-1),
  m_cursor(false)
{
  m_width =500;
  m_height=500;

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


  return(false);
}

/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: doRender()
{
  bang();
}
void gemcocoawindow :: renderMess()
{
  bang();
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
    error("buffer can only be '1' (single) or '2' (double) buffered");
    break;
  }
}

/////////////////////////////////////////////////////////
// fsaaMess
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: fsaaMess(int value)
{
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
// createMess
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: createMess(void)
{


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
// cursorMess
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: cursorMess(bool setting)
{
  m_cursor=setting;

}
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemcocoawindow :: obj_setupCallback(t_class *classPtr)
{
  class_addbang(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::renderMessCallback));
  
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::titleMessCallback),        gensym("title"), A_DEFSYM ,A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::createMessCallback),       gensym("create") ,A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::bufferMessCallback),       gensym("buffer"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::fullscreenMessCallback),   gensym("fullscreen"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::dimensionsMessCallback),   gensym("dimen"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::offsetMessCallback),       gensym("offset"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::cursorMessCallback),       gensym("cursor"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::destroyMessCallback),      gensym("destroy"), A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::printMessCallback),        gensym("print"), A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::borderMessCallback),       gensym("border"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemcocoawindow::fsaaMessCallback),         gensym("FSAA"), A_FLOAT, A_NULL);
}
void gemcocoawindow :: printMessCallback(void *)
{
  //  GemMan::printInfo();
}
void gemcocoawindow :: borderMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->borderMess(static_cast<int>(state));
}
void gemcocoawindow :: destroyMessCallback(void *data)
{
  GetMyClass(data)->destroyMess();
}
void gemcocoawindow :: renderMessCallback(void *data)
{
  GetMyClass(data)->renderMess();
}
void gemcocoawindow :: titleMessCallback(void *data, t_symbol* disp)
{
  GetMyClass(data)->titleMess(disp);
}
void gemcocoawindow :: createMessCallback(void *data)
{
  GetMyClass(data)->createMess();
}
void gemcocoawindow :: bufferMessCallback(void *data, t_floatarg buf)
{
  GetMyClass(data)->bufferMess(static_cast<int>(buf));
}
void gemcocoawindow :: fullscreenMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->fullscreenMess(static_cast<int>(on));
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
void gemcocoawindow :: fsaaMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->fsaaMess(static_cast<int>(val));
}
