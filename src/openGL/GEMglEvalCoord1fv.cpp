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

#include "GEMglEvalCoord1fv.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglEvalCoord1fv, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglEvalCoord1fv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglEvalCoord1fv :: GEMglEvalCoord1fv  (t_floatarg arg0)
{
  vMess(arg0);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglEvalCoord1fv :: ~GEMglEvalCoord1fv ()
{
  inlet_free(m_inlet);
}
//////////////////
// extension check
bool GEMglEvalCoord1fv :: isRunnable(void)
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
void GEMglEvalCoord1fv :: render(GemState *state)
{
  glEvalCoord1fv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglEvalCoord1fv :: vMess (t_float arg0)          // FUN
{
  v[0]=static_cast<GLfloat>(arg0);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglEvalCoord1fv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglEvalCoord1fv::vMessCallback),
                  gensym("v"), A_DEFFLOAT, A_NULL);
}

void GEMglEvalCoord1fv :: vMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->vMess ( arg0);
}
