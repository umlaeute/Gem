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

#import <Cocoa/Cocoa.h>
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

@interface GemCocoaWindow : NSOpenGLView
{
  gemcocoawindow*gemwin;
}
@end


class GEM_EXTERN gemcocoawindow : public GemContext
{
  CPPEXTERN_HEADER(gemcocoawindow, GemContext)

    public:

  //////////
  // Constructor
  gemcocoawindow(void);

  //////////
  // Destructor
  virtual ~gemcocoawindow(void);


  // create window
  virtual bool create(void);

  // destroy window
  virtual void destroy(void);

  // check whether we have a window and if so, make it current
  virtual bool makeCurrent(void);

  void doRender(void);

  /* rendering */
  void renderMess(void);

  /* dispatch window events */
  void dispatch(void);

  /* render context (pre creation) */
  void  bufferMess(int buf);
  int         m_buffer;
  void    fsaaMess(int value);
  int         m_fsaa;

  /* window decoration (pre creation) */
  void titleMess(t_symbol* s);
  std::string     m_title;
  void borderMess(bool on);
  bool       m_border;

  /* window position/dimension (pre creation) */
  virtual void    dimensionsMess(int width, int height);
  unsigned int         m_width, m_height;

  void    fullscreenMess(bool on);
  bool              m_fullscreen;
  void        offsetMess(int x, int y);
  unsigned int      m_xoffset, m_yoffset;

  /* creation/destruction */
  void        createMess(void);
  void       destroyMess(void);

  /* post creation */
  void        cursorMess(bool on);
  bool              m_cursor;

 private:

  GemCocoaWindow*m_win;

  //////////
  // Static member functions (rendering)
  static void     renderMessCallback(void *data);

  //////////
  // Static member functions (window pre-creation)
  static void     bufferMessCallback(void *data, t_floatarg buf);
  static void     fsaaMessCallback(void *data,t_floatarg val);
  static void     titleMessCallback(void *data, t_symbol* s);
  static void     dimensionsMessCallback(void *data, t_floatarg width, t_floatarg height);
  static void     offsetMessCallback(void *data, t_floatarg x, t_floatarg y);
  static void     fullscreenMessCallback(void *data, t_floatarg on);
  static void     borderMessCallback(void *, t_floatarg state);

  //////////
  // Static member functions (window creation)
  static void     createMessCallback(void *);
  static void     destroyMessCallback(void *);

  //////////
  // Static member functions (window post-creation)
  static void     cursorMessCallback(void *, t_floatarg);

  //////////
  // Static member functions (misc)
  static void     printMessCallback(void *);

  t_clock*m_clock;
  int m_polltime;
  static void clockCallback(void*);
  void clock(void);
};

#endif    // for header file
