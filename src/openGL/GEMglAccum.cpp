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

#include "GEMglAccum.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglAccum, t_floatarg, A_DEFFLOAT,
                              t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglAccum :: GEMglAccum        (t_floatarg arg0, t_floatarg arg1) :
  op(static_cast<GLenum>(arg0)),
  value(static_cast<GLfloat>(arg1))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("op"));
  m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("value"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglAccum :: ~GEMglAccum ()
{
  inlet_free(m_inlet[0]);
  inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglAccum :: render(GemState *state)
{
  glAccum (op, value);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglAccum :: opMess (t_float arg1)        // FUN
{
  op = static_cast<GLenum>(arg1);
  setModified();
}

void GEMglAccum :: valueMess (t_float arg1)     // FUN
{
  value = static_cast<GLfloat>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglAccum :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglAccum::opMessCallback),     gensym("op"),
                  A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglAccum::valueMessCallback),
                  gensym("value"), A_DEFFLOAT, A_NULL);
}

void GEMglAccum :: opMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->opMess ( static_cast<t_float>(arg0));
}
void GEMglAccum :: valueMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->valueMess ( static_cast<t_float>(arg0));
}
