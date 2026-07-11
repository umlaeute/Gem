/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_CONE_H_
#define _INCLUDE__GEM_GEOS_CONE_H_

#include "cylinder.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    cone

    Creates a cone

KEYWORD
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN cone : public cylinder
{
  CPPEXTERN_HEADER(cone, cylinder);

public:

  //////////
  // Constructor
  cone(t_floatarg size,t_floatarg slice);

protected:

  //////////
  // Destructor
  virtual ~cone();

  //////////
  // Do the rendering
  virtual void setupParameters(void);
};

#endif  // for header file
