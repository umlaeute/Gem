/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Interface for the window manager

  Copyright (c) 1997-200 Mark Danks.
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_GEMEXTWIN_H_
#define INCLUDE_GEMEXTWIN_H_

#ifndef HELPSYMBOL_BASE
# define HELPSYMBOL_BASE "Gem/"
#endif

#include "Base/GemOutput.h"
#include "Base/GemWinCreate.h"


#ifdef unix
#include <GL/glx.h>
#endif


/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemextwin
    
  output to an external openGL-context

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN gemextwin : public GemOutput
{
  CPPEXTERN_HEADER(gemextwin, GemOutput)
    
    public:
    
  //////////
  // Constructor
  gemextwin();
    	
 private:

  //////////
  // Destructor
  virtual     	~gemextwin();

  // do pure virtual functions
  virtual void  makeCurrent();
  virtual void  postRender (GemState);

  //////////
  // set the current-window
  void	windowMess(char*);
#ifdef _WINDOWS
  bool  windowMess(HWND);
#endif

  //////////
  // destroy the window(context)
  void closeWindow();

  WindowInfo m_info;

 private:
  static void	windowMessCallback(void *, t_symbol*);
#ifdef _WINDOWS
  static BOOL CALLBACK windowMessCB(HWND, LPARAM);
#endif

};

#endif	// for header file
