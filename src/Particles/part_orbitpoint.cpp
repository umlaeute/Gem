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

#include "part_orbitpoint.h"


#include "papi/papi.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS(part_orbitpoint, t_floatarg, A_DEFFLOAT,
                             t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// part_orbitpoint
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_orbitpoint :: part_orbitpoint(t_floatarg xpos, t_floatarg ypos,
                                   t_floatarg zpos, t_floatarg grav)
  : m_grav(1.f)
{
  m_pos[0] = xpos;
  m_pos[1] = ypos;
  m_pos[2] = zpos;
  if (grav) {
    m_grav = grav;
  }

  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"),
            gensym("vector"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
            gensym("grav"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_orbitpoint :: ~part_orbitpoint(void)
{ }

/////////////////////////////////////////////////////////
// renderParticles
//
/////////////////////////////////////////////////////////
void part_orbitpoint :: renderParticles(GemState *state)
{
  if (m_tickTime > 0.f) {
    pOrbitPoint(m_pos[0], m_pos[1], m_pos[2], m_grav);
  }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_orbitpoint :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "grav", gravMess, float);
  CPPEXTERN_MSG3(classPtr, "vector", vectorMess, float, float, float);
}

//////////
void    part_orbitpoint :: vectorMess(float val1, float val2, float val3)
{
  m_pos[0] = val1;
  m_pos[1] = val2;
  m_pos[2] = val3;
}
void    part_orbitpoint :: gravMess(float grav)
{
  m_grav = grav;
}
