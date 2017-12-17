/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

        Apply damping to particles

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PARTICLES_PART_DAMP_H_
#define _INCLUDE__GEM_PARTICLES_PART_DAMP_H_

#include "papi/papi.h"

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

        part_damp

        Apply damping to particles

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN part_damp : public partlib_base
{
  CPPEXTERN_HEADER(part_damp, partlib_base);

public:

  //////////
  // Constructor
  part_damp(t_floatarg val1, t_floatarg val2, t_floatarg val3);

  //////////
  virtual void    renderParticles(GemState *state);

  //////////
  // Destructor
  virtual ~part_damp(void);

  //////////
  // Vector changed
  void            vectorMess(float x, float y, float z);

protected:

  //////////
  float                     m_vector[3];
};

#endif  // for header file
