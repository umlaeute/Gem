/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_DRAW_H_
#define _INCLUDE__GEM_PARTICLES_PART_DRAW_H_

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

  part_draw

DESCRIPTION

  Draw a particle group

-----------------------------------------------------------------*/
class GEM_EXTERN part_draw : public partlib_base
{
  CPPEXTERN_HEADER(part_draw, partlib_base);

public:

  //////////
  // Constructor
  part_draw(void);

  //////////
  virtual void    renderParticles(GemState *state);

  //////////
  // Destructor
  virtual ~part_draw(void);

  //////////
  // How the object should be drawn
  void                      typeMess(t_symbol*,int,t_atom*);

protected:

  //////////
  int                               m_drawType;
};

#endif  // for header file
