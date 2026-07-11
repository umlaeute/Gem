/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_CIRCLE_H_
#define _INCLUDE__GEM_GEOS_CIRCLE_H_

#include "Base/GemShape.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    circle

    Creates a circle

KEYWORDS
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN circle : public GemShape
{
  CPPEXTERN_HEADER(circle, GemShape);

public:

  //////////
  // Constructor
  circle(t_floatarg size);

protected:

  //////////
  // Destructor
  virtual ~circle();

  //////////
  // Do the renderShapeing
  virtual void    renderShape(GemState *state);

  //////////
  // cos lookup table
  static GLfloat *m_cos;

  //////////
  // sin lookup table
  static GLfloat *m_sin;
};

#endif  // for header file
