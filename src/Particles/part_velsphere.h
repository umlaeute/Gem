/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_VELSPHERE_H_
#define _INCLUDE__GEM_PARTICLES_PART_VELSPHERE_H_

#include "papi/papi.h"

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS

  part_velsphere

  DESCRIPTION

  Set the velocity domain for particles

  -----------------------------------------------------------------*/
class GEM_EXTERN part_velsphere : public partlib_base
{
  CPPEXTERN_HEADER(part_velsphere, partlib_base);

public:

  //////////
  // Constructor
  part_velsphere(t_floatarg val1, t_floatarg val2, t_floatarg val,
                 t_floatarg radius);

  //////////
  virtual void  renderParticles(GemState *state);

protected:

  //////////
  // Destructor
  virtual ~part_velsphere(void);

  //////////
  void                  vectorMess(float val1, float val2, float val3);
  void                  velMess   (float num);

  //////////
  float                 m_pos[3];
  //////////
  float                 m_radius;
};

#endif  // for header file
