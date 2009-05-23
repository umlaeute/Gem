/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Interface for the window manager

  Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_GEMGLUTWINDOW_H_
#define INCLUDE_GEMGLUTWINDOW_H_

#include "Base/CPPExtern.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemglutwindow

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


class GEM_EXTERN gemglutwindow : public CPPExtern
{
  CPPEXTERN_HEADER(gemglutwindow, CPPExtern)

    public:

  //////////
  // Constructor
  gemglutwindow(void);

 private:

  typedef struct _list {
    gemglutwindow*object;
    int window;
    struct _list*next;
  } t_list;
  
  static t_list *ggw_list;
  static gemglutwindow* list_find(int win);
  static void list_add(gemglutwindow*obj,int win);
  static void list_del(int win);

  //////////
  // Destructor
  virtual ~gemglutwindow(void);

  void doRender(void);

  /* info */
  void info(t_symbol*s, t_float value);
  void info(t_symbol*s, int, t_atom*);

  void motion(int x, int y);
  void button(int id, int state);
  void key(t_symbol*id, int state);


  /* rendering */
  void   bangMess(void);
  void renderMess(void);

  /* render context (pre creation) */
  void  bufferMess(int buf);
  int         m_buffer;
  void    fsaaMess(int value);
  int         m_fsaa;

  /* window decoration (pre creation) */
  void titleMess(t_symbol* s);
  char*     m_title;
  void borderMess(bool on);
  bool       m_border;

  /* window position/dimension (pre creation) */
  void    dimensionsMess(int width, int height);
  unsigned int      m_width, m_height;
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


  // check whether we have a window and if so, make it current
  bool checkWindow(void);

  /* an outlet to send info to the patch */
  t_outlet    *m_infoOut;
 private:

  /* the GLUT window id */
  int m_window;

  //////////
  // Static member functions (rendering)
  static void     bangMessCallback(void *data);
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


  //////////
  // glut callbacks 
  static void displayCb(void);
  static void visibleCb(int);
  static void keyboardCb(unsigned char, int, int);
  static void specialCb(int, int, int);
  static void reshapeCb(int, int);
  static void mouseCb(int,int,int,int);
  static void motionCb(int,int);
  static void passivemotionCb(int, int);
  static void entryCb(int);
  static void keyboardupCb(unsigned char, int, int);
  static void specialupCb(int, int, int);
  static void joystickCb(unsigned int, int, int, int);
  static void menustateCb(int);
  static void menustatusCb(int, int, int);
  static void windowstatusCb(int);
};

#endif    // for header file
