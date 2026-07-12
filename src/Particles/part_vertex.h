/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_VERTEX_H_
#define _INCLUDE__GEM_PARTICLES_PART_VERTEX_H_

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

  part_vertex

DESCRIPTION

  Generate particles

-----------------------------------------------------------------*/
class GEM_EXTERN part_vertex : public partlib_base
{
  CPPEXTERN_HEADER(part_vertex, partlib_base);

public:

  //////////
  // Constructor
  part_vertex(t_floatarg x=0, t_floatarg y=0, t_floatarg z=0);

  //////////
  virtual void  renderParticles(GemState *state);

protected:

  //////////
  // Destructor
  virtual ~part_vertex(void);
  //////////
  void          posMess(t_float x, t_float y, t_float z);

  t_float m_x, m_y, m_z;
};

#endif  // for header file
