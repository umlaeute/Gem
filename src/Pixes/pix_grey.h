/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2001, FUKUCHI Kentaro
 * SPDX-FileCopyrightText: © 2003, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_GREY_H_
#define _INCLUDE__GEM_PIXES_PIX_GREY_H_

#include "Pixes/pix_rgba.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_grey

KEYWORDS
    pix

DESCRIPTION
    age an image
    this is based on EffecTV by Fukuchi Kentauro

-----------------------------------------------------------------*/

class GEM_EXTERN pix_grey : public pix_rgba
{
  CPPEXTERN_HEADER(pix_grey, GemPixObj);

public:

  //////////
  // Constructor
  pix_grey();

protected:
};

#endif  // for header file
