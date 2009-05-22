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

#include "gemwindow.h"

#include "Base/GemGL.h"
 
#ifdef __APPLE__
# include <Carbon/Carbon.h>
# include <AvailabilityMacros.h>
#endif // __APPLE__

#include "Base/GemMath.h"
#include "Base/GemMan.h"

CPPEXTERN_NEW_WITH_ONE_ARG(gemwindow, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// gemwindow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemwindow :: gemwindow(t_floatarg framespersecond)
{
  m_infoOut = outlet_new(this->x_obj, 0);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemwindow :: ~gemwindow()
{
  destroyMess();
  outlet_free(m_infoOut); m_infoOut=NULL;
}

/////////////////////////////////////////////////////////
// bangMess
//
/////////////////////////////////////////////////////////
void gemwindow :: bangMess()
{
#if 0
  if ( GemMan::windowExists() )
    {
      if(1==GemMan::m_buffer)
        GemMan::swapBuffers();
      else /* double buffered mode */
        GemMan::render(NULL);
    }
  else 
    error("no window");
#endif
}

/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void gemwindow :: renderMess()
{
#if 0
	if (GemMan::getRenderState())
		GemMan::render(NULL);
	else
		error("not in render mode");
#endif
}
/////////////////////////////////////////////////////////
// bufferMess
//
/////////////////////////////////////////////////////////
void gemwindow :: bufferMess(int buf)
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
void gemwindow :: fsaaMess(int value)
{
  m_fsaa=value;
}

/////////////////////////////////////////////////////////
// titleMess
//
/////////////////////////////////////////////////////////
void gemwindow :: titleMess(t_symbol* s)
{
  m_title = s->s_name;
}
/////////////////////////////////////////////////////////
// border
//
/////////////////////////////////////////////////////////
void gemwindow :: borderMess(bool setting)
{
  m_border=setting;
}
/////////////////////////////////////////////////////////
// dimensionsMess
//
/////////////////////////////////////////////////////////
void gemwindow :: dimensionsMess(int width, int height)
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
void gemwindow :: fullscreenMess(bool on)
{
  m_fullscreen = on;
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void gemwindow :: offsetMess(int x, int y)
{
  m_xoffset = x;
  m_yoffset = y;
}

/////////////////////////////////////////////////////////
// secondScreen
//
/////////////////////////////////////////////////////////
void gemwindow :: secondscreenMess(bool on)
{
  m_secondscreen = on;
}

/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
void gemwindow :: createMess(t_symbol* s)
{
  char* disp = NULL;
  if (s != &s_) disp = s->s_name;
#if 0

  if ( !GemMan::windowExists() )  {
    GemMan::createContext(disp);
    if ( !GemMan::createWindow(disp) )  {
      error("no window made");
      return;
    }
    GemMan::swapBuffers();	
    GemMan::swapBuffers();	
  } else
    error("window already made");
#endif
}
/////////////////////////////////////////////////////////
// destroy window
//
/////////////////////////////////////////////////////////
void gemwindow :: destroyMess(void)
{
}


/////////////////////////////////////////////////////////
// cursorMess
//
/////////////////////////////////////////////////////////
void gemwindow :: cursorMess(bool setting)
{
}

/////////////////////////////////////////////////////////
// topmostMess
//
/////////////////////////////////////////////////////////
void gemwindow :: topmostMess(bool topmost)
{
}


/////////////////////////////////////////////////////////
// menuBarMess		1 = show	-1 = hide, but autoshow
//					0 = hide, but neverShow
/////////////////////////////////////////////////////////
void gemwindow :: menuBarMess(int on)
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
void gemwindow :: obj_setupCallback(t_class *classPtr)
{
  class_addbang(classPtr, (t_method)&gemwindow::bangMessCallback);
  class_addmethod(classPtr, (t_method)&gemwindow::renderMessCallback, gensym("render"), A_NULL);

  class_addmethod(classPtr, (t_method)&gemwindow::titleMessCallback,        gensym("title"), A_DEFSYM ,A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::createMessCallback,       gensym("create"), A_DEFSYM ,A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::bufferMessCallback,       gensym("buffer"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::fullscreenMessCallback,   gensym("fullscreen"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::menuBarMessCallback,      gensym("menubar"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::secondscreenMessCallback, gensym("secondscreen"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::dimensionsMessCallback,   gensym("dimen"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::offsetMessCallback,       gensym("offset"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::cursorMessCallback,       gensym("cursor"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::destroyMessCallback,      gensym("destroy"), A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::printMessCallback,        gensym("print"), A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::borderMessCallback,       gensym("border"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::topmostMessCallback,      gensym("topmost"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemwindow::fsaaMessCallback,         gensym("FSAA"), A_FLOAT, A_NULL);
}
void gemwindow :: printMessCallback(void *)
{
  GemMan::printInfo();
}
void gemwindow :: borderMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->borderMess((int)state);
}
void gemwindow :: destroyMessCallback(void *data)
{
  GetMyClass(data)->destroyMess();
}
void gemwindow :: bangMessCallback(void *data)
{
  GetMyClass(data)->bangMess();
}
void gemwindow :: renderMessCallback(void *data)
{
  GetMyClass(data)->renderMess();
}
void gemwindow :: titleMessCallback(void *data, t_symbol* disp)
{
  GetMyClass(data)->titleMess(disp);
}
void gemwindow :: createMessCallback(void *data, t_symbol* disp)
{
  GetMyClass(data)->createMess(disp);
}
void gemwindow :: bufferMessCallback(void *data, t_floatarg buf)
{
  GetMyClass(data)->bufferMess((int)buf);
}
void gemwindow :: fullscreenMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->fullscreenMess((int)on);
}
void gemwindow :: menuBarMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->menuBarMess((int)on);
}
void gemwindow :: secondscreenMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->secondscreenMess((int)on);
}
void gemwindow :: dimensionsMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->dimensionsMess((int)width, (int)height);
}
void gemwindow :: offsetMessCallback(void *data, t_floatarg x, t_floatarg y)
{
  GetMyClass(data)->offsetMess((int)x, (int)y);
}
void gemwindow :: cursorMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->cursorMess((float)val);
}
void gemwindow :: topmostMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->topmostMess((float)val);
}
void gemwindow :: fsaaMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->fsaaMess((int) val);
}

