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

#include "GEMglPatchParameteri.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglPatchParameteri, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglPatchParameteri :: GEMglPatchParameteri  (t_floatarg arg0) :
  param(static_cast<GLint>(arg0))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("param"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglPatchParameteri :: ~GEMglPatchParameteri ()
{
  inlet_free(m_inlet[0]);
}

//////////////////
// extension check
bool GEMglPatchParameteri :: isRunnable(void)
{
  if(GLEW_VERSION_1_1) { // TODO : put the correct version
    return true;
  }
  error("your system does not support OpenGL-1.1");
  return false;
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglPatchParameteri :: render(GemState *state)
{
  glPatchParameteri (GL_PATCH_VERTICES, param);  
}

/////////////////////////////////////////////////////////
// Variables
//

void GEMglPatchParameteri :: paramMess (t_float arg0)      // FUN
{
  param = static_cast<GLint>(arg0);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglPatchParameteri :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglPatchParameteri::paramMessCallback),
                  gensym("param"), A_DEFFLOAT, A_NULL);
}

void GEMglPatchParameteri :: paramMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->paramMess ( static_cast<t_float>(arg0));
}
