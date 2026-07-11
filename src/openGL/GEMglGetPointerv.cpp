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

#include "GEMglGetPointerv.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglGetPointerv, t_floatarg, A_DEFFLOAT );

/////////////////////////////////////////////////////////
//
// GEMglGetPointerv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglGetPointerv :: GEMglGetPointerv    (t_floatarg arg0)
{
  pnameMess(arg0);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglGetPointerv :: ~GEMglGetPointerv ()
{
  inlet_free(m_inlet);
}

//////////////////
// extension check
bool GEMglGetPointerv :: isRunnable(void)
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
void GEMglGetPointerv :: render(GemState *state)
{
  glGetPointerv (pname,params);
  post("not really implemented:: got data @ %X, what should i do with it?",
       params);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglGetPointerv :: pnameMess (t_float arg0)       // FUN
{
  pname=static_cast<GLenum>(arg0);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglGetPointerv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglGetPointerv::pnameMessCallback),
                  gensym("pname"), A_DEFFLOAT, A_NULL);
}

void GEMglGetPointerv :: pnameMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->pnameMess ( arg0 );
}
