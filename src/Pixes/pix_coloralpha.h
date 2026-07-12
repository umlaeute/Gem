/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_COLORALPHA_H_
#define _INCLUDE__GEM_PIXES_PIX_COLORALPHA_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_coloralpha

KEYWORDS
    pix

DESCRIPTION

    Set the alpha values of a pix

-----------------------------------------------------------------*/
class GEM_EXTERN pix_coloralpha : public GemPixObj
{
  CPPEXTERN_HEADER(pix_coloralpha, GemPixObj);

public:

  //////////
  // Constructor
  pix_coloralpha();

protected:

  //////////
  // Destructor
  virtual ~pix_coloralpha();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);
};

#endif  // for header file
