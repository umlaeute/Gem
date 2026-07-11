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

#include "GEMglCullFace.h"
#include "Gem/Exception.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglCullFace );
using namespace gem::utils::gl;

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglCullFace :: GEMglCullFace (int argc, t_atom*argv) :
  mode(0)
{
  if(1==argc) {
    modeMess(argv[0]);
  } else if(argc) {
    throw(GemException("invalid number of arguments"));
  }
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("mode"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglCullFace :: ~GEMglCullFace ()
{
  inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglCullFace :: render(GemState *state)
{
  glCullFace (mode);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglCullFace :: modeMess (t_atom arg)     // FUN
{
  mode = static_cast<GLenum>(getGLdefine(&arg));
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglCullFace :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglCullFace::modeMessCallback),
                  gensym("mode"), A_GIMME, A_NULL);
};

void GEMglCullFace :: modeMessCallback (void* data, t_symbol*, int argc,
                                        t_atom*argv)
{
  if(argc==1) {
    GetMyClass(data)->modeMess ( argv[0]);
  }
}
