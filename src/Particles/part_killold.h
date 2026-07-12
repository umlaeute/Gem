/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_KILLOLD_H_
#define _INCLUDE__GEM_PARTICLES_PART_KILLOLD_H_

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

  part_killold

DESCRIPTION

  Kill particles that are past a certain time

-----------------------------------------------------------------*/
class GEM_EXTERN part_killold : public partlib_base
{
  CPPEXTERN_HEADER(part_killold, partlib_base);

public:

  //////////
  // Constructor
  part_killold(t_floatarg num);

  //////////
  virtual void    renderParticles(GemState *state);

protected:

  //////////
  // Destructor
  virtual ~part_killold(void);

  //////////
  void                      ageMess(float age);

  //////////
  float                     m_killAge;
};

#endif  // for header file
