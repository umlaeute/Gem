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

#ifndef INCLUDE_GEMWINDOW_H_
#define INCLUDE_GEMWINDOW_H_

#ifndef HELPSYMBOL_BASE
# define HELPSYMBOL_BASE "Gem/"
#endif

#include "Base/GemOutput.h"
#include "Base/GemWinCreate.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemwindow
    
  The window manager

  DESCRIPTION
    
  Access to GemMan.
    
  "title" - set a title for the graphics window
  "create" - create a graphics window
  "destroy" - destroy the graphics window
  "dimen" - the window dimensions
  "fullscreen" - fullscreen mode
  "offset" - the window offset
  "border" - whether we want a border or not
  "cursor" - whether we want a cursor or not
  "topmost" - set the window to stay on top

  -----------------------------------------------------------------*/
class GEM_EXTERN gemwindow : public CPPExtern
{
  CPPEXTERN_HEADER(gemwindow, CPPExtern)
    
    public:
    
  //////////
  // Constructor
  gemwindow();
    	
 private:

  //////////
  // Destructor
  virtual     	~gemwindow();

  // overwrite parents functions
  virtual void  preRender (gem_control gc);
  virtual void  postRender(gem_control gc);

  // specific for this module
  virtual void 	createMess    (t_symbol* s);
  virtual void 	destroyMess   ();

  virtual void 	dimensionsMess(int width, int height);
  virtual void 	fullscreenMess(int on);
  virtual void 	offsetMess    (int x, int y);
  virtual void	borderMess    (bool on);
  virtual void 	titleMess     (t_symbol* s);

  virtual void  cursorMess    (bool on);
  virtual void	topmostMess   (bool on);


  char*        m_title;
  int          m_fullscreen;
  bool         m_border, m_cursor;
  unsigned int m_width, m_height;
  int          m_xoffset, m_yoffset;
  int          m_fsaa;
  int          m_topmost;

  // what is necessary ???
  int m_windowNumber, m_buffer;
  int m_w, m_h;

 private:

  WindowInfo gfxInfo;


  int m_windowContext;
  bool m_windowState;

  void createContext    (char* disp);
  int  createWindow     (char* disp);
  int  createConstWindow(char* disp);
  void windowInit       ();


  //////////
  // Static member functions
  static void 	createMessCallback(void *data, t_symbol* s);
  static void 	destroyMessCallback(void *);

  static void 	dimensionsMessCallback(void *data, t_floatarg width, t_floatarg height);
  static void 	offsetMessCallback(void *data, t_floatarg x, t_floatarg y);
  static void 	fullscreenMessCallback(void *data, t_floatarg on);
  static void 	borderMessCallback(void *, t_floatarg state);
  static void 	titleMessCallback(void *data, t_symbol* s);

  static void 	cursorMessCallback(void *data, t_floatarg val);	
  static void 	topmostMessCallback(void *data, t_floatarg val);	
};

#endif	// for header file
