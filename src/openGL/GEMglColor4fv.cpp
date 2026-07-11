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

#include "GEMglColor4fv.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglColor4fv, t_floatarg, A_DEFFLOAT,
                               t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglColor4fv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglColor4fv :: GEMglColor4fv  (t_floatarg arg0, t_floatarg arg1,
                                 t_floatarg arg2, t_floatarg arg3)
{
  vMess(arg0, arg1, arg2, arg3);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglColor4fv :: ~GEMglColor4fv ()
{
  inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglColor4fv :: render(GemState *state)
{
  glColor4fv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglColor4fv :: vMess (t_float arg0, t_float arg1, t_float arg2,
                             t_float arg3)    // FUN
{
  v[0]=static_cast<GLfloat>(arg0);
  v[1]=static_cast<GLfloat>(arg1);
  v[2]=static_cast<GLfloat>(arg2);
  v[3]=static_cast<GLfloat>(arg3);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglColor4fv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglColor4fv::vMessCallback),   gensym("v"),
                  A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void GEMglColor4fv :: vMessCallback (void* data, t_float arg0,
                                     t_float arg1, t_float arg2, t_float arg3)
{
  GetMyClass(data)->vMess ( arg0, arg1, arg2, arg3);
}
