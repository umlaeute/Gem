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

#include "GEMglIsEnabled.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglIsEnabled , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglIsEnabled :: GEMglIsEnabled	(t_floatarg arg0=0) : cap((GLenum)arg0) {
#ifndef GL_VERSION_1_1
  error("GEMglIsEnabled: GEM was compiled without GL_VERSION_1_1");
  error("GEMglIsEnabled: therefore this object will do nothing");
#endif
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("cap"));
  m_outlet=outlet_new(this->x_obj, 0);
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglIsEnabled :: ~GEMglIsEnabled () {
  inlet_free(m_inlet);
  outlet_free(m_outlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglIsEnabled :: render(GemState *state) {
  GLboolean b = glIsEnabled (cap);
  outlet_float(m_outlet, b?1.0:0.0);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglIsEnabled :: capMess (t_float arg1) {	// FUN
  cap = (GLenum)arg1;
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//
void GEMglIsEnabled :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglIsEnabled::capMessCallback, gensym("cap"), A_DEFFLOAT, A_NULL);
}

void GEMglIsEnabled :: capMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->capMess ( arg0);
}
