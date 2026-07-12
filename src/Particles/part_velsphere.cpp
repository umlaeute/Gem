////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////

#include "part_velsphere.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS(part_velsphere, t_floatarg, A_DEFFLOAT,
                             t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// part_velsphere
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_velsphere :: part_velsphere(t_floatarg xpos, t_floatarg ypos,
                                 t_floatarg zpos, t_floatarg rad)
  : m_radius(rad)
{
  error("this is obsolete, use [part_velocity sphere <x> <y> <z> <r>] instead");

  m_pos[0] = xpos;
  m_pos[1] = ypos;
  m_pos[2] = zpos;

  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"),
            gensym("vector"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
            gensym("vel"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_velsphere :: ~part_velsphere(void)
{ }

/////////////////////////////////////////////////////////
// renderParticles
//
/////////////////////////////////////////////////////////
void part_velsphere :: renderParticles(GemState *state)
{
  if (m_tickTime > 0.f) {
    pVelocityD(PDSphere, m_pos[0], m_pos[1], m_pos[2], m_radius);
  }
}

void part_velsphere::vectorMess(float val1, float val2, float val3)
{
  m_pos[0] = val1;
  m_pos[1] = val2;
  m_pos[2] = val3;
}
void part_velsphere::velMess(float num)
{
  m_radius = num;
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_velsphere :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG3(classPtr, "vector", vectorMess, float, float, float);
  CPPEXTERN_MSG1(classPtr, "vel", velMess, float);
}
