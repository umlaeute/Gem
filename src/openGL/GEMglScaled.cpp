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

#include "GEMglScaled.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglScaled, t_floatarg, A_DEFFLOAT,
                                t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglScaled :: GEMglScaled      (t_floatarg arg0, t_floatarg arg1,
                                 t_floatarg arg2) :
  x(static_cast<GLdouble>(arg0)),
  y(static_cast<GLdouble>(arg1)),
  z(static_cast<GLdouble>(arg2))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("x"));
  m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("y"));
  m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("z"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglScaled :: ~GEMglScaled ()
{
  inlet_free(m_inlet[0]);
  inlet_free(m_inlet[1]);
  inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglScaled :: render(GemState *state)
{
  glScaled (x, y, z);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglScaled :: xMess (t_float arg1)        // FUN
{
  x = static_cast<GLdouble>(arg1);
  setModified();
}

void GEMglScaled :: yMess (t_float arg1)        // FUN
{
  y = static_cast<GLdouble>(arg1);
  setModified();
}

void GEMglScaled :: zMess (t_float arg1)        // FUN
{
  z = static_cast<GLdouble>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglScaled :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglScaled::xMessCallback),     gensym("x"),
                  A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglScaled::yMessCallback),     gensym("y"),
                  A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglScaled::zMessCallback),     gensym("z"),
                  A_DEFFLOAT, A_NULL);
};

void GEMglScaled :: xMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->xMess ( static_cast<t_float>(arg0));
}
void GEMglScaled :: yMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->yMess ( static_cast<t_float>(arg0));
}
void GEMglScaled :: zMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->zMess ( static_cast<t_float>(arg0));
}
