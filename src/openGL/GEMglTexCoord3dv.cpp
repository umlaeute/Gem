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

#include "GEMglTexCoord3dv.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglTexCoord3dv, t_floatarg, A_DEFFLOAT,
                                t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglTexCoord3dv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTexCoord3dv :: GEMglTexCoord3dv    (t_floatarg arg0, t_floatarg arg1,
    t_floatarg arg2)
{
  vMess(arg0, arg1, arg2);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglTexCoord3dv :: ~GEMglTexCoord3dv ()
{
  inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglTexCoord3dv :: render(GemState *state)
{
  glTexCoord3dv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglTexCoord3dv :: vMess (t_float arg0, t_float arg1,
                                t_float arg2)       // FUN
{
  v[0]=static_cast<GLdouble>(arg0);
  v[1]=static_cast<GLdouble>(arg1);
  v[2]=static_cast<GLdouble>(arg2);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTexCoord3dv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglTexCoord3dv::vMessCallback),
                  gensym("v"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void GEMglTexCoord3dv :: vMessCallback (void* data, t_float arg0,
                                        t_float arg1, t_float arg2)
{
  GetMyClass(data)->vMess ( arg0, arg1, arg2);
}
