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

#include "GEMglEnableClientState.h"
#include "Gem/Exception.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglEnableClientState );
using namespace gem::utils::gl;

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglEnableClientState :: GEMglEnableClientState (int argc, t_atom*argv) :
  array(0)
{
  if(1==argc) {
    arrayMess(argv[0]);
  } else if(argc) {
    throw(GemException("invalid number of arguments"));
  }
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("array"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglEnableClientState :: ~GEMglEnableClientState ()
{
  inlet_free(m_inlet[0]);
}
//////////////////
// extension check
bool GEMglEnableClientState :: isRunnable(void)
{
  if(GLEW_VERSION_1_1) {
    return true;
  }
  error("your system does not support OpenGL-1.1");
  return false;
}
/////////////////////////////////////////////////////////
// Render
//
void GEMglEnableClientState :: render(GemState *state)
{
  glEnableClientState (array);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglEnableClientState :: arrayMess (t_atom arg)   // FUN
{
  array = static_cast<GLenum>(getGLdefine(&arg));
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglEnableClientState :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglEnableClientState::arrayMessCallback),
                  gensym("array"), A_GIMME, A_NULL);
}

void GEMglEnableClientState :: arrayMessCallback (void* data, t_symbol*,
    int argc, t_atom*argv)
{
  if(argc==1) {
    GetMyClass(data)->arrayMess ( argv[0]);
  }
}
