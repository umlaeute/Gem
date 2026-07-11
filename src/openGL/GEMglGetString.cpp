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

#include "GEMglGetString.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglGetString, t_floatarg, A_DEFFLOAT);
using namespace gem::utils::gl;

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglGetString :: GEMglGetString        (t_floatarg arg0) :
  name(static_cast<GLenum>(arg0))
{
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("name"));
  m_outlet=outlet_new(this->x_obj, 0);
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglGetString :: ~GEMglGetString ()
{
  inlet_free (m_inlet);
  outlet_free(m_outlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglGetString :: render(GemState *state)
{
  const char* test=reinterpret_cast<const char*>(glGetString (name));
  if(test!=NULL) {
    outlet_symbol(m_outlet, gensym(test));
  }
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglGetString :: nameMess (t_atom arg)    // FUN
{
  name = static_cast<GLenum>(getGLdefine(&arg));
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//
void GEMglGetString :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglGetString::nameMessCallback),
                  gensym("name"), A_GIMME, A_NULL);
}

void GEMglGetString :: nameMessCallback (void* data, t_symbol*, int argc,
    t_atom*argv)
{
  if(argc==1) {
    GetMyClass(data)->nameMess ( argv[0]);
  }
}
