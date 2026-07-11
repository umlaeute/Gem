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

#include "GEMglRasterPos2s.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglRasterPos2s, t_floatarg, A_DEFFLOAT,
                              t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglRasterPos2s :: GEMglRasterPos2s    (t_floatarg arg0,
    t_floatarg arg1) :
  x(static_cast<GLshort>(arg0)),
  y(static_cast<GLshort>(arg1))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("x"));
  m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("y"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglRasterPos2s :: ~GEMglRasterPos2s ()
{
  inlet_free(m_inlet[0]);
  inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglRasterPos2s :: render(GemState *state)
{
  glRasterPos2s (x, y);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglRasterPos2s :: xMess (t_float arg1)   // FUN
{
  x = static_cast<GLshort>(arg1);
  setModified();
}

void GEMglRasterPos2s :: yMess (t_float arg1)   // FUN
{
  y = static_cast<GLshort>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglRasterPos2s :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglRasterPos2s::xMessCallback),
                  gensym("x"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglRasterPos2s::yMessCallback),
                  gensym("y"), A_DEFFLOAT, A_NULL);
};

void GEMglRasterPos2s :: xMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->xMess ( static_cast<t_float>(arg0));
}
void GEMglRasterPos2s :: yMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->yMess ( static_cast<t_float>(arg0));
}
