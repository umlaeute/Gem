/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_KILLSLOW_H_
#define _INCLUDE__GEM_PARTICLES_PART_KILLSLOW_H_

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

  part_killslow

DESCRIPTION

  Kill particles that are too slow

-----------------------------------------------------------------*/
class GEM_EXTERN part_killslow : public partlib_base
{
  CPPEXTERN_HEADER(part_killslow, partlib_base);

public:

  //////////
  // Constructor
  part_killslow(t_floatarg num);

  //////////
  virtual void    renderParticles(GemState *state);

protected:

  //////////
  // Destructor
  virtual ~part_killslow(void);

  //////////
  void                      speedMess(float num);

  //////////
  float                     m_killSpeed;
};

#endif  // for header file
