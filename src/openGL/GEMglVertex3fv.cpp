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

#include "GEMglVertex3fv.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglVertex3fv , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglVertex3fv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglVertex3fv :: GEMglVertex3fv	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) {
vMess(arg0, arg1, arg2);
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglVertex3fv :: ~GEMglVertex3fv () {
	inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglVertex3fv :: render(GemState *state) {
	glVertex3fv (v);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglVertex3fv :: vMess (t_float arg0, t_float arg1, t_float arg2) {	// FUN
	v[0]=(GLfloat)arg0;
	v[1]=(GLfloat)arg1;
	v[2]=(GLfloat)arg2;
	setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglVertex3fv :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglVertex3fv::vMessCallback,  	gensym("v"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void GEMglVertex3fv :: vMessCallback (void* data, t_floatarg arg0, t_floatarg arg1, t_floatarg arg2) {
	GetMyClass(data)->vMess ( arg0, arg1, arg2);
}
