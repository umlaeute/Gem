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

#include "GEMglRasterPos4f.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglRasterPos4f , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglRasterPos4f :: GEMglRasterPos4f	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
		x((GLfloat)arg0), 
		y((GLfloat)arg1), 
		z((GLfloat)arg2), 
		w((GLfloat)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("z"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("w"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglRasterPos4f :: ~GEMglRasterPos4f () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglRasterPos4f :: render(GemState *state) {
	glRasterPos4f (x, y, z, w);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglRasterPos4f :: xMess (t_float arg1) {	// FUN
	x = (GLfloat)arg1;
	setModified();
}

void GEMglRasterPos4f :: yMess (t_float arg1) {	// FUN
	y = (GLfloat)arg1;
	setModified();
}

void GEMglRasterPos4f :: zMess (t_float arg1) {	// FUN
	z = (GLfloat)arg1;
	setModified();
}

void GEMglRasterPos4f :: wMess (t_float arg1) {	// FUN
	w = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglRasterPos4f :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglRasterPos4f::xMessCallback,  	gensym("x"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglRasterPos4f::yMessCallback,  	gensym("y"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglRasterPos4f::zMessCallback,  	gensym("z"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglRasterPos4f::wMessCallback,  	gensym("w"), A_DEFFLOAT, A_NULL);
};

void GEMglRasterPos4f :: xMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->xMess ( (t_float)    arg0);
}
void GEMglRasterPos4f :: yMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->yMess ( (t_float)    arg0);
}
void GEMglRasterPos4f :: zMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->zMess ( (t_float)    arg0);
}
void GEMglRasterPos4f :: wMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->wMess ( (t_float)    arg0);
}
