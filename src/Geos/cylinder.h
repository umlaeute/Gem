/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_CYLINDER_H_
#define _INCLUDE__GEM_GEOS_CYLINDER_H_

#include "Base/GemGluObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
        cylinder

    Creates a cylinder

KEYWORD
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN cylinder : public GemGluObj
{
  CPPEXTERN_HEADER(cylinder, GemGluObj);

public:

  //////////
  // Constructor
  cylinder(t_floatarg size,t_floatarg slize);

protected:

  //////////
  // Destructor
  virtual ~cylinder();

  //////////
  // Do the rendering
  virtual void    renderShape(GemState *state);


  virtual void setupParameters(void);
  GLdouble baseRadius;
  GLdouble topRadius;
  GLdouble height;
  GLint    slices;
  GLint    stacks;

};

#endif  // for header file
