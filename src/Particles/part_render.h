/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PARTICLES_PART_RENDER_H_
#define _INCLUDE__GEM_PARTICLES_PART_RENDER_H_

#include "Particles/partlib_base.h"
#include "Gem/GemGL.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

  part_render

DESCRIPTION

  Draw a part_render group

-----------------------------------------------------------------*/
class GEM_EXTERN part_render : public partlib_base
{
  CPPEXTERN_HEADER(part_render, partlib_base);

public:

  //////////
  // Constructor
  part_render(void);

  //////////
  virtual void  renderParticles(GemState *state);
  virtual void  postrender(GemState *state);

protected:

  //////////
  // Destructor
  virtual ~part_render();
  void          colorMess(bool state);
  void          sizeMess (bool state);

  // How the object should be drawn
  bool          m_colorize;
  bool          m_sizing;

  unsigned int           m_number;

  GLfloat        *m_pos;
  GLfloat        *m_colors;
  GLfloat        *m_sizes;

};

#endif  // for header file
