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

#include "GEMglIndexubv.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglIndexubv , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglIndexubv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglIndexubv :: GEMglIndexubv	(t_floatarg arg0=0) {
#ifndef GL_VERSION_1_1
        error("GEMglIndexubv: GEM was compiled without GL_VERSION_1_1");
        error("GEMglIndexubv: therefore this object will do nothing");
#endif
	cMess(arg0);
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglIndexubv :: ~GEMglIndexubv () {
	inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglIndexubv :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glIndexubv (c);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglIndexubv :: cMess (t_float arg0) {	// FUN
  c[0]=(GLubyte)arg0;
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglIndexubv :: obj_setupCallback(t_class *classPtr) {
  class_addmethod(classPtr, (t_method)&GEMglIndexubv::cMessCallback,  	gensym("c"), A_DEFFLOAT, A_NULL);
}

void GEMglIndexubv :: cMessCallback (void* data, t_floatarg arg0) {
	GetMyClass(data)->cMess (arg0);
}
