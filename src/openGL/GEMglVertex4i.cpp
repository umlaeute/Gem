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

#include "GEMglVertex4i.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglVertex4i , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglVertex4i :: GEMglVertex4i	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
		x((GLint)arg0), 
		y((GLint)arg1), 
		z((GLint)arg2), 
		w((GLint)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("z"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("w"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglVertex4i :: ~GEMglVertex4i () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglVertex4i :: render(GemState *state) {
	glVertex4i (x, y, z, w);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglVertex4i :: xMess (t_float arg1) {	// FUN
	x = (GLint)arg1;
	setModified();
}

void GEMglVertex4i :: yMess (t_float arg1) {	// FUN
	y = (GLint)arg1;
	setModified();
}

void GEMglVertex4i :: zMess (t_float arg1) {	// FUN
	z = (GLint)arg1;
	setModified();
}

void GEMglVertex4i :: wMess (t_float arg1) {	// FUN
	w = (GLint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglVertex4i :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglVertex4i::xMessCallback,  	gensym("x"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglVertex4i::yMessCallback,  	gensym("y"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglVertex4i::zMessCallback,  	gensym("z"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglVertex4i::wMessCallback,  	gensym("w"), A_DEFFLOAT, A_NULL);
};

void GEMglVertex4i :: xMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->xMess ( (t_float)    arg0);
}
void GEMglVertex4i :: yMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->yMess ( (t_float)    arg0);
}
void GEMglVertex4i :: zMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->zMess ( (t_float)    arg0);
}
void GEMglVertex4i :: wMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->wMess ( (t_float)    arg0);
}
