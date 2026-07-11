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

#include "GEMglIndexMask.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglIndexMask, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglIndexMask :: GEMglIndexMask        (t_floatarg arg0) :
  mask(static_cast<GLuint>(arg0))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("mask"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglIndexMask :: ~GEMglIndexMask ()
{
  inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglIndexMask :: render(GemState *state)
{
  glIndexMask (mask);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglIndexMask :: maskMess (t_float arg1)          // FUN
{
  mask = static_cast<GLuint>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglIndexMask :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglIndexMask::maskMessCallback),
                  gensym("mask"), A_DEFFLOAT, A_NULL);
};

void GEMglIndexMask :: maskMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->maskMess ( static_cast<t_float>(arg0));
}
