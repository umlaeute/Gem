/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_DRAW_H_
#define _INCLUDE__GEM_PIXES_PIX_DRAW_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_draw

KEYWORDS
    pix

DESCRIPTION
    Draw a pix to a framebuffer
-----------------------------------------------------------------*/
class GEM_EXTERN pix_draw : public GemBase {
  CPPEXTERN_HEADER(pix_draw, GemBase);

public:

  //////////
  // Constructor
  pix_draw();

protected:

  //////////
  // Destructor
  virtual ~pix_draw();

  //////////
  // Do the rendering
  virtual void    render(GemState *state);
};

#endif  // for header file
