/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Interface for the window manager

  Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_GEMMACWINDOW_H_
#define INCLUDE_GEMMACWINDOW_H_

#include "Base/GemContext.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemmacwindow

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


class GEM_EXTERN gemmacwindow : public GemContext
{
  CPPEXTERN_HEADER(gemmacwindow, GemContext)

    public:

  //////////
  // Constructor
  gemmacwindow(void);

 private:

  //////////
  // Destructor
  virtual ~gemmacwindow(void);


  // create window
  virtual bool create(void);

  // destroy window
  virtual void destroy(void);

  // check whether we have a window and if so, make it current
  virtual bool makeCurrent(void);
  
  virtual void swap(void);

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

  //////////
  // the real width/height of the window (set by createGemWindow())
  unsigned int m_real_w, m_real_h, m_real_x, m_real_y;

  //////////
  // Should the window be realized
#warning actuallyDisplay
  bool         m_actuallyDisplay;
  
  OSStatus eventHandler (EventRef event);

 private:

  class Info;
  Info*m_info;
  
  static bool     init(void);

  t_clock*m_clock;
  int m_polltime;
  static void clockCallback(void*);
  void clock(void);
};

#endif    // for header file
