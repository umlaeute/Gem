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

#include "GEMglColor4ubv.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglColor4ubv, t_floatarg, A_DEFFLOAT,
                               t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglColor4ubv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglColor4ubv :: GEMglColor4ubv        (t_floatarg arg0, t_floatarg arg1,
    t_floatarg arg2, t_floatarg arg3)
{
  vMess(arg0, arg1, arg2, arg3);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglColor4ubv :: ~GEMglColor4ubv ()
{
  inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglColor4ubv :: render(GemState *state)
{
  glColor4ubv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglColor4ubv :: vMess (t_float arg0, t_float arg1, t_float arg2,
                              t_float arg3)   // FUN
{
  v[0]=static_cast<GLubyte>(arg0);
  v[1]=static_cast<GLubyte>(arg1);
  v[2]=static_cast<GLubyte>(arg2);
  v[3]=static_cast<GLubyte>(arg3);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglColor4ubv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglColor4ubv::vMessCallback),
                  gensym("v"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void GEMglColor4ubv :: vMessCallback (void* data, t_float arg0,
                                      t_float arg1, t_float arg2, t_float arg3)
{
  GetMyClass(data)->vMess ( arg0, arg1, arg2, arg3);
}
