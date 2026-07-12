/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_HEAD_H_
#define _INCLUDE__GEM_PARTICLES_PART_HEAD_H_

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

  part_head

DESCRIPTION

  Starting point for a particle system

-----------------------------------------------------------------*/
struct _ParticleState;

class GEM_EXTERN part_head : public partlib_base
{
  CPPEXTERN_HEADER(part_head, partlib_base);

public:

  //////////
  // Constructor
  part_head(t_floatarg priority);

protected:

  //////////
  // Destructor
  virtual ~part_head(void);

  //////////
  virtual void    renderParticles(GemState *state);

  //////////
  // The speed of the particle system
  void            speedMess(float speed);


  //////////
  // The particle group
  int                               m_particleGroup;


  //////////
  // The speed of the object
  float             m_speed;

  _ParticleState   *m_particlestate;
};

#endif  // for header file
