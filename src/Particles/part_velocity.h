/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_VELOCITY_H_
#define _INCLUDE__GEM_PARTICLES_PART_VELOCITY_H_

#include "papi/papi.h"

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS

  part_velocity

  DESCRIPTION

  Set the velocity domain for particles

  -----------------------------------------------------------------*/
class GEM_EXTERN part_velocity : public partlib_base
{
  CPPEXTERN_HEADER(part_velocity, partlib_base);

public:

  //////////
  // Constructor
  part_velocity(int,t_atom*);

  //////////
  virtual void  renderParticles(GemState *state);

protected:

  //////////
  // Destructor
  virtual ~part_velocity(void);

  //////////
  void          vectorMess(t_symbol*s, int argc, t_atom*argv);
  void          domainMess(const std::string&);

  //////////
  float         m_arg[9];
  PDomainEnum   m_domain;
};

#endif  // for header file
