/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_MASK_H_
#define _INCLUDE__GEM_PIXES_PIX_MASK_H_

#include "Base/GemPixDualObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_mask

KEYWORDS
    pix

DESCRIPTION
    Do a blue screen with pix images.

-----------------------------------------------------------------*/
class GEM_EXTERN pix_mask : public GemPixDualObj
{
  CPPEXTERN_HEADER(pix_mask, GemPixDualObj);

public:

  //////////
  // Constructor
  pix_mask();

protected:

  //////////
  // Destructor
  virtual ~pix_mask();

  //////////
  // Do the processing
  virtual void    processRGBA_RGBA(imageStruct &image, imageStruct &right);

  //////////
  //  This is called whenever a new image comes through.
  //      The left image is an RGBA, the right is a gray8
  virtual void    processRGBA_Gray(imageStruct &image, imageStruct &right);

  virtual void    processRGBA_YUV(imageStruct &image, imageStruct &right);
};

#endif  // for header file
