/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_ROTATE_H_
#define _INCLUDE__GEM_MANIPS_ROTATE_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    rotate

    rotate a gem object

DESCRIPTION

    Inlet for a list - "vector"
    Inlet for a float - "ft1"

    "vector" - the vector of rotation
    "ft1" - the angle

-----------------------------------------------------------------*/
class GEM_EXTERN rotate : public GemBase
{
  CPPEXTERN_HEADER(rotate, GemBase);

public:

  //////////
  // Constructor
  rotate(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~rotate();

  //////////
  // When a gem message is received
  virtual void    render(GemState *state);

  //////////
  // The rotation angle
  float           m_angle;

  //////////
  // The rotation values (x, y, z)
  float           m_vector[3];

  //////////
  // Angle changed
  void            angleMess(float angle);

  //////////
  // Vector changed
  void            vectorMess(float x, float y, float z);
};

#endif  // for header file
