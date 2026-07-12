/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_TARGETCOLOR_H_
#define _INCLUDE__GEM_PARTICLES_PART_TARGETCOLOR_H_

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

  part_targetcolor

DESCRIPTION

  Set the target color for a particle system

-----------------------------------------------------------------*/
class GEM_EXTERN part_targetcolor : public partlib_base
{
  CPPEXTERN_HEADER(part_targetcolor, partlib_base);

public:

  //////////
  // Constructor
  part_targetcolor(int argc, t_atom *argv);

  //////////
  virtual void    renderParticles(GemState *state);

protected:

  //////////
  // Destructor
  virtual ~part_targetcolor(void);

  //////////
  // Scale changed
  void            scaleMess(float scale);

  //////////
  // Color changed
  void            colorMess(float red, float green, float blue, float alpha);


  //////////
  // The scale factor angle
  float           m_scale;

  //////////
  // The color vector (RGBA)
  float                     m_color[4];

private:

  //////////
  // static member functions
  static void     colorMessCallback(void *data, t_symbol*, int argc,
                                    t_atom *argv);
};

#endif  // for header file
