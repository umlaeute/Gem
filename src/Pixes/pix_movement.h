/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2000, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------
pix_movement

  set the Alpha-channel of a pixBlock depending on the difference between the current and the last pixBlock

  2803:forum::für::umläute:2000
  0409:forum::für::umläute:2000
  1801:forum::für::umläute:2001  added the second mode

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_MOVEMENT_H_
#define _INCLUDE__GEM_PIXES_PIX_MOVEMENT_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
pix_movement

  KEYWORDS
  pix

        DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN pix_movement : public GemPixObj
{
  CPPEXTERN_HEADER(pix_movement, GemPixObj);

public:

  //////////
  // Constructor
  pix_movement(t_floatarg f);

protected:

  //////////
  // Destructor
  virtual ~pix_movement();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);
  virtual void    processYUVImage(imageStruct &image);
  virtual void    processGrayImage(imageStruct &image);
#ifdef __MMX__
  virtual void    processGrayMMX(imageStruct &image);
#endif

#ifdef __VEC__
  virtual void    processYUVAltivec(imageStruct &image);
#endif

  //////////
  // the last image (grey-scale)
  imageStruct    buffer;
  imageStruct    buffer2; // (the difference image for greyscales)
  //////////
  // the movement-mode
  unsigned char  threshold;

  //////////
  // the methods
  static void threshMessCallback(void *data, t_float fthresh);
};

#endif  // for header file
