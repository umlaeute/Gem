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
  @public NSWindow*window;
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
  virtual void swapBuffers(void);
  virtual void render(void);
  void renderMess(void);

  virtual void dispatch(void);
  void dispatchEvent(NSEvent*);

  void             bufferMess(int buf);
  void              titleMess(std::string s);
  virtual void dimensionsMess(int width, int height);
  void             offsetMess(int x, int y);
  void             borderMess(bool on);
  void         fullscreenMess(bool on);
  void               fsaaMess(int value);
  void             cursorMess(bool on);
  void            menubarMess(int value);
  void		         move(void);

  // window<->GemContext
  void dimension(unsigned int, unsigned int);

  int          m_fsaa;
  std::string  m_title;
  bool         m_border;
  unsigned int m_xoffset, m_yoffset;
  bool         m_fullscreen;
  bool         m_cursor;

 private:
  NSUInteger m_modifierFlags;

  GemCocoaView*m_win;
};

#endif    // for header file
