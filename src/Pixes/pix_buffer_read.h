/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2005, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_BUFFER_READ_H_
#define _INCLUDE__GEM_PIXES_PIX_BUFFER_READ_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_buffer_read

  KEYWORDS
  pix

  DESCRIPTION
  reads from a pix_buffer

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_buffer_read : public GemPixObj
{
  CPPEXTERN_HEADER(pix_buffer_read, GemPixObj);

public:

  //////////
  // Constructor
  pix_buffer_read(t_symbol* s);

protected:

  //////////
  // Destructor
  virtual ~pix_buffer_read();

  //////////
  // set some stuff
  virtual void  setMess(t_symbol*);
  virtual void  frameMess(t_float);
  virtual void  autoMess(t_float);
  virtual void  loopMess(int);

  //////////
  // do the rendering
  virtual void render(GemState*state);

  //////////
  // Clear the dirty flag on the pixBlock
  virtual void postrender(GemState *state);

  //////////
  // update buffer at startRendering
  virtual void  startRendering();

  //////////
  // the frame to read in the next render-cycle
  t_float m_frame;
  //////////
  // an auto-increment value for proceeding to the next value
  t_float m_auto;
  //////////
  // should we loop when we reach the end of the buffer ?
  bool m_loop;

  //////////
  // the pixBlock with the current image
  pixBlock m_pixBlock;
  //////////
  // do we currently have an image ?
  bool m_haveImage;

  //////////
  // the name of the buffer we bind to
  t_symbol* m_bindname;

  //////////
  // update pix_buffer & image
  bool m_needsupdate;
  virtual void update_image();

private:
  //////////
  // static member functions
  static void setMessCallback  (void*data, t_symbol*s);
  static void frameMessCallback(void*data, t_float  f);
  static void autoMessCallback(void*data, t_float  f);
  static void loopMessCallback(void*data, t_float  f);
};

#endif  // for header file
