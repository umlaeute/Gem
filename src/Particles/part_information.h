/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_INFORMATION_H_
#define _INCLUDE__GEM_PARTICLES_PART_INFORMATION_H_

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

  part_information

DESCRIPTION

  Output particle information (position, color, size, velocity,...)

-----------------------------------------------------------------*/
class GEM_EXTERN part_information : public partlib_base
{
  CPPEXTERN_HEADER(part_information, partlib_base);

public:

  //////////
  // Constructor
  part_information();

  //////////
  virtual void  renderParticles(GemState *state);

protected:

  //////////
  // Destructor
  virtual ~part_information();

  // How the object should be drawn
  std::vector<float>m_position;
  std::vector<float>m_color;
  std::vector<float>m_size;
  std::vector<float>m_velocity;
  std::vector<float>m_age;

  t_outlet *out_num, *out_position, *out_color, *out_velocity, *out_size, *out_age;
  t_atom m_alist[13];
};

#endif  // for header file
