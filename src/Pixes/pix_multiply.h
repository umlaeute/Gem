/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_MULTIPLY_H_
#define _INCLUDE__GEM_PIXES_PIX_MULTIPLY_H_

#include "Base/GemPixDualObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_multiply

    Multiply two images together.

KEYWORDS
    pix

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN pix_multiply : public GemPixDualObj
{
  CPPEXTERN_HEADER(pix_multiply, GemPixDualObj);

public:

  //////////
  // Constructor
  pix_multiply();

protected:

  //////////
  // Destructor
  virtual ~pix_multiply();

  //////////
  // Do the processing
  virtual void    processRGBA_RGBA(imageStruct &image, imageStruct &right);

  //////////
  //  This is called whenever a new image comes through.
  //      The left image is an RGBA, the right is a gray8
  virtual void    processRGBA_Gray(imageStruct &image, imageStruct &right);

  //////////
  //  This is called whenever a new image comes through.
  //      Both images are gray8
  virtual void    processGray_Gray(imageStruct &image, imageStruct &right);

  //////////
  // Do the processing
  virtual void    processYUV_YUV(imageStruct &image, imageStruct &right);

#ifdef __MMX__
  virtual void    processRGBA_MMX(imageStruct &image, imageStruct &right);
  virtual void    processYUV_MMX(imageStruct &image, imageStruct &right);
  virtual void    processGray_MMX(imageStruct &image, imageStruct &right);
#endif

};

#endif  // for header file
