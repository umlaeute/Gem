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

#include "rotate.h"
#include "Gem/GemGL.h"
#include "Gem/Exception.h"

CPPEXTERN_NEW_WITH_GIMME(rotate);

/////////////////////////////////////////////////////////
//
// rotate
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
rotate :: rotate(int argc, t_atom *argv)
{
  m_angle = 0.0;
  if (argc == 4) {
    m_angle = atom_getfloat(&argv[0]);
    vectorMess(atom_getfloat(&argv[1]), atom_getfloat(&argv[2]),
               atom_getfloat(&argv[3]));
  } else if (argc == 3) vectorMess(atom_getfloat(&argv[0]),
                                     atom_getfloat(&argv[1]),
                                     atom_getfloat(&argv[2]));

  else if (argc == 0) {
    vectorMess(1, 0, 0);
  } else {
    throw(GemException("needs 0, 3, or 4 arguments"));
  }

  // create the new inlets
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("ft1"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vector"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
rotate :: ~rotate()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void rotate :: render(GemState *)
{
  glRotatef(m_angle, m_vector[0], m_vector[1], m_vector[2]);
}

/////////////////////////////////////////////////////////
// angleMess
//
/////////////////////////////////////////////////////////
void rotate :: angleMess(float angle)
{
  if ( angle > 0) {
    while (angle >= 360.0) {
      angle -= 360.;
    }
  } else if ( angle < 0) {
    while (angle <= -360.0) {
      angle += 360.;
    }
  }
  m_angle = angle;
  setModified();
}

/////////////////////////////////////////////////////////
// vectorMess
//
/////////////////////////////////////////////////////////
void rotate :: vectorMess(float x, float y, float z)
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
void rotate :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "ft1", angleMess, float);
  CPPEXTERN_MSG3(classPtr, "vector", vectorMess, float, float, float);
}
