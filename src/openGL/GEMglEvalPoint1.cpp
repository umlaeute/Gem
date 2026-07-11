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

#include "GEMglEvalPoint1.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglEvalPoint1, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglEvalPoint1 :: GEMglEvalPoint1      (t_floatarg arg0) :
  i(static_cast<GLint>(arg0))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("i"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglEvalPoint1 :: ~GEMglEvalPoint1 ()
{
  inlet_free(m_inlet[0]);
}
//////////////////
// extension check
bool GEMglEvalPoint1 :: isRunnable(void)
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
void GEMglEvalPoint1 :: render(GemState *state)
{
  glEvalPoint1 (i);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglEvalPoint1 :: iMess (t_float arg1)    // FUN
{
  i = static_cast<GLint>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglEvalPoint1 :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglEvalPoint1::iMessCallback),
                  gensym("i"), A_DEFFLOAT, A_NULL);
}

void GEMglEvalPoint1 :: iMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->iMess ( static_cast<t_float>(arg0));
}
