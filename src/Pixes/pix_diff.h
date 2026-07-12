/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_DIFF_H_
#define _INCLUDE__GEM_PIXES_PIX_DIFF_H_

#include "Base/GemPixDualObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_diff

KEYWORDS
    pix

DESCRIPTION
    Subtract two images.

-----------------------------------------------------------------*/
class GEM_EXTERN pix_diff : public GemPixDualObj
{
  CPPEXTERN_HEADER(pix_diff, GemPixDualObj);

public:

  //////////
  // Constructor
  pix_diff();

protected:

  //////////
  // Destructor
  virtual ~pix_diff();

  //////////
  // Do the processing
  virtual void    processRGBA_RGBA(imageStruct &image, imageStruct &right);
  virtual void    processYUV_YUV(imageStruct &image, imageStruct &right);
  virtual void    processGray_Gray(imageStruct &image, imageStruct &right);

#ifdef __MMX__
  virtual void    processRGBA_MMX(imageStruct &image, imageStruct &right);
  virtual void    processYUV_MMX (imageStruct &image, imageStruct &right);
  virtual void    processGray_MMX(imageStruct &image, imageStruct &right);

#endif

#ifdef __VEC__
  virtual void    processRGBA_Altivec(imageStruct &image,
                                      imageStruct &right);
  virtual void    processYUV_Altivec(imageStruct &image, imageStruct &right);
#endif
};

#endif  // for header file
