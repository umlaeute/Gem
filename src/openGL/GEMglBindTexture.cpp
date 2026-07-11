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

#include "GEMglBindTexture.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglBindTexture, t_floatarg, A_DEFFLOAT,
                              t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglBindTexture :: GEMglBindTexture    (t_floatarg arg0,
    t_floatarg arg1) :
  target(static_cast<GLenum>(arg0)),
  texture(static_cast<GLuint>(arg1))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("target"));
  m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("texture"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglBindTexture :: ~GEMglBindTexture ()
{
  inlet_free(m_inlet[0]);
  inlet_free(m_inlet[1]);
}
//////////////////
// extension check
bool GEMglBindTexture :: isRunnable(void)
{
  if(GLEW_VERSION_1_1) {
    return true;
  }
  error("your system does not support OpenGL-1.1");
  return false;
}
/////////////////////////////////////////////////////////
// Render
//
void GEMglBindTexture :: render(GemState *state)
{
  glBindTexture (target, texture);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglBindTexture :: targetMess (t_float arg1)      // FUN
{
  target = static_cast<GLenum>(arg1);
  setModified();
}

void GEMglBindTexture :: textureMess (t_float arg1)     // FUN
{
  texture = static_cast<GLuint>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglBindTexture :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglBindTexture::targetMessCallback),
                  gensym("target"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglBindTexture::textureMessCallback),
                  gensym("texture"), A_DEFFLOAT, A_NULL);
}

void GEMglBindTexture :: targetMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->targetMess ( static_cast<t_float>(arg0));
}
void GEMglBindTexture :: textureMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->textureMess ( static_cast<t_float>(arg0));
}
