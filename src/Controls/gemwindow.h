/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Interface for the window manager

  Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_GEMWINDOW_H_
#define INCLUDE_GEMWINDOW_H_

#include "Base/CPPExtern.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemwindow
    
  The window manager

  DESCRIPTION
    
  Access to GemMan.
    
  "bang"  - swap the buffers
  "render" - render a frame now

  "create" - create a graphics window
  "destroy" - destroy the graphics window


  "buffer" - single or double buffered
  "fsaa" - full screen anti-aliasing

  "title" - set a title for the graphics window
  "border" - whether we want a border as decoration or not

  "dimen" - the window dimensions
  "fullscreen" - fullscreen mode
  "offset" - the window offset
  "secondscreen" - render to the secondscreen (auto-offset)

  "cursor" - whether we want a cursor or not
  "menubar" - hide notorious menubars
  "topmost" - set the window to stay on top

  -----------------------------------------------------------------*/
class GEM_EXTERN gemwindow : public CPPExtern
{
  CPPEXTERN_HEADER(gemwindow, CPPExtern)
    
    public:
    
  //////////
  // Constructor
  gemwindow(t_floatarg framespersecond);
    	
 private:

  //////////
  // Destructor
  virtual     	~gemwindow();

  /* rendering */
  void 	    	bangMess();
  void			  renderMess();

  /* render context (pre creation) */
  void 	    	bufferMess(int buf);
  int         m_buffer;
  void        fsaaMess(int value);
  int         m_fsaa;

  /* window decoration (pre creation) */
  void 	    	titleMess(t_symbol* s);
  char*       m_title;
  void 	    	borderMess(int on);
  bool        m_border;

  /* window position/dimension (pre creation) */
  void 	    	dimensionsMess(int width, int height);
  unsigned int m_width, m_height;
  void 	    	fullscreenMess(int on);
  bool        m_fullscreen;
  void 	    	offsetMess(int x, int y);
  unsigned int m_xoffset, m_yoffset;
  void 	    	secondscreenMess(int on);
  bool        m_secondscreen;

  /* creation/destruction */
  void 	    	createMess(t_symbol* s);
  void 	    	destroyMess(void);

  /* post creation */
  void        cursorMess(int on);
  void		    topmostMess(int on);
  void			  menuBarMess(int on);

  /* an outlet to send info to the patch */
  t_outlet	*m_infoOut;
 private:
    
  //////////
  // Static member functions (rendering)
  static void 	bangMessCallback(void *data);
  static void 	renderMessCallback(void *data);

  //////////
  // Static member functions (window pre-creation)
  static void 	bufferMessCallback(void *data, t_floatarg buf);
  static void 	fsaaMessCallback(void *data,t_floatarg val);

  static void 	titleMessCallback(void *data, t_symbol* s);

  static void 	dimensionsMessCallback(void *data, t_floatarg width, t_floatarg height);
  static void 	offsetMessCallback(void *data, t_floatarg x, t_floatarg y);
  static void 	fullscreenMessCallback(void *data, t_floatarg on);
  static void 	secondscreenMessCallback(void *data, t_floatarg on);

  static void 	borderMessCallback(void *, t_floatarg state);


  //////////
  // Static member functions (window creation)
  static void 	createMessCallback(void *data, t_symbol* s);
  static void 	destroyMessCallback(void *);

  //////////
  // Static member functions (window post-creation)
  static void 	cursorMessCallback(void *data, t_floatarg val);	
  static void 	topmostMessCallback(void *data, t_floatarg val);	
  static void 	menuBarMessCallback(void *data, t_floatarg on);

  //////////
  // Static member functions (misc)
  static void 	printMessCallback(void *);
};

#endif	// for header file
