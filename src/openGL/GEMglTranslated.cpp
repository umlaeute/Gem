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

#include "GEMglTranslated.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglTranslated, t_floatarg, A_DEFFLOAT,
                                t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTranslated :: GEMglTranslated      (t_floatarg arg0, t_floatarg arg1,
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
GEMglTranslated :: ~GEMglTranslated ()
{
  inlet_free(m_inlet[0]);
  inlet_free(m_inlet[1]);
  inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglTranslated :: render(GemState *state)
{
  glTranslated (x, y, z);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglTranslated :: xMess (t_float arg1)    // FUN
{
  x = static_cast<GLdouble>(arg1);
  setModified();
}

void GEMglTranslated :: yMess (t_float arg1)    // FUN
{
  y = static_cast<GLdouble>(arg1);
  setModified();
}

void GEMglTranslated :: zMess (t_float arg1)    // FUN
{
  z = static_cast<GLdouble>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTranslated :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglTranslated::xMessCallback),
                  gensym("x"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglTranslated::yMessCallback),
                  gensym("y"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglTranslated::zMessCallback),
                  gensym("z"), A_DEFFLOAT, A_NULL);
};

void GEMglTranslated :: xMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->xMess ( static_cast<t_float>(arg0));
}
void GEMglTranslated :: yMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->yMess ( static_cast<t_float>(arg0));
}
void GEMglTranslated :: zMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->zMess ( static_cast<t_float>(arg0));
}
