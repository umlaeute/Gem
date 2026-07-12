/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_FOLLOW_H_
#define _INCLUDE__GEM_PARTICLES_PART_FOLLOW_H_

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

  part_follow

DESCRIPTION

  Have the particles follow each other

-----------------------------------------------------------------*/
class GEM_EXTERN part_follow : public partlib_base
{
  CPPEXTERN_HEADER(part_follow, partlib_base);

public:

  //////////
  // Constructor
  part_follow(t_floatarg num);

protected:

  //////////
  // Destructor
  virtual ~part_follow(void);

  //////////
  virtual void    renderParticles(GemState *state);

  //////////
  void                      accelMess(float);

  //////////
  float                     m_accel;

};

#endif  // for header file
