////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//	zmoelnig@iem.kug.ac.at
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
//  this file has been generated...
////////////////////////////////////////////////////////

#include "GEMglGetString.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglGetString , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglGetString :: GEMglGetString	(t_floatarg arg0=0) :
  name((GLenum)arg0) {
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("name"));
  m_outlet=outlet_new(this->x_obj, 0);
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglGetString :: ~GEMglGetString () {
  inlet_free (m_inlet);
  outlet_free(m_outlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglGetString :: render(GemState *state) {
  const GLubyte* test=glGetString (name);
  outlet_symbol(m_outlet, gensym((char*)test));
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglGetString :: nameMess (t_float arg1) {	// FUN
  name = (GLenum)arg1;
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//
void GEMglGetString :: obj_setupCallback(t_class *classPtr) {
  class_addmethod(classPtr, (t_method)&GEMglGetString::nameMessCallback, gensym("name"), A_DEFFLOAT, A_NULL);
}

void GEMglGetString :: nameMessCallback (void* data, t_floatarg arg0){
  GetMyClass(data)->nameMess (arg0);
}
