/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_SCALE_H_
#define _INCLUDE__GEM_MANIPS_SCALE_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    scale

    scale a gem object

DESCRIPTION

    Inlet for a list - "vector"
    Inlet for a float - "ft1"

    "vector" - the vector of scale
    "ft1" - the distance

-----------------------------------------------------------------*/
class GEM_EXTERN scale : public GemBase
{
  CPPEXTERN_HEADER(scale, GemBase);

public:

  //////////
  // Constructor
  scale(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~scale();

  //////////
  // When rendering occurs
  virtual void    render(GemState *state);

  //////////
  // The translation vector (x, y, z)
  float           m_vector[3];

  //////////
  // The scale distance
  float           m_distance;

  //////////
  // Distance changed
  void            distanceMess(float distance);

  //////////
  // Vector changed
  void            vectorMess(float x, float y, float z);
};

#endif  // for header file
