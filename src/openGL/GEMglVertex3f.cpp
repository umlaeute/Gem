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

#include "GEMglVertex3f.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglVertex3f , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglVertex3f :: GEMglVertex3f	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		x((GLfloat)arg0), 
		y((GLfloat)arg1), 
		z((GLfloat)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("z"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglVertex3f :: ~GEMglVertex3f () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglVertex3f :: render(GemState *state) {
	glVertex3f (x, y, z);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglVertex3f :: xMess (t_float arg1) {	// FUN
	x = (GLfloat)arg1;
	setModified();
}

void GEMglVertex3f :: yMess (t_float arg1) {	// FUN
	y = (GLfloat)arg1;
	setModified();
}

void GEMglVertex3f :: zMess (t_float arg1) {	// FUN
	z = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglVertex3f :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglVertex3f::xMessCallback,  	gensym("x"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglVertex3f::yMessCallback,  	gensym("y"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglVertex3f::zMessCallback,  	gensym("z"), A_DEFFLOAT, A_NULL);
};

void GEMglVertex3f :: xMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->xMess ( (t_float)    arg0);
}
void GEMglVertex3f :: yMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->yMess ( (t_float)    arg0);
}
void GEMglVertex3f :: zMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->zMess ( (t_float)    arg0);
}
