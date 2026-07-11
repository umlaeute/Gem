////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
// this file has been generated...
////////////////////////////////////////////////////////

#include "GEMglDisable.h"
#include "Gem/Exception.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglDisable );
using namespace gem::utils::gl;

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglDisable :: GEMglDisable (int argc, t_atom*argv) :
  cap(0)
{
  if(1==argc) {
    capMess(argv[0]);
  } else if(argc) {
    throw(GemException("invalid number of arguments"));
  }
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("cap"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglDisable :: ~GEMglDisable ()
{
  inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglDisable :: render(GemState *state)
{
  glDisable (cap);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglDisable :: capMess (t_atom arg)       // FUN
{
  cap = static_cast<GLenum>(getGLdefine(&arg));
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglDisable :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglDisable::capMessCallback),
                  gensym("cap"), A_GIMME, A_NULL);
};

void GEMglDisable :: capMessCallback (void* data, t_symbol*, int argc,
                                      t_atom*argv)
{
  if(argc==1) {
    GetMyClass(data)->capMess ( argv[0]);
  }
}
