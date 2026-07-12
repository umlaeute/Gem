/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_INVERT_H_
#define _INCLUDE__GEM_PIXES_PIX_INVERT_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_invert

    Pixel color inversion

KEYWORDS
    pix

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN pix_invert : public GemPixObj
{
  CPPEXTERN_HEADER(pix_invert, GemPixObj);

public:

  //////////
  // Constructor
  pix_invert();

protected:

  //////////
  // Destructor
  virtual ~pix_invert();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);
  virtual void    processGrayImage(imageStruct &image);
  virtual void    processYUVImage (imageStruct &image);

#ifdef __MMX__
  virtual void    processRGBAMMX(imageStruct &image);
  virtual void    processYUVMMX (imageStruct &image);
  virtual void    processGrayMMX(imageStruct &image);
#endif
#ifdef __VEC__
  virtual void    processYUVAltivec  (imageStruct &image);
#endif
};

#endif  // for header file
