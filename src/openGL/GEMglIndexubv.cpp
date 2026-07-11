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

#include "GEMglIndexubv.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglIndexubv, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglIndexubv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglIndexubv :: GEMglIndexubv  (t_floatarg arg0)
{
  cMess(arg0);
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglIndexubv :: ~GEMglIndexubv ()
{
  inlet_free(m_inlet);
}

//////////////////
// extension check
bool GEMglIndexubv :: isRunnable(void)
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
void GEMglIndexubv :: render(GemState *state)
{
  glIndexubv (c);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglIndexubv :: cMess (t_float arg0)      // FUN
{
  c[0]=static_cast<GLubyte>(arg0);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglIndexubv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglIndexubv::cMessCallback),
                  gensym("c"), A_DEFFLOAT, A_NULL);
}

void GEMglIndexubv :: cMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->cMess (arg0);
}
