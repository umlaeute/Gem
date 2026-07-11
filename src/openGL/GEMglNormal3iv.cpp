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

#include "GEMglNormal3iv.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglNormal3iv, t_floatarg, A_DEFFLOAT,
                                t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglNormal3iv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglNormal3iv :: GEMglNormal3iv        (t_floatarg arg0, t_floatarg arg1,
    t_floatarg arg2)
{
  vMess(arg0, arg1, arg2);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglNormal3iv :: ~GEMglNormal3iv ()
{
  inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglNormal3iv :: render(GemState *state)
{
  glNormal3iv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglNormal3iv :: vMess (t_float arg0, t_float arg1,
                              t_float arg2)         // FUN
{
  v[0]=static_cast<GLint>(arg0);
  v[1]=static_cast<GLint>(arg1);
  v[2]=static_cast<GLint>(arg2);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglNormal3iv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglNormal3iv::vMessCallback),
                  gensym("v"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void GEMglNormal3iv :: vMessCallback (void* data, t_float arg0,
                                      t_float arg1, t_float arg2)
{
  GetMyClass(data)->vMess ( arg0, arg1, arg2);
}
