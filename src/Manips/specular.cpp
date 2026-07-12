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

#include "specular.h"
#include "Gem/GemGL.h"
#include "Gem/Exception.h"

CPPEXTERN_NEW_WITH_GIMME(specular);

/////////////////////////////////////////////////////////
//
// specular
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
specular :: specular(int argc, t_atom *argv)
{
  if (argc == 4) specularMess(atom_getfloat(&argv[0]),
                                atom_getfloat(&argv[1]),
                                atom_getfloat(&argv[2]), atom_getfloat(&argv[3]));
  else if (argc == 3) specularMess(atom_getfloat(&argv[0]),
                                     atom_getfloat(&argv[1]),
                                     atom_getfloat(&argv[2]), 1.f);
  else if (argc == 0) {
    specularMess(0.f, 0.f, 0.f, 1.f);
  } else {
    throw(GemException("needs 0, 3, or 4 arguments"));
  }

  // create the new inlet
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("specular"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
specular :: ~specular()
{ }

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void specular :: postrender(GemState *)
{
  glEnable(GL_COLOR_MATERIAL);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void specular :: render(GemState *)
{
  glDisable(GL_COLOR_MATERIAL);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
}

/////////////////////////////////////////////////////////
// specularMess
//
/////////////////////////////////////////////////////////
void specular :: specularMess(float red, float green, float blue,
                              float alpha)
{
  m_specular[0] = red;
  m_specular[1] = green;
  m_specular[2] = blue;
  m_specular[3] = alpha;
  setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void specular :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&specular::specularMessCallback),
                  gensym("specular"), A_GIMME, A_NULL);
}
void specular :: specularMessCallback(void *data, t_symbol*, int argc,
                                      t_atom *argv)
{
  float alpha = 1;
  if (argc == 4) {
    alpha = atom_getfloat(&argv[3]);
  }
  GetMyClass(data)->specularMess(atom_getfloat(&argv[0]),
                                 atom_getfloat(&argv[1]),
                                 atom_getfloat(&argv[2]), alpha);
}
