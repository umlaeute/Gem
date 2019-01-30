/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Interface for the window manager

  Copyright (c) 2009-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_OUTPUT_GEMSDL2WINDOW_H_
#define _INCLUDE__GEM_OUTPUT_GEMSDL2WINDOW_H_

#include "Base/GemWindow.h"
#include <SDL.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemsdl2window

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


class GEM_EXPORT gemsdl2window : public GemWindow
{
  CPPEXTERN_HEADER(gemsdl2window, GemWindow);

public:

  //////////
  // Constructor
  gemsdl2window(void);

private:

  //////////
  // Destructor
  virtual ~gemsdl2window(void);

  void doRender(void);

  /* rendering */
  void renderMess(void);

  /* render context (pre creation) */
  void  bufferMess(int buf);

  /* window decoration (pre creation) */
  virtual void titleMess(const std::string&s);

  /* window position/dimension (pre creation) */
  virtual void dimensionsMess(unsigned int width, unsigned int height);
  virtual void offsetMess(int x, int y);
  virtual void fullscreenMess(int on);
  virtual void borderMess(bool on);
  virtual void cursorMess(bool on);
  virtual void topmostMess(bool on);
  virtual void grabmouseMess(bool on);
  virtual void relativemouseMess(bool on);
  virtual void opacityMess(float);
  virtual void userGestureMess(t_symbol*s, int, t_atom*);

  /* creation/destruction */
  virtual bool create(void);
  virtual void destroy(void);

  virtual void createMess(const std::string&);
  virtual void destroyMess(void);

  /* post creation */
  void menuMess(void);
  void addMenuMess(t_symbol*, int, t_atom*);


  // check whether we have a window and if so, make it current
  virtual bool makeCurrent(void);
  // swap buffers
  virtual void swapBuffers(void);
  // dispatch events
  virtual void dispatch(void);
  virtual void dispatch(SDL_Event&event);

private:

  /* the SDL surface */
  SDL_Window*m_window;
  SDL_GLContext m_context;

  /* hints how to create the window */
  int m_videoFlags;

  bool m_grabmouse;
  bool m_relativemouse;
  float m_opacity;
};

#endif    // for header file
