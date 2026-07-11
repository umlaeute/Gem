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

#include "GEMglIndexf.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglIndexf, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglIndexf :: GEMglIndexf      (t_floatarg arg0) :
  c(static_cast<GLfloat>(arg0))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("c"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglIndexf :: ~GEMglIndexf ()
{
  inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglIndexf :: render(GemState *state)
{
  glIndexf (c);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglIndexf :: cMess (t_float arg1)        // FUN
{
  c = static_cast<GLfloat>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglIndexf :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglIndexf::cMessCallback),     gensym("c"),
                  A_DEFFLOAT, A_NULL);
};

void GEMglIndexf :: cMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->cMess ( static_cast<t_float>(arg0));
}
