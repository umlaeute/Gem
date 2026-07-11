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

#include "GEMglRasterPos2sv.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglRasterPos2sv, t_floatarg, A_DEFFLOAT,
                              t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglRasterPos2sv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglRasterPos2sv :: GEMglRasterPos2sv  (t_floatarg arg0, t_floatarg arg1)
{
  vMess(arg0, arg1);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglRasterPos2sv :: ~GEMglRasterPos2sv ()
{
  inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglRasterPos2sv :: render(GemState *state)
{
  glRasterPos2sv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglRasterPos2sv :: vMess (t_float arg0, t_float arg1)    // FUN
{
  v[0]=static_cast<GLshort>(arg0);
  v[1]=static_cast<GLshort>(arg1);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglRasterPos2sv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglRasterPos2sv::vMessCallback),
                  gensym("v"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void GEMglRasterPos2sv :: vMessCallback (void* data, t_float arg0,
    t_float arg1)
{
  GetMyClass(data)->vMess ( arg0, arg1);
}
