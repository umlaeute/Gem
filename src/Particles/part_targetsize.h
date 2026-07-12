/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_TARGETSIZE_H_
#define _INCLUDE__GEM_PARTICLES_PART_TARGETSIZE_H_

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

  part_targetsize

DESCRIPTION

  Set the target color for a particle system

-----------------------------------------------------------------*/
class GEM_EXTERN part_targetsize : public partlib_base
{
  CPPEXTERN_HEADER(part_targetsize, partlib_base);

public:

  //////////
  // Constructor
  part_targetsize(t_floatarg size, t_floatarg scale);

  //////////
  virtual void    renderParticles(GemState *state);

protected:

  //////////
  // Destructor
  virtual ~part_targetsize();

  //////////
  // The scale factor angle
  float           m_scale[3];

  //////////
  // The target size
  float           m_size[3];

  //////////
  // Scale changed
  void            scaleMess(float x, float y, float z);

  //////////
  // Size changed
  void            sizeMess(float sizex, float sizey, float sizez);

private:

  //////////
  // static member functions
  static void     scaleMessCallback(void *data, t_symbol*, int, t_atom*);
  static void     sizeMessCallback (void *data, t_symbol*, int, t_atom*);
};

#endif  // for header file
