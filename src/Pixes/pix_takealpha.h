/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_TAKEALPHA_H_
#define _INCLUDE__GEM_PIXES_PIX_TAKEALPHA_H_

#include "Base/GemPixDualObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_takealpha

    Add two images together.

KEYWORDS
    pix

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN pix_takealpha : public GemPixDualObj
{
  CPPEXTERN_HEADER(pix_takealpha, GemPixDualObj);

public:

  //////////
  // Constructor
  pix_takealpha();

protected:

  //////////
  // Destructor
  virtual ~pix_takealpha();

  //////////
  // Do the processing
  virtual void    processRGBA_RGBA(imageStruct &image, imageStruct &right);

  //////////
  // Do the processing
  virtual void    processRGBA_Gray(imageStruct &image, imageStruct &right);

  // MMX isn't really faster here
};

#endif  // for header file
