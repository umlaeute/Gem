/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_DELAY_H_
#define _INCLUDE__GEM_PIXES_PIX_DELAY_H_

#include "Base/GemPixObj.h"

#define DEFAULT_MAX_FRAMES 256
/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_delay

  KEYWORDS
  pix

  DESCRIPTION
  delay line for pixes
  -----------------------------------------------------------------*/
class GEM_EXTERN pix_delay : public GemPixObj {
  CPPEXTERN_HEADER(pix_delay, GemPixObj);

public:

  //////////
  // Constructor
  pix_delay(t_float &f);

protected:

  //////////
  // Destructor
  virtual ~pix_delay();

  //////////
  // Do the processing
  virtual void  processImage(imageStruct &image);

  imageStruct    myImage;

  virtual void  delayMess(int frames);

  int m_maxframes, m_curframe;
  int m_frame;


private:

  //////////
  // static member functions
  static void delayMessCallback(void *data, t_float);
};

#endif  // for header file
