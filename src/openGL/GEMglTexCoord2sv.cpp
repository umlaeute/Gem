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

#include "GEMglTexCoord2sv.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglTexCoord2sv, t_floatarg, A_DEFFLOAT,
                              t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglTexCoord2sv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTexCoord2sv :: GEMglTexCoord2sv    (t_floatarg arg0, t_floatarg arg1)
{
  vMess(arg0, arg1);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglTexCoord2sv :: ~GEMglTexCoord2sv ()
{
  inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglTexCoord2sv :: render(GemState *state)
{
  glTexCoord2sv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglTexCoord2sv :: vMess (t_float arg0, t_float arg1)     // FUN
{
  v[0]=static_cast<GLshort>(arg0);
  v[1]=static_cast<GLshort>(arg1);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTexCoord2sv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglTexCoord2sv::vMessCallback),
                  gensym("v"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void GEMglTexCoord2sv :: vMessCallback (void* data, t_float arg0,
                                        t_float arg1)
{
  GetMyClass(data)->vMess ( arg0, arg1);
}
