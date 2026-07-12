/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_SINK_H_
#define _INCLUDE__GEM_PARTICLES_PART_SINK_H_

#include "Particles/partlib_base.h"
#include "papi/papi.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS

  part_sink

  DESCRIPTION

  Generate particles

  -----------------------------------------------------------------*/
class GEM_EXTERN part_sink : public partlib_base
{
  CPPEXTERN_HEADER(part_sink, partlib_base);

public:

  //////////
  // Constructor
  part_sink(int argc, t_atom*argv);

  //////////
  virtual void  renderParticles(GemState *state);

protected:

  //////////
  // Destructor
  virtual ~part_sink(void);

  //////////
  void                  killMess(int num);
  void      domainMess(const std::string&s);
  void                  vectorMess(t_symbol*,int argc, t_atom*argv);

  bool                  m_kill;
  PDomainEnum           m_domain;
  float                 m_arg[9];
};

#endif  // for header file
