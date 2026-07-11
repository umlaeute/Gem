/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_CUBE_H_
#define _INCLUDE__GEM_GEOS_CUBE_H_

#include "Base/GemShape.h"
#include <string.h>

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    cube

    Creates a cube

KEYWORDS
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN cube : public GemShape
{
  CPPEXTERN_HEADER(cube, GemShape);

public:

  //////////
  // Constructor
  cube(t_floatarg size);

protected:

  //////////
  // Destructor
  virtual ~cube();

  //////////
  // Do the renderShapeing
  virtual void    renderShape(GemState *state);
};

#endif  // for header file
