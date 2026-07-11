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

#include "GEMglDisableClientState.h"
#include "Gem/Exception.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglDisableClientState );
using namespace gem::utils::gl;

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglDisableClientState :: GEMglDisableClientState (int argc,
    t_atom*argv) :
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
GEMglDisableClientState :: ~GEMglDisableClientState ()
{
  inlet_free(m_inlet[0]);
}
//////////////////
// extension check
bool GEMglDisableClientState :: isRunnable(void)
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
void GEMglDisableClientState :: render(GemState *state)
{
  glDisableClientState (array);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglDisableClientState :: arrayMess (t_atom arg)          // FUN
{
  array = static_cast<GLenum>(getGLdefine(&arg));
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglDisableClientState :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglDisableClientState::arrayMessCallback),
                  gensym("array"), A_GIMME, A_NULL);
}

void GEMglDisableClientState :: arrayMessCallback (void* data, t_symbol*,
    int argc, t_atom*argv)
{
  if(argc==1) {
    GetMyClass(data)->arrayMess ( argv[0]);
  }
}
