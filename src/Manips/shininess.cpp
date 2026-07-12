////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2000, IOhannes m zmölnig and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////

#include "shininess.h"
#include "Gem/GemGL.h"
#include "Gem/Exception.h"

CPPEXTERN_NEW_WITH_GIMME(shininess);

/////////////////////////////////////////////////////////
//
// shininess
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
shininess :: shininess(int argc, t_atom *argv)
{
  if (argc == 1) {
    shininessMess(atom_getfloat(&argv[0]));
  } else if (argc == 0) {
    shininessMess(0.f);
  } else {
    throw(GemException("needs 0 or 1 arguments"));
  }

  // create the new inlet
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("shininess"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
shininess :: ~shininess()
{ }

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void shininess :: postrender(GemState *)
{
  glEnable(GL_COLOR_MATERIAL);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void shininess :: render(GemState *)
{
  glDisable(GL_COLOR_MATERIAL);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &m_shininess);
}

/////////////////////////////////////////////////////////
// shininessMess
//
/////////////////////////////////////////////////////////
void shininess :: shininessMess(float val)
{
  m_shininess = val;
  if (m_shininess < 0.f) {
    m_shininess = 0.f;
  } else if (m_shininess > 128.f) {
    m_shininess = 128.f;
  }

  setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void shininess :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "shininess", shininessMess, float);
}
