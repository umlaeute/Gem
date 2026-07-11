/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_SPHERE_H_
#define _INCLUDE__GEM_GEOS_SPHERE_H_

#include "Base/GemGluObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    sphere

    Creates a sphere

KEYWORDS
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN sphere : public GemGluObj
{
  CPPEXTERN_HEADER(sphere, GemGluObj);

public:

  //////////
  // Constructor
  sphere(t_floatarg size, t_floatarg slice=10.0);

protected:

  //////////
  // Destructor
  virtual ~sphere(void);

  //////////
  // Do the rendering
  virtual void    renderShape(GemState *state);

  virtual void    createSphere(GemState *state);

  float           *m_x;
  float           *m_y;
  float           *m_z;
  int             oldStacks, oldSlices;
  GLenum          oldDrawType;
  int             oldTexture;

};

#endif  // for header file
