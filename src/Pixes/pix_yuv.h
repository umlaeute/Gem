/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_YUV_H_
#define _INCLUDE__GEM_PIXES_PIX_YUV_H_

#include "Pixes/pix_rgba.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_yuv

    Change pix from "any" color-space to GL_YUV

KEYWORDS
    pix

DESCRIPTION

-----------------------------------------------------------------*/

class GEM_EXTERN pix_yuv : public pix_rgba
{
  CPPEXTERN_HEADER(pix_yuv, GemPixObj);

public:

  //////////
  // Constructor
  pix_yuv();

protected:
};

#endif  // for header file
