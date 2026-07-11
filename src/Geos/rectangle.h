/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_RECTANGLE_H_
#define _INCLUDE__GEM_GEOS_RECTANGLE_H_

#include "Base/GemShape.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    rectangle

    Creates a rectangle

KEYWORDS
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN rectangle : public GemShape
{
  CPPEXTERN_HEADER(rectangle, GemShape);

public:

  //////////
  // Constructor
  rectangle(t_floatarg width, t_floatarg height);

protected:

  //////////
  // Destructor
  virtual ~rectangle(void);

  //////////
  // The height of the object
  void            heightMess(float size);

  //////////
  // Do the rendering
  virtual void    renderShape(GemState *state);

  //////////
  // The height of the object
  GLfloat         m_height;

  //////////
  // The height inlet
  t_inlet         *m_inletH;
};

#endif  // for header file
