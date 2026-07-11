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

#include "GEMglRasterPos3sv.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglRasterPos3sv, t_floatarg, A_DEFFLOAT,
                                t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglRasterPos3sv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglRasterPos3sv :: GEMglRasterPos3sv  (t_floatarg arg0, t_floatarg arg1,
    t_floatarg arg2)
{
  vMess(arg0, arg1, arg2);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglRasterPos3sv :: ~GEMglRasterPos3sv ()
{
  inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglRasterPos3sv :: render(GemState *state)
{
  glRasterPos3sv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglRasterPos3sv :: vMess (t_float arg0, t_float arg1,
                                 t_float arg2)      // FUN
{
  v[0]=static_cast<GLshort>(arg0);
  v[1]=static_cast<GLshort>(arg1);
  v[2]=static_cast<GLshort>(arg2);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglRasterPos3sv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglRasterPos3sv::vMessCallback),
                  gensym("v"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void GEMglRasterPos3sv :: vMessCallback (void* data, t_float arg0,
    t_float arg1, t_float arg2)
{
  GetMyClass(data)->vMess ( arg0, arg1, arg2);
}
