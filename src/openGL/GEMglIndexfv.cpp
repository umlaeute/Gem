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

#include "GEMglIndexfv.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglIndexfv, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglIndexfv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglIndexfv :: GEMglIndexfv    (t_floatarg arg0)
{
  cMess(arg0);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglIndexfv :: ~GEMglIndexfv ()
{
  inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglIndexfv :: render(GemState *state)
{
  glIndexfv (c);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglIndexfv :: cMess (t_float arg0)       // FUN
{
  c[0]=static_cast<GLfloat>(arg0);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglIndexfv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglIndexfv::cMessCallback),   gensym("c"),
                  A_DEFFLOAT, A_NULL);
}

void GEMglIndexfv :: cMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->cMess (arg0);
}
