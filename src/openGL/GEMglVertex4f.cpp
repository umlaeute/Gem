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

#include "GEMglVertex4f.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglVertex4f , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglVertex4f :: GEMglVertex4f	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
		x(static_cast<GLfloat>(arg0)), 
		y(static_cast<GLfloat>(arg1)), 
		z(static_cast<GLfloat>(arg2)), 
		w(static_cast<GLfloat>(arg3))
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("z"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("w"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglVertex4f :: ~GEMglVertex4f () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglVertex4f :: render(GemState *state) {
	glVertex4f (x, y, z, w);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglVertex4f :: xMess (t_float arg1) {	// FUN
	x = static_cast<GLfloat>(arg1);
	setModified();
}

void GEMglVertex4f :: yMess (t_float arg1) {	// FUN
	y = static_cast<GLfloat>(arg1);
	setModified();
}

void GEMglVertex4f :: zMess (t_float arg1) {	// FUN
	z = static_cast<GLfloat>(arg1);
	setModified();
}

void GEMglVertex4f :: wMess (t_float arg1) {	// FUN
	w = static_cast<GLfloat>(arg1);
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglVertex4f :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglVertex4f::xMessCallback),  	gensym("x"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglVertex4f::yMessCallback),  	gensym("y"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglVertex4f::zMessCallback),  	gensym("z"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglVertex4f::wMessCallback),  	gensym("w"), A_DEFFLOAT, A_NULL);
};

void GEMglVertex4f :: xMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->xMess ( static_cast<t_float>(arg0));
}
void GEMglVertex4f :: yMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->yMess ( static_cast<t_float>(arg0));
}
void GEMglVertex4f :: zMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->zMess ( static_cast<t_float>(arg0));
}
void GEMglVertex4f :: wMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->wMess ( static_cast<t_float>(arg0));
}
