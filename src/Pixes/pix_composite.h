/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_COMPOSITE_H_
#define _INCLUDE__GEM_PIXES_PIX_COMPOSITE_H_

#include "Base/GemPixDualObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_composite

KEYWORDS
    pix

DESCRIPTION
    Composite two pix images.
    Puts the second image over the top using alpha blending.

-----------------------------------------------------------------*/
class GEM_EXTERN pix_composite : public GemPixDualObj
{
  CPPEXTERN_HEADER(pix_composite, GemPixDualObj);

public:

  //////////
  // Constructor
  pix_composite();

protected:

  //////////
  // Destructor
  virtual ~pix_composite();

  //////////
  // Do the processing
  virtual void        processRGBA_RGBA(imageStruct &image,
                                       imageStruct &right);
  virtual void        processRGBA_Gray(imageStruct &image,
                                       imageStruct &right);
#ifdef __MMX__
  virtual void        processRGBA_MMX(imageStruct &image,
                                      imageStruct &right);
#endif
};

#endif  // for header file
