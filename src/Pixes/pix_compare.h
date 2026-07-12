/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, Chris Clepper and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_COMPARE_H_
#define _INCLUDE__GEM_PIXES_PIX_COMPARE_H_

#include "Base/GemPixDualObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_compare

KEYWORDS
    pix
    yuv

DESCRIPTION

   compares the pixel values of two images

-----------------------------------------------------------------*/

class GEM_EXTERN pix_compare : public GemPixDualObj {
  CPPEXTERN_HEADER(pix_compare, GemPixDualObj);

public:

  //////////
  // Constructor
  pix_compare(t_floatarg f=0);

protected:

  //////////
  // Destructor
  virtual ~pix_compare();


  //////////
  // Do the processing
  virtual void    processRGBA_RGBA(imageStruct &image, imageStruct &right);
  virtual void    processYUV_YUV  (imageStruct &image, imageStruct &right);
  virtual void    processGray_Gray(imageStruct &image, imageStruct &right);

#ifdef __MMX__
  virtual void    processYUV_MMX (imageStruct &image, imageStruct &right);
  virtual void    processGray_MMX(imageStruct &image, imageStruct &right);
#endif
#ifdef __VEC__
  //////////
  // Do the Altivec processing
  virtual void    processYUV_Altivec(imageStruct &image, imageStruct &right);
#endif

  int m_direction;

private:

  //////////
  // Static member functions
  static void directionCallback(void *data, t_float state);
};

#endif
