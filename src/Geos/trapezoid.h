/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2008, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_TRAPEZOID_H_
#define _INCLUDE__GEM_GEOS_TRAPEZOID_H_

#include "Base/GemShape.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    trapezoid

    Creates a trapezoid

KEYWORDS
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN trapezoid : public GemShape
{
  CPPEXTERN_HEADER(trapezoid, GemShape);

public:

  //////////
  // Constructor
  trapezoid(t_floatarg size, t_floatarg scale);

protected:

  //////////
  // Destructor
  virtual ~trapezoid(void);



  //////////
  // length of the upper side of the trapezoid
  t_inlet *m_topinlet;
  t_float   m_top;
  void    toplengthMess(t_float len);

  //////////
  // a texture scale factor
  t_float   m_scale_texcoord;
  void    texscaleMess(t_float scale);

  //////////
  // Do the renderShapeing
  virtual void    renderShape(GemState *state);
};

#endif  // for header file
