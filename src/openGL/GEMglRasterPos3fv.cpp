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

#include "GEMglRasterPos3fv.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglRasterPos3fv, t_floatarg, A_DEFFLOAT,
                                t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglRasterPos3fv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglRasterPos3fv :: GEMglRasterPos3fv  (t_floatarg arg0, t_floatarg arg1,
    t_floatarg arg2)
{
  vMess(arg0, arg1, arg2);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglRasterPos3fv :: ~GEMglRasterPos3fv ()
{
  inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglRasterPos3fv :: render(GemState *state)
{
  glRasterPos3fv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglRasterPos3fv :: vMess (t_float arg0, t_float arg1,
                                 t_float arg2)      // FUN
{
  v[0]=static_cast<GLfloat>(arg0);
  v[1]=static_cast<GLfloat>(arg1);
  v[2]=static_cast<GLfloat>(arg2);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglRasterPos3fv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglRasterPos3fv::vMessCallback),
                  gensym("v"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void GEMglRasterPos3fv :: vMessCallback (void* data, t_float arg0,
    t_float arg1, t_float arg2)
{
  GetMyClass(data)->vMess ( arg0, arg1, arg2);
}
