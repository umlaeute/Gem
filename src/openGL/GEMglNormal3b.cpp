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

#include "GEMglNormal3b.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglNormal3b , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglNormal3b :: GEMglNormal3b	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		nx((GLbyte)arg0), 
		ny((GLbyte)arg1), 
		nz((GLbyte)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("nx"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ny"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("nz"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglNormal3b :: ~GEMglNormal3b () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglNormal3b :: render(GemState *state) {
	glNormal3b (nx, ny, nz);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglNormal3b :: nxMess (t_float arg1) {	// FUN
	nx = (GLbyte)arg1;
	setModified();
}

void GEMglNormal3b :: nyMess (t_float arg1) {	// FUN
	ny = (GLbyte)arg1;
	setModified();
}

void GEMglNormal3b :: nzMess (t_float arg1) {	// FUN
	nz = (GLbyte)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglNormal3b :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglNormal3b::nxMessCallback,  	gensym("nx"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglNormal3b::nyMessCallback,  	gensym("ny"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglNormal3b::nzMessCallback,  	gensym("nz"), A_DEFFLOAT, A_NULL);
};

void GEMglNormal3b :: nxMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->nxMess ( (t_float)    arg0);
}
void GEMglNormal3b :: nyMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->nyMess ( (t_float)    arg0);
}
void GEMglNormal3b :: nzMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->nzMess ( (t_float)    arg0);
}
