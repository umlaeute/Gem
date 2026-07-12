/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX__GREY_H_
#define _INCLUDE__GEM_PIXES_PIX__GREY_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_2grey

KEYWORDS
    pix

DESCRIPTION
    Change pix to greyscale

-----------------------------------------------------------------*/
class GEM_EXTERN pix_2grey : public GemPixObj
{
  CPPEXTERN_HEADER(pix_2grey, GemPixObj);

public:

  //////////
  // Constructor
  pix_2grey();

protected:

  //////////
  // Destructor
  virtual ~pix_2grey();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);
  virtual void    processYUVImage(imageStruct &image);
#ifdef __MMX__
# ifndef __APPLE__
  virtual void    processRGBAMMX(imageStruct &image);
# endif
  virtual void    processYUVMMX(imageStruct &image);
#endif
#ifdef __SSE2__
  virtual void    processYUVSSE2(imageStruct &image);
#endif
#ifdef __VEC__
  virtual void    processRGBAAltivec(imageStruct &image);
  virtual void    processYUVAltivec(imageStruct &image);
#endif

  //////////
  // Do the processing - this is a no-op
  virtual void    processGrayImage(imageStruct &image) { }
};

#endif  // for header file
