/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Interface for the window manager

  Copyright (c) 2009-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_OUTPUT_GEMGLFWINDOW_H_
#define _INCLUDE__GEM_OUTPUT_GEMGLFWINDOW_H_

#include "Base/GemWindow.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemglfwindow

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


class GEM_EXTERN gemglfwindow : public GemWindow
{
  CPPEXTERN_HEADER(gemglfwindow, GemWindow);

    public:

  //////////
  // Constructor
  gemglfwindow(void);

 private:

  //////////
  // Destructor
  virtual ~gemglfwindow(void);

  void doRender(void);

  /* rendering */
  void renderMess(void);

  /* render context (pre creation) */
  void  bufferMess(int buf);
  virtual void    fsaaMess(int value);

  /* window decoration (pre creation) */
  virtual void titleMess(std::string s);

  /* window position/dimension (pre creation) */
  virtual void    dimensionsMess(unsigned int width, unsigned int height);
  virtual void    fullscreenMess(int on);
  virtual void        offsetMess(int x, int y);

  /* creation/destruction */
  virtual bool create (void);
  virtual void destroy(void);

  virtual void        createMess(std::string);
  virtual void       destroyMess(void);

  /* post creation */
  virtual void        cursorMess(bool on);

  // check whether we have a window and if so, make it current
  virtual bool makeCurrent(void);
  // swap buffers
  virtual void swapBuffers(void);
  // dispatch events
  virtual void dispatch(void);

 private:

  //////////
  // glfw callbacks

  static void windowsizeCb(int width, int height);
  static void windowcloseCb(void);
  static void windowrefreshCb(void);
  static void keyCb(int key, int action);
  static void charCb(int character, int action);
  static void mousebuttonCb(int button, int action);
  static void mouseposCb(int x, int y);
  static void mousewheelCb(int pos);
};

#endif    // for header file
