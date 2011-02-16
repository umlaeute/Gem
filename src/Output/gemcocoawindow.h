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


class gemcocoawindow;
@interface GemCocoaView : NSOpenGLView
{
  @public gemcocoawindow*parent;
}
@end


class GEM_EXTERN gemcocoawindow : public GemContext
{
  CPPEXTERN_HEADER(gemcocoawindow, GemContext)

    public:

  //////////
  // Constructor
  gemcocoawindow(void);
  virtual ~gemcocoawindow(void);

  virtual bool create(void);
  virtual void destroy(void);
  void        createMess(void);
  void       destroyMess(void);

  // check whether we have a window and if so, make it current
  virtual bool makeCurrent(void);

  void renderMess(void);
  void dispatch(void);

  void             bufferMess(int buf);
  void              titleMess(t_symbol* s);
  virtual void dimensionsMess(int width, int height);
  void             offsetMess(int x, int y);
  void             borderMess(bool on);
  void         fullscreenMess(bool on);
  void               fsaaMess(int value);
  void             cursorMess(bool on);

  // window<->GemContext
  void dimension(unsigned int, unsigned int);
  void position (int, int);
  void motion(int x, int y);
  void button(int id, int state);
  void key(std::string, int, int state);

  int          m_buffer;
  int          m_fsaa;
  std::string  m_title;
  bool         m_border;
  unsigned int m_width, m_height;
  unsigned int m_xoffset, m_yoffset;
  bool         m_fullscreen;
  bool         m_cursor;

 private:

  GemCocoaView*m_win;

  //////////
  // Static member functions (window creation)
  static void     createMessCallback(void *);
  static void     destroyMessCallback(void *);
  static void     renderMessCallback(void *);
  // Static member functions (window pre-creation)
  static void     bufferMessCallback(void *data, t_floatarg buf);
  static void     titleMessCallback(void *data, t_symbol* s);
  static void     dimensionsMessCallback(void *data, t_floatarg width, t_floatarg height);
  static void     offsetMessCallback(void *data, t_floatarg x, t_floatarg y);
  static void     borderMessCallback(void *, t_floatarg state);
  static void     fullscreenMessCallback(void *data, t_floatarg on);
  static void     fsaaMessCallback(void *data,t_floatarg val);

  static void     cursorMessCallback(void *data, t_floatarg on);
};

#endif    // for header file
