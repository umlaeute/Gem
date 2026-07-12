/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_POLYGON_SMOOTH_H_
#define _INCLUDE__GEM_MANIPS_POLYGON_SMOOTH_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    polygon_smooth

    Turn on polygon smoothing

DESCRIPTION

    "polygon_smoothstate" - whether to use polygon_smooth blending

-----------------------------------------------------------------*/
class GEM_EXTERN polygon_smooth : public GemBase
{
  CPPEXTERN_HEADER(polygon_smooth, GemBase);

public:

  //////////
  // Constructor
  polygon_smooth();

protected:

  //////////
  // Destructor
  virtual ~polygon_smooth();

  //////////
  // Do the rendering
  virtual void    render(GemState *state);

  //////////
  // Turn off polygon_smooth blending
  virtual void    postrender(GemState *state);

  //////////
  // polygon_smooth blending state
  int                 m_polygon_smoothState;
  //////////
  // Polygon_Smooth state changed
  void            polygon_smoothMess(int polygon_smoothState);
};

#endif  // for header file
