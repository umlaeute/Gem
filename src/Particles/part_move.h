/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2025, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_MOVE_H_
#define _INCLUDE__GEM_PARTICLES_PART_MOVE_H_

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

  part_move

DESCRIPTION

  Advance particles

-----------------------------------------------------------------*/
class GEM_EXTERN part_move : public partlib_base
{
  CPPEXTERN_HEADER(part_move, partlib_base);

public:

  //////////
  // Constructor
  part_move();

  //////////
  virtual void  renderParticles(GemState *state);

protected:

  //////////
  // Destructor
  virtual ~part_move();
};

#endif  // for header file
