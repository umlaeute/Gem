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

#include "part_killold.h"


#include "papi/papi.h"

CPPEXTERN_NEW_WITH_ONE_ARG(part_killold, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// part_killold
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_killold :: part_killold(t_floatarg num)
  : m_killAge(10.f)
{
  if (num > 0) {
    m_killAge = num;
  }
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
            gensym("age"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_killold :: ~part_killold(void)
{ }

/////////////////////////////////////////////////////////
// renderParticles
//
/////////////////////////////////////////////////////////
void part_killold :: renderParticles(GemState *state)
{
  if (m_tickTime > 0.f) {
    pKillOld(m_killAge);
  }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_killold :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "age", ageMess, float);
}
void part_killold :: ageMess(float age)
{
  m_killAge = age;
}
