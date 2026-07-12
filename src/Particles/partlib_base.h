/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PARTLIB_BASE_H_
#define _INCLUDE__GEM_PARTICLES_PARTLIB_BASE_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

  partlib_base

DESCRIPTION

  Base class of all the part_* objects

-----------------------------------------------------------------*/
class GEM_EXTERN partlib_base : public GemBase
{
public:

  //////////
  // Constructor
  partlib_base(void);

  //////////
  // Destructor
  virtual ~partlib_base();

  //////////
  virtual void  render(GemState *);

  virtual void renderParticles(GemState*)=0;

protected:
  float m_tickTime;

};

#endif  // for header file
