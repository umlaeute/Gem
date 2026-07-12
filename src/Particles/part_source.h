/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_SOURCE_H_
#define _INCLUDE__GEM_PARTICLES_PART_SOURCE_H_

#include "Particles/partlib_base.h"
#include "papi/papi.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS

  part_source

  DESCRIPTION

  Generate particles

  -----------------------------------------------------------------*/
class GEM_EXTERN part_source : public partlib_base
{
  CPPEXTERN_HEADER(part_source, partlib_base);

public:

  //////////
  // Constructor
  part_source(t_floatarg num);

  //////////
  virtual void  renderParticles(GemState *state);

protected:

  //////////
  // Destructor
  virtual ~part_source(void);

  //////////
  // Number to add
  void                  numberMess(float num);
  void      domainMess(const std::string&s);
  void                  vectorMess(t_symbol*s, int argc, t_atom*argv);

  t_float                       m_numberToAdd;

  //////////
  // vel.domain
  PDomainEnum           m_domain;
  float                 m_arg[9];
};

#endif  // for header file
