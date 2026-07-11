/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_SQUARE_H_
#define _INCLUDE__GEM_GEOS_SQUARE_H_

#include "Base/GemShape.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    square

    Creates a square

KEYWORDS
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN square : public GemShape
{
  CPPEXTERN_HEADER(square, GemShape);

public:

  //////////
  // Constructor
  square(t_floatarg size);

protected:

  //////////
  // Destructor
  virtual ~square();

  //////////
  // Do the rendering
  virtual void    renderShape(GemState *state);

};

#endif  // for header file
