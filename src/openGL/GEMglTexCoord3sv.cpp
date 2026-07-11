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

#include "GEMglTexCoord3sv.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglTexCoord3sv, t_floatarg, A_DEFFLOAT,
                                t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglTexCoord3sv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTexCoord3sv :: GEMglTexCoord3sv    (t_floatarg arg0, t_floatarg arg1,
    t_floatarg arg2)
{
  vMess(arg0, arg1, arg2);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglTexCoord3sv :: ~GEMglTexCoord3sv ()
{
  inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglTexCoord3sv :: render(GemState *state)
{
  glTexCoord3sv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglTexCoord3sv :: vMess (t_float arg0, t_float arg1,
                                t_float arg2)       // FUN
{
  v[0]=static_cast<GLshort>(arg0);
  v[1]=static_cast<GLshort>(arg1);
  v[2]=static_cast<GLshort>(arg2);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTexCoord3sv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglTexCoord3sv::vMessCallback),
                  gensym("v"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void GEMglTexCoord3sv :: vMessCallback (void* data, t_float arg0,
                                        t_float arg1, t_float arg2)
{
  GetMyClass(data)->vMess ( arg0, arg1, arg2);
}
