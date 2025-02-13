/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

        Advance particles

    Copyright (c) 2025 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PARTICLES_PART_MOVE_H_
#define _INCLUDE__GEM_PARTICLES_PART_MOVE_H_

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

        part_move

        Draw a part_move group

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN part_move : public partlib_base
{
  CPPEXTERN_HEADER(part_move, partlib_base);

public:

  //////////
  // Constructor
  part_move();

  //////////
  virtual void  renderParticles(GemState *state);

protected:

  //////////
  // Destructor
  virtual ~part_move();
};

#endif  // for header file
