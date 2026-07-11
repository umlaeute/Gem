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

#include "GEMglPixelTransferi.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglPixelTransferi, t_floatarg, A_DEFFLOAT,
                              t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglPixelTransferi :: GEMglPixelTransferi      (t_floatarg arg0,
    t_floatarg arg1) :
  pname(static_cast<GLenum>(arg0)),
  param(static_cast<GLint>(arg1))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("pname"));
  m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("param"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglPixelTransferi :: ~GEMglPixelTransferi ()
{
  inlet_free(m_inlet[0]);
  inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglPixelTransferi :: render(GemState *state)
{
  glPixelTransferi (pname, param);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglPixelTransferi :: pnameMess (t_float arg1)    // FUN
{
  pname = static_cast<GLenum>(arg1);
  setModified();
}

void GEMglPixelTransferi :: paramMess (t_float arg1)    // FUN
{
  param = static_cast<GLint>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglPixelTransferi :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglPixelTransferi::pnameMessCallback),
                  gensym("pname"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglPixelTransferi::paramMessCallback),
                  gensym("param"), A_DEFFLOAT, A_NULL);
};

void GEMglPixelTransferi :: pnameMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->pnameMess ( static_cast<t_float>(arg0));
}
void GEMglPixelTransferi :: paramMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->paramMess ( static_cast<t_float>(arg0));
}
