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

#include "GEMglRasterPos3i.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglRasterPos3i , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglRasterPos3i :: GEMglRasterPos3i	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		x((GLint)arg0), 
		y((GLint)arg1), 
		z((GLint)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("z"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglRasterPos3i :: ~GEMglRasterPos3i () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglRasterPos3i :: render(GemState *state) {
	glRasterPos3i (x, y, z);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglRasterPos3i :: xMess (t_float arg1) {	// FUN
	x = (GLint)arg1;
	setModified();
}

void GEMglRasterPos3i :: yMess (t_float arg1) {	// FUN
	y = (GLint)arg1;
	setModified();
}

void GEMglRasterPos3i :: zMess (t_float arg1) {	// FUN
	z = (GLint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglRasterPos3i :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglRasterPos3i::xMessCallback,  	gensym("x"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglRasterPos3i::yMessCallback,  	gensym("y"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglRasterPos3i::zMessCallback,  	gensym("z"), A_DEFFLOAT, A_NULL);
};

void GEMglRasterPos3i :: xMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->xMess ( (t_float)    arg0);
}
void GEMglRasterPos3i :: yMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->yMess ( (t_float)    arg0);
}
void GEMglRasterPos3i :: zMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->zMess ( (t_float)    arg0);
}
