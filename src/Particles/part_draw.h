/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

        Draw a particle group

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PARTICLES_PART_DRAW_H_
#define _INCLUDE__GEM_PARTICLES_PART_DRAW_H_

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

        part_draw

        Draw a particle group

DESCRIPTION

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
