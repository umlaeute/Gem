////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//	zmoelnig@iem.kug.ac.at
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
//  this file has been generated...
////////////////////////////////////////////////////////

#include "GEMglRasterPos2fv.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglRasterPos2fv , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglRasterPos2fv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglRasterPos2fv :: GEMglRasterPos2fv	(t_floatarg arg0=0, t_floatarg arg1=0) {
vMess(arg0, arg1);
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglRasterPos2fv :: ~GEMglRasterPos2fv () {
	inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglRasterPos2fv :: render(GemState *state) {
	glRasterPos2fv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglRasterPos2fv :: vMess (t_float arg0, t_float arg1) {	// FUN
	v[0]=(GLfloat)arg0;
	v[1]=(GLfloat)arg1;
	setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglRasterPos2fv :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglRasterPos2fv::vMessCallback,  	gensym("v"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void GEMglRasterPos2fv :: vMessCallback (void* data, t_floatarg arg0, t_floatarg arg1) {
	GetMyClass(data)->vMess ( arg0, arg1);
}
