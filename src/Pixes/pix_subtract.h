/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_SUBTRACT_H_
#define _INCLUDE__GEM_PIXES_PIX_SUBTRACT_H_

#include "Base/GemPixDualObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_subtract

    Subtract two images.

KEYWORDS
    pix

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN pix_subtract : public GemPixDualObj
{
  CPPEXTERN_HEADER(pix_subtract, GemPixDualObj);

public:

  //////////
  // Constructor
  pix_subtract();

protected:

  //////////
  // Destructor
  virtual ~pix_subtract();

  //////////
  // Do the processing
  virtual void    processRGBA_RGBA(imageStruct &image, imageStruct &right);
  virtual void    processRGBA_Gray(imageStruct &image, imageStruct &right);
  virtual void    processYUV_YUV(imageStruct &image, imageStruct &right);
#ifdef __MMX__
  virtual void    processRGBA_MMX(imageStruct &image, imageStruct &right);
  virtual void    processYUV_MMX(imageStruct &image, imageStruct &right);
  virtual void    processGray_MMX(imageStruct &image, imageStruct &right);
#endif
#ifdef __VEC__
  virtual void    processYUV_Altivec(imageStruct &image, imageStruct &right);
  virtual void    processRGBA_Altivec(imageStruct &image,
                                      imageStruct &right);
#endif
  virtual void    processDualImage(imageStruct &image, imageStruct &right);
};

#endif  // for header file
