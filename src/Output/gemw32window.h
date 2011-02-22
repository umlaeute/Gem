/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Interface for the window manager

  Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_GEMW32WINDOW_H_
#define INCLUDE_GEMW32WINDOW_H_

#include "Base/GemContext.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemw32window

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


class GEM_EXTERN gemw32window : public GemContext
{
  CPPEXTERN_HEADER(gemw32window, GemContext);

 public:

  //////////
  // Constructor
  gemw32window(void);

 private:

  //////////
  // Destructor
  virtual ~gemw32window(void);


  // create window
  virtual bool create(void);

  // destroy window
  virtual void destroy(void);

  // check whether we have a window and if so, make it current
  virtual bool makeCurrent(void);
  virtual void swapBuffers(void);

  /* dispatch window events */
  virtual void dispatch(void);

  LONG WINAPI event(UINT uMsg, WPARAM wParam, LPARAM lParam);
  void gemw32window::move(void);

  virtual void createMess(std::string);
  virtual void        dimensionsMess(unsigned int, unsigned int);
  virtual void        offsetMess(int, int);
  virtual void        cursorMess(bool on);
  virtual void        topmostMess(bool on);

 private:
  class Window;
  Window*m_win;
};

#endif    // for header file
