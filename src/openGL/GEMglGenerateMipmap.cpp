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

#include "GEMglGenerateMipmap.h"
#include "Gem/Exception.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglGenerateMipmap );
using namespace gem::utils::gl;

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglGenerateMipmap :: GEMglGenerateMipmap (int argc, t_atom*argv) :
  target(0)
{
  switch(argc) {
  case 0:
    break;
  case 1:
    targetMess(argv[0]);
    break;
  default:
    throw(GemException("invalid number of arguments"));
  }
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("target"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglGenerateMipmap :: ~GEMglGenerateMipmap ()
{
  inlet_free(m_inlet[0]);
}
//////////////////
// extension check
bool GEMglGenerateMipmap:: isRunnable(void)
{
  if(GLEW_VERSION_1_3) {
    return true;
  }
  error("your system does not support OpenGL-1.3");
  return false;
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglGenerateMipmap :: render(GemState *state)
{
  glGenerateMipmap (target);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglGenerateMipmap :: targetMess (t_atom&arg)        // FUN
{
  target = static_cast<GLenum>(getGLdefine(&arg));
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglGenerateMipmap :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglGenerateMipmap::targetMessCallback),   gensym("target"),
                  A_GIMME, A_NULL);
};

void GEMglGenerateMipmap :: targetMessCallback (void* data, t_symbol*, int argc,
    t_atom*argv)
{
  if(argc==1) {
    GetMyClass(data)->targetMess ( argv[0]);
  }
}
