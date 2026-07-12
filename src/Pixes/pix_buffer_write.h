/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2005, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_BUFFER_WRITE_H_
#define _INCLUDE__GEM_PIXES_PIX_BUFFER_WRITE_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_buffer_write

  writes into a pix_buffer

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_buffer_write : public GemPixObj
{
  CPPEXTERN_HEADER(pix_buffer_write, GemPixObj);

public:

  //////////
  // Constructor
  pix_buffer_write(t_symbol* s);

protected:

  //////////
  // Destructor
  virtual ~pix_buffer_write();

  //////////
  // Do the processing
  virtual void  setMess(t_symbol*);
  virtual void  frameMess(int);

  virtual void render(GemState*state);


  int m_frame;
  int m_lastframe;
  t_symbol* m_bindname;

private:

  //////////
  // static member functions
  static void setMessCallback  (void*data, t_symbol*s);
  static void frameMessCallback(void*data, t_float  f);
};

#endif  // for header file
