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

#include "GEMglVertex4dv.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglVertex4dv, t_floatarg, A_DEFFLOAT,
                               t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglVertex4dv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglVertex4dv :: GEMglVertex4dv        (t_floatarg arg0, t_floatarg arg1,
    t_floatarg arg2, t_floatarg arg3)
{
  vMess(arg0, arg1, arg2, arg3);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglVertex4dv :: ~GEMglVertex4dv ()
{
  inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglVertex4dv :: render(GemState *state)
{
  glVertex4dv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglVertex4dv :: vMess (t_float arg0, t_float arg1, t_float arg2,
                              t_float arg3)   // FUN
{
  v[0]=static_cast<GLdouble>(arg0);
  v[1]=static_cast<GLdouble>(arg1);
  v[2]=static_cast<GLdouble>(arg2);
  v[3]=static_cast<GLdouble>(arg3);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglVertex4dv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglVertex4dv::vMessCallback),
                  gensym("v"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void GEMglVertex4dv :: vMessCallback (void* data, t_float arg0,
                                      t_float arg1, t_float arg2, t_float arg3)
{
  GetMyClass(data)->vMess ( arg0, arg1, arg2, arg3);
}
