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

#ifndef INCLUDE_GEMCONTROL_H_
#define INCLUDE_GEMCONTROL_H_

#ifndef HELPSYMBOL_BASE
# define HELPSYMBOL_BASE "Gem/"
#endif

#include "Base/CPPExtern.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemcontrol
    
  The interface to GemMan

  DESCRIPTION
    
  Access to GemMan.

  what was formerly known as [gemwin] is now split into 2 objects:
  - [gemcontrol] :the core render-trigger 
  - [gemwindow] (or any child of GemOutput): the display-module

  idea: each render-cycle all the GemOutput's that are connected to the [gemcontrol] receive a 
  pointer to the actual rendering function; they just setup their context and call the rendering-function

  "1/0" - turn on/off rendering

  the rest - is passed through to [gemwindow] (cool for compatibility)

  -----------------------------------------------------------------*/
class GEM_EXTERN gemcontrol : public CPPExtern
{
  CPPEXTERN_HEADER(gemcontrol, CPPExtern)
    
    public:
    
  //////////
  // Constructor
  gemcontrol(t_floatarg fps);
    	
 protected:

  //////////
  // Destructor
  virtual     	~gemcontrol();

  void intMess(int state);


 public:
  void render();

 private:

  t_outlet *m_outlet, *m_fpsOut;

  static void	floatMessCallback(void*, t_float);
  static void   frameMessCallback(void*, t_symbol*,int,t_atom*);
  void frameMess(int, t_atom*);

  static void	profileMessCallback(void*, t_float);

  // the fallback for every message we don't understand
  void thru(t_symbol*, int, t_atom*);
  static void	anyMessCallback(void*, t_symbol*, int, t_atom*);
};

#endif	// for header file
