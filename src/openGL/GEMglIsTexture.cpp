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

#include "GEMglIsTexture.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglIsTexture, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglIsTexture :: GEMglIsTexture        (t_floatarg arg0) : texture(
    static_cast<GLuint>(arg0))
{
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("texture"));
  m_outlet=outlet_new(this->x_obj, 0);
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglIsTexture :: ~GEMglIsTexture ()
{
  inlet_free(m_inlet);
  outlet_free(m_outlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglIsTexture :: render(GemState *state)
{
  GLboolean b = glIsTexture (texture);
  outlet_float(m_outlet, b?1.0:0.0);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglIsTexture :: textureMess (t_float arg1)       // FUN
{
  texture = static_cast<GLuint>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglIsTexture :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglIsTexture::textureMessCallback),
                  gensym("texture"), A_DEFFLOAT, A_NULL);
}

void GEMglIsTexture :: textureMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->textureMess (arg0);
}
