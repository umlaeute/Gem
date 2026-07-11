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

#include "GEMglTexCoord1s.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglTexCoord1s, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTexCoord1s :: GEMglTexCoord1s      (t_floatarg arg0) :
  s(static_cast<GLshort>(arg0))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("s"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglTexCoord1s :: ~GEMglTexCoord1s ()
{
  inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglTexCoord1s :: render(GemState *state)
{
  glTexCoord1s (s);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglTexCoord1s :: sMess (t_float arg1)    // FUN
{
  s = static_cast<GLshort>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTexCoord1s :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglTexCoord1s::sMessCallback),
                  gensym("s"), A_DEFFLOAT, A_NULL);
};

void GEMglTexCoord1s :: sMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->sMess ( static_cast<t_float>(arg0));
}
