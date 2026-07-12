/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, Chris Clepper and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_ROLL_H_
#define _INCLUDE__GEM_PIXES_PIX_ROLL_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_roll



KEYWORDS
    pix
    yuv

DESCRIPTION

   template for yuv_ objects

-----------------------------------------------------------------*/

class GEM_EXTERN pix_roll : public GemPixObj
{
  CPPEXTERN_HEADER(pix_roll, GemPixObj);

public:

  //////////
  // Constructor
  pix_roll();

protected:

  //////////
  // Destructor
  virtual ~pix_roll();

  //////////
  // Do the processing
  virtual void  processImage(imageStruct &image);
  virtual void  processRGBAImage(imageStruct &image);

  //////////
  // Do the YUV processing
  virtual void  processYUVImage(imageStruct &image);

  unsigned char  *saved;
  int           m_vroll,m_axis;
  int           m_blurH,m_blurW,m_blurSize,m_blurBpp;
  t_inlet         *inletBlur;

private:

  //////////
  // Static member functions

  static void rollCallback       (void *data, t_float value);
  static void axisCallback       (void *data, t_float value);
};

#endif
