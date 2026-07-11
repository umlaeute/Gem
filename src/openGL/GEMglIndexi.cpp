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

#include "GEMglIndexi.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglIndexi, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglIndexi :: GEMglIndexi      (t_floatarg arg0) :
  c(static_cast<GLint>(arg0))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("c"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglIndexi :: ~GEMglIndexi ()
{
  inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglIndexi :: render(GemState *state)
{
  glIndexi (c);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglIndexi :: cMess (t_float arg1)        // FUN
{
  c = static_cast<GLint>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglIndexi :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglIndexi::cMessCallback),     gensym("c"),
                  A_DEFFLOAT, A_NULL);
};

void GEMglIndexi :: cMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->cMess ( static_cast<t_float>(arg0));
}
