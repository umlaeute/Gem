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

#include "GEMglTexCoord3iv.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglTexCoord3iv, t_floatarg, A_DEFFLOAT,
                                t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglTexCoord3iv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTexCoord3iv :: GEMglTexCoord3iv    (t_floatarg arg0, t_floatarg arg1,
    t_floatarg arg2)
{
  vMess(arg0, arg1, arg2);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglTexCoord3iv :: ~GEMglTexCoord3iv ()
{
  inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglTexCoord3iv :: render(GemState *state)
{
  glTexCoord3iv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglTexCoord3iv :: vMess (t_float arg0, t_float arg1,
                                t_float arg2)       // FUN
{
  v[0]=static_cast<GLint>(arg0);
  v[1]=static_cast<GLint>(arg1);
  v[2]=static_cast<GLint>(arg2);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTexCoord3iv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglTexCoord3iv::vMessCallback),
                  gensym("v"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void GEMglTexCoord3iv :: vMessCallback (void* data, t_float arg0,
                                        t_float arg1, t_float arg2)
{
  GetMyClass(data)->vMess ( arg0, arg1, arg2);
}
