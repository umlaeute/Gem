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

#include "GEMglTexCoord1sv.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglTexCoord1sv, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglTexCoord1sv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTexCoord1sv :: GEMglTexCoord1sv    (t_floatarg arg0)
{
  vMess(arg0);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglTexCoord1sv :: ~GEMglTexCoord1sv ()
{
  inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglTexCoord1sv :: render(GemState *state)
{
  glTexCoord1sv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglTexCoord1sv :: vMess (t_float arg0)   // FUN
{
  v[0]=static_cast<GLshort>(arg0);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTexCoord1sv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglTexCoord1sv::vMessCallback),
                  gensym("v"), A_DEFFLOAT, A_NULL);
}

void GEMglTexCoord1sv :: vMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->vMess ( arg0);
}
