/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_SIZE_H_
#define _INCLUDE__GEM_PARTICLES_PART_SIZE_H_

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

  part_size

DESCRIPTION

  Set the initial size

-----------------------------------------------------------------*/
class GEM_EXTERN part_size : public partlib_base
{
  CPPEXTERN_HEADER(part_size, partlib_base);

public:

  //////////
  // Constructor
  part_size(int,t_atom*);

  //////////
  virtual void    renderParticles(GemState *state);

protected:

  //////////
  // Destructor
  virtual ~part_size(void);

  //////////
  void              sizeMess(t_symbol*,int,t_atom*);

  //////////
  float             m_size[3];
};

#endif  // for header file
