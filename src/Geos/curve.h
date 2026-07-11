/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_CURVE_H_
#define _INCLUDE__GEM_GEOS_CURVE_H_

#include "Geos/polygon.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    curve

    Creates a curve

KEYWORDS
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN curve : public polygon
{
  CPPEXTERN_HEADER(curve, polygon);

public:

  //////////
  // Constructor
  curve(t_floatarg numInputs);

protected:

  //////////
  // Destructor
  virtual ~curve();

  //////////
  // Do the rendering
  virtual void    render(GemState *state);

  //////////
  // Resolution callback
  void            resolutionMess(int res);

  //////////
  // The rendering resolution
  int             m_resolution;

  GLfloat m_texCoords[4][2];

private:

  //////////
  // Static member functions
  static void     resolutionMessCallback(void *data, t_float res);
};

#endif  // for header file
