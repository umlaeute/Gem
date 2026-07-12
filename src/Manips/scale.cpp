////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
/////////////////////////////////////////////////////////

#include "scale.h"
#include "Gem/GemGL.h"
#include "Gem/Exception.h"

CPPEXTERN_NEW_WITH_GIMME(scale);

/////////////////////////////////////////////////////////
//
// scale
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
scale :: scale(int argc, t_atom *argv)
{
  m_distance  = 0.0f;
  if (argc == 4) {
    m_distance = atom_getfloat(&argv[0]);
    vectorMess(atom_getfloat(&argv[1]), atom_getfloat(&argv[2]),
               atom_getfloat(&argv[3]));
  } else if (argc == 3) {
    m_distance = 1.f;
    vectorMess(atom_getfloat(&argv[0]), atom_getfloat(&argv[1]),
               atom_getfloat(&argv[2]));
  } else if (argc == 1) {
    m_distance = atom_getfloat(&argv[0]);
    vectorMess(1.f, 1.f, 1.f);
  } else if (argc == 0) {
    vectorMess(1.f, 1.f, 1.f);
  } else {
    throw(GemException("needs 0, 1, 3, or 4 arguments"));
  }

  // create the new inlets
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("ft1"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vector"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
scale :: ~scale()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void scale :: render(GemState *)
{
  glScalef(m_vector[0] * m_distance, m_vector[1] * m_distance,
           m_vector[2] * m_distance);
}

/////////////////////////////////////////////////////////
// distanceMess
//
/////////////////////////////////////////////////////////
void scale :: distanceMess(float distance)
{
  m_distance = distance;
  setModified();
}

/////////////////////////////////////////////////////////
// vectorMess
//
/////////////////////////////////////////////////////////
void scale :: vectorMess(float x, float y, float z)
{
  m_vector[0] = x;
  m_vector[1] = y;
  m_vector[2] = z;
  setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void scale :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "ft1", distanceMess, float);
  CPPEXTERN_MSG3(classPtr, "vector", vectorMess, float, float, float);
}
