/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_DAMP_H_
#define _INCLUDE__GEM_PARTICLES_PART_DAMP_H_

#include "papi/papi.h"

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

  part_damp

DESCRIPTION

  Apply damping to particles

-----------------------------------------------------------------*/
class GEM_EXTERN part_damp : public partlib_base
{
  CPPEXTERN_HEADER(part_damp, partlib_base);

public:

  //////////
  // Constructor
  part_damp(t_floatarg val1, t_floatarg val2, t_floatarg val3);

  //////////
  virtual void    renderParticles(GemState *state);

  //////////
  // Destructor
  virtual ~part_damp(void);

  //////////
  // Vector changed
  void            vectorMess(float x, float y, float z);

protected:

  //////////
  float                     m_vector[3];
};

#endif  // for header file
