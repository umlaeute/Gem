/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2011, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OUTPUT_GEMCOCOAWINDOW_H_
#define _INCLUDE__GEM_OUTPUT_GEMCOCOAWINDOW_H_

#import <Cocoa/Cocoa.h>
#include "Base/GemWindow.h"


class GEM_EXPORT gemcocoawindow : public GemWindow
{
  CPPEXTERN_HEADER(gemcocoawindow, GemWindow);

public:

  //////////
  // Constructor
  gemcocoawindow(void);
  virtual ~gemcocoawindow(void);

  virtual bool create(void);
  virtual void destroy(void);
  virtual void        createMess(const std::string&);
  virtual void       destroyMess(void);

  // check whether we have a window and if so, make it current
  virtual bool makeCurrent(void);
  virtual void swapBuffers(void);
  virtual void render(void);
  void renderMess(void);

  virtual void dispatch(void);
  void dispatchEvent(NSEvent*);

  virtual void              titleMess(const std::string&s);
  virtual void dimensionsMess(unsigned int width, unsigned int height);
  virtual void             offsetMess(int x, int y);
  virtual void         fullscreenMess(int on);
  virtual void             cursorMess(bool on);
  virtual void            menubarMess(int value);
  void                   move(void); // apply dimen/offset to current window
  void                  moved(void); // window dimen/offset changed

private:
  class PIMPL;
  PIMPL*m_pimpl;
  friend class PIMPL;
};

#endif    // for header file
