/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

        Set the velocity domain for particles

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PARTICLES_PART_VELCONE_H_
#define _INCLUDE__GEM_PARTICLES_PART_VELCONE_H_

#include "papi/papi.h"

#include "Particles/partlib_base.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

        part_velcone

        Set the velocity domain for particles

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN part_velcone : public partlib_base
{
  CPPEXTERN_HEADER(part_velcone, partlib_base);

public:

  //////////
  // Constructor
  part_velcone(t_floatarg val1, t_floatarg val2, t_floatarg val,
               t_floatarg radius);

  //////////
  virtual void    renderParticles(GemState *state);

protected:

  //////////
  // Destructor
  virtual ~part_velcone(void);

  //////////
  void                      vectorMess(float val1, float val2, float val3);
  void                      velMess(float num);

  //////////
  float                     m_pos[3];
  float                     m_radius;
};

#endif  // for header file
