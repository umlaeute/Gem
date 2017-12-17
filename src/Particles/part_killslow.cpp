////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "part_killslow.h"


#include "papi/papi.h"

CPPEXTERN_NEW_WITH_ONE_ARG(part_killslow, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// part_killslow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_killslow :: part_killslow(t_floatarg num)
  : m_killSpeed(.01f)
{
  if (num > 0) {
    m_killSpeed = num;
  }
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
            gensym("speed"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_killslow :: ~part_killslow()
{ }

/////////////////////////////////////////////////////////
// renderParticles
//
/////////////////////////////////////////////////////////
void part_killslow :: renderParticles(GemState *state)
{
  if (m_tickTime > 0.f) {
    pKillSlow(m_killSpeed);
  }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_killslow :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "speed", speedMess, float);
}

void part_killslow :: speedMess(float num)
{
  m_killSpeed = num;
}
