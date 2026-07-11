/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_COLORSQUARE_H_
#define _INCLUDE__GEM_GEOS_COLORSQUARE_H_

#include "Base/GemShape.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    colorSquare

    Creates a colorSquare

KEYWORDS
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN colorSquare : public GemShape
{
  CPPEXTERN_HEADER(colorSquare, GemShape);

public:

  //////////
  // Constructor
  colorSquare(t_floatarg size);

protected:

  //////////
  // Destructor
  virtual ~colorSquare();

  //////////
  // Do the renderShapeing
  virtual void    renderShape(GemState *state);

  //////////
  // After renderShapeing
  virtual void    postrenderShape(GemState *state);

  //////////
  // Set the individual color vertices
  void            vertColorMess(int whichVert, float r, float g, float b);

  //////////
  // Color values
  float           m_color[4][3];

private:

  //////////
  // static member functions
  static void     vert0MessCallback(void *data, t_float r, t_float g,
                                    t_float b);
  static void     vert1MessCallback(void *data, t_float r, t_float g,
                                    t_float b);
  static void     vert2MessCallback(void *data, t_float r, t_float g,
                                    t_float b);
  static void     vert3MessCallback(void *data, t_float r, t_float g,
                                    t_float b);
};

#endif  // for header file
