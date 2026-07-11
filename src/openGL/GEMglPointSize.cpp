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

#include "GEMglPointSize.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglPointSize, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglPointSize :: GEMglPointSize        (t_floatarg arg0) :
  size(static_cast<GLfloat>(arg0))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("size"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglPointSize :: ~GEMglPointSize ()
{
  inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglPointSize :: render(GemState *state)
{
  glPointSize (size);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglPointSize :: sizeMess (t_float arg1)          // FUN
{
  size = static_cast<GLfloat>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglPointSize :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglPointSize::sizeMessCallback),
                  gensym("size"), A_DEFFLOAT, A_NULL);
};

void GEMglPointSize :: sizeMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->sizeMess ( static_cast<t_float>(arg0));
}
