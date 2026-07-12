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

#include "scaleXYZ.h"
#include "Gem/GemGL.h"
#include "Gem/Exception.h"

CPPEXTERN_NEW_WITH_GIMME(scaleXYZ);

/////////////////////////////////////////////////////////
//
// scaleXYZ
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
scaleXYZ :: scaleXYZ(int argc, t_atom *argv)
{
  if (argc == 3) {
    m_vector[0] = atom_getfloat(&argv[0]);
    m_vector[1] = atom_getfloat(&argv[1]);
    m_vector[2] = atom_getfloat(&argv[2]);
  } else if (argc == 1) {
    m_vector[0] = atom_getfloat(&argv[0]);
    m_vector[1] = atom_getfloat(&argv[0]);
    m_vector[2] = atom_getfloat(&argv[0]);
  } else if (argc == 0) {
    m_vector[0] = 1.f;
    m_vector[1] = 1.f;
    m_vector[2] = 1.f;
  } else {
    throw(GemException("needs 0, 1, or 3 arguments"));
  }

  // create the new inlets
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("xVal"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("yVal"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("zVal"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
scaleXYZ :: ~scaleXYZ()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void scaleXYZ :: render(GemState *)
{
  glScalef(m_vector[0], m_vector[1], m_vector[2]);
}

/////////////////////////////////////////////////////////
// xMess
//
/////////////////////////////////////////////////////////
void scaleXYZ :: xMess(float val)
{
  m_vector[0] = val;
  setModified();
}

/////////////////////////////////////////////////////////
// yMess
//
/////////////////////////////////////////////////////////
void scaleXYZ :: yMess(float val)
{
  m_vector[1] = val;
  setModified();
}

/////////////////////////////////////////////////////////
// zMess
//
/////////////////////////////////////////////////////////
void scaleXYZ :: zMess(float val)
{
  m_vector[2] = val;
  setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void scaleXYZ :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "xVal", xMess, float);
  CPPEXTERN_MSG1(classPtr, "yVal", yMess, float);
  CPPEXTERN_MSG1(classPtr, "zVal", zMess, float);
}
