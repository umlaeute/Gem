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

#include "GEMglNormal3i.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglNormal3i , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglNormal3i :: GEMglNormal3i	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		nx((GLint)arg0), 
		ny((GLint)arg1), 
		nz((GLint)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("nx"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ny"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("nz"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglNormal3i :: ~GEMglNormal3i () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglNormal3i :: render(GemState *state) {
	glNormal3i (nx, ny, nz);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglNormal3i :: nxMess (t_float arg1) {	// FUN
	nx = (GLint)arg1;
	setModified();
}

void GEMglNormal3i :: nyMess (t_float arg1) {	// FUN
	ny = (GLint)arg1;
	setModified();
}

void GEMglNormal3i :: nzMess (t_float arg1) {	// FUN
	nz = (GLint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglNormal3i :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglNormal3i::nxMessCallback,  	gensym("nx"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglNormal3i::nyMessCallback,  	gensym("ny"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglNormal3i::nzMessCallback,  	gensym("nz"), A_DEFFLOAT, A_NULL);
};

void GEMglNormal3i :: nxMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->nxMess ( (t_float)    arg0);
}
void GEMglNormal3i :: nyMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->nyMess ( (t_float)    arg0);
}
void GEMglNormal3i :: nzMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->nzMess ( (t_float)    arg0);
}
