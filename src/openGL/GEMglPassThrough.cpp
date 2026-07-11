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

#include "GEMglPassThrough.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglPassThrough, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglPassThrough :: GEMglPassThrough    (t_floatarg arg0) :
  token(static_cast<GLfloat>(arg0))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("token"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglPassThrough :: ~GEMglPassThrough ()
{
  inlet_free(m_inlet[0]);
}

//////////////////
// extension check
bool GEMglPassThrough :: isRunnable(void)
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
void GEMglPassThrough :: render(GemState *state)
{
  glPassThrough (token);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglPassThrough :: tokenMess (t_float arg1)       // FUN
{
  token = static_cast<GLfloat>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglPassThrough :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglPassThrough::tokenMessCallback),
                  gensym("token"), A_DEFFLOAT, A_NULL);
}

void GEMglPassThrough :: tokenMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->tokenMess ( static_cast<t_float>(arg0));
}
