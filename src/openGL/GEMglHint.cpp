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

#include "GEMglHint.h"
#include "Gem/Exception.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglHint );
using namespace gem::utils::gl;

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglHint :: GEMglHint (int argc, t_atom*argv) :
  target(0),
  mode(0)
{
  if(2==argc) {
    targetMess(argv[0]);
    modeMess(argv[1]);
  } else if (argc) {
    throw(GemException("invalid number of arguments"));
  }
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("target"));
  m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("mode"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglHint :: ~GEMglHint ()
{
  inlet_free(m_inlet[0]);
  inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglHint :: render(GemState *state)
{
  glHint (target, mode);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglHint :: targetMess (t_atom arg)       // FUN
{
  target = static_cast<GLenum>(getGLdefine(&arg));
  setModified();
}

void GEMglHint :: modeMess (t_atom arg)         // FUN
{
  mode = static_cast<GLenum>(getGLdefine(&arg));
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglHint :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglHint::targetMessCallback),
                  gensym("target"), A_GIMME, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglHint::modeMessCallback),
                  gensym("mode"), A_GIMME, A_NULL);
};

void GEMglHint :: targetMessCallback (void* data, t_symbol*, int argc,
                                      t_atom*argv)
{
  if(argc==1) {
    GetMyClass(data)->targetMess ( argv[0]);
  }
}
void GEMglHint :: modeMessCallback (void* data, t_symbol*, int argc,
                                    t_atom*argv)
{
  if(argc==1) {
    GetMyClass(data)->modeMess ( argv[0]);
  }
}
