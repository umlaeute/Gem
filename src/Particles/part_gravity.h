/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_GRAVITY_H_
#define _INCLUDE__GEM_PARTICLES_PART_GRAVITY_H_

#include "papi/papi.h"

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

  part_gravity

DESCRIPTION

  Apply gravity to particles

-----------------------------------------------------------------*/
class GEM_EXTERN part_gravity : public partlib_base
{
  CPPEXTERN_HEADER(part_gravity, partlib_base);

public:

  //////////
  // Constructor
  part_gravity(t_floatarg val1, t_floatarg val2, t_floatarg val3);

  //////////
  virtual void    renderParticles(GemState *state);

protected:

  //////////
  // Destructor
  virtual ~part_gravity(void);

  //////////
  // Vector changed
  void            vectorMess(float x, float y, float z);

  //////////
  float                   m_vector[3];
};

#endif  // for header file
