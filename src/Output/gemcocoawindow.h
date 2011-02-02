/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Interface for the window manager

  Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_GEMCOCOAWINDOW_H_
#define INCLUDE_GEMCOCOAWINDOW_H_

#include "Base/GemContext.h"


/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemcocoawindow

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

class GEM_EXTERN gemcocoawindow : public GemContext
{
  CPPEXTERN_HEADER(gemcocoawindow, GemContext)

    public:

  //////////
  // Constructor
  gemcocoawindow();

  //////////
  // Destructor
  virtual         ~gemcocoawindow();

  virtual void bang(void);

 private:

  //////////
  // Static member functions (rendering)
  static void     bangMessCallback(void *data);

  //////////
  // Static member functions (window pre-creation)
  static void     bufferMessCallback(void *data, t_floatarg buf);
  static void     fsaaMessCallback(void *data,t_floatarg val);

  static void     titleMessCallback(void *data, t_symbol* s);

  static void     dimensionsMessCallback(void *data, t_floatarg width, t_floatarg height);
  static void     offsetMessCallback(void *data, t_floatarg x, t_floatarg y);
  static void     fullscreenMessCallback(void *data, t_floatarg on);
  static void     secondscreenMessCallback(void *data, t_floatarg on);

  static void     borderMessCallback(void *, t_floatarg state);


  //////////
  // Static member functions (window creation)
  static void     createMessCallback(void *data, t_symbol* s);
  static void     destroyMessCallback(void *);

  //////////
  // Static member functions (window post-creation)
  static void     cursorMessCallback(void *data, t_floatarg val);
  static void     topmostMessCallback(void *data, t_floatarg val);
  static void     menuBarMessCallback(void *data, t_floatarg on);

  //////////
  // Static member functions (misc)
  static void     printMessCallback(void *);

  class PIMPL;
  PIMPL*m_pimpl;
};

#endif    // for header file
