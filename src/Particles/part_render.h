/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

        Draw a part_render group

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PARTICLES_PART_RENDER_H_
#define _INCLUDE__GEM_PARTICLES_PART_RENDER_H_

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

        part_render

        Draw a part_render group

DESCRIPTION

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
