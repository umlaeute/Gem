/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_TRANSLATE_H_
#define _INCLUDE__GEM_MANIPS_TRANSLATE_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    translate

    translate a gem object

DESCRIPTION

    Inlet for a list - "vector"
    Inlet for a float - "ft1"

    "vector" - the vector of translation
    "ft1" - the distance

-----------------------------------------------------------------*/
class GEM_EXTERN translate : public GemBase
{
  CPPEXTERN_HEADER(translate, GemBase);

public:

  //////////
  // Constructor
  translate(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~translate();

  //////////
  // When rendering occurs
  virtual void    render(GemState *state);

  //////////
  // The translation vector (x, y, z)
  float           m_vector[3];

  //////////
  // The translation distance
  float           m_distance;

  //////////
  // Distance changed
  void            distanceMess(float distance);

  //////////
  // Vector changed
  void            vectorMess(float x, float y, float z);
};

#endif  // for header file
