/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2009, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OUTPUT_GEMSDLWINDOW_H_
#define _INCLUDE__GEM_OUTPUT_GEMSDLWINDOW_H_

#include "Base/GemWindow.h"
#include <SDL.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemsdlwindow

  The window manager (SDL-1.2)

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


class GEM_EXPORT gemsdlwindow : public GemWindow
{
  CPPEXTERN_HEADER(gemsdlwindow, GemWindow);

public:

  //////////
  // Constructor
  gemsdlwindow(void);

private:

  //////////
  // Destructor
  virtual ~gemsdlwindow(void);

  /* rendering */
  void renderMess(void);

  /* render context (pre creation) */
  void  bufferMess(int buf);

  /* window decoration (pre creation) */
  virtual void titleMess(const std::string&s);

  /* window position/dimension (pre creation) */
  virtual void    dimensionsMess(unsigned int width, unsigned int height);
  virtual void    fullscreenMess(int on);

  /* creation/destruction */
  virtual bool        create(void);
  virtual void destroy(void);

  virtual void        createMess(const std::string&);
  virtual void       destroyMess(void);

  /* post creation */
  void menuMess(void);
  void addMenuMess(t_symbol*, int, t_atom*);


  // check whether we have a window and if so, make it current
  virtual bool makeCurrent(void);
  // swap buffers
  virtual void swapBuffers(void);
  // dispatch events
  virtual void dispatch(void);

private:

  /* hints how to create the window */
  int m_videoFlags;

  /* bits per pixel */
  unsigned int m_bpp;
};

#endif    // for header file
