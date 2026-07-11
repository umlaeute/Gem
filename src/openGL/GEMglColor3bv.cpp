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

#include "GEMglColor3bv.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglColor3bv, t_floatarg, A_DEFFLOAT,
                                t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglColor3bv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglColor3bv :: GEMglColor3bv  (t_floatarg arg0, t_floatarg arg1,
                                 t_floatarg arg2)
{
  vMess(arg0, arg1, arg2);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglColor3bv :: ~GEMglColor3bv ()
{
  inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglColor3bv :: render(GemState *state)
{
  glColor3bv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglColor3bv :: vMess (t_float arg0, t_float arg1,
                             t_float arg2)          // FUN
{
  v[0]=static_cast<GLbyte>(arg0);
  v[1]=static_cast<GLbyte>(arg1);
  v[2]=static_cast<GLbyte>(arg2);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglColor3bv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglColor3bv::vMessCallback),   gensym("v"),
                  A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void GEMglColor3bv :: vMessCallback (void* data, t_float arg0,
                                     t_float arg1, t_float arg2)
{
  GetMyClass(data)->vMess ( arg0, arg1, arg2);
}
