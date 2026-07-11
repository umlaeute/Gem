/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_TRIANGLE_H_
#define _INCLUDE__GEM_GEOS_TRIANGLE_H_

#include "Base/GemShape.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    triangle

    Creates a triangle

KEYWORDS
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN triangle : public GemShape
{
  CPPEXTERN_HEADER(triangle, GemShape);

public:

  //////////
  // Constructor
  triangle(t_floatarg size);

protected:

  //////////
  // Destructor
  virtual ~triangle();

  //////////
  // Do the renderShapeing
  virtual void    renderShape(GemState *state);
};

#endif  // for header file
