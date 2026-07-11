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

#include "GEMglLogicOp.h"
#include "Gem/Exception.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglLogicOp);
using namespace gem::utils::gl;

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglLogicOp :: GEMglLogicOp    (int argc, t_atom*argv) :
  opcode(0)
{
  if(1==argc) {
    opcodeMess(argv[0]);
  } else if(argc) {
    throw(GemException("invalid number of arguments"));
  }
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("opcode"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglLogicOp :: ~GEMglLogicOp ()
{
  inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglLogicOp :: render(GemState *state)
{
  glLogicOp (opcode);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglLogicOp :: opcodeMess (t_atom arg)    // FUN
{
  opcode = static_cast<GLenum>(getGLdefine(&arg));
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglLogicOp :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglLogicOp::opcodeMessCallback),
                  gensym("opcode"), A_GIMME, A_NULL);
};

void GEMglLogicOp :: opcodeMessCallback (void* data, t_symbol*,int argc,
    t_atom*argv)
{
  if(argc==1) {
    GetMyClass(data)->opcodeMess ( argv[0]);
  }
}
