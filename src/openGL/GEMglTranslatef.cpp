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

#include "GEMglTranslatef.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglTranslatef , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTranslatef :: GEMglTranslatef	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
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
GEMglTranslatef :: ~GEMglTranslatef () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglTranslatef :: render(GemState *state) {
	glTranslatef (x, y, z);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglTranslatef :: xMess (t_float arg1) {	// FUN
	x = (GLfloat)arg1;
	setModified();
}

void GEMglTranslatef :: yMess (t_float arg1) {	// FUN
	y = (GLfloat)arg1;
	setModified();
}

void GEMglTranslatef :: zMess (t_float arg1) {	// FUN
	z = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTranslatef :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglTranslatef::xMessCallback,  	gensym("x"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTranslatef::yMessCallback,  	gensym("y"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTranslatef::zMessCallback,  	gensym("z"), A_DEFFLOAT, A_NULL);
};

void GEMglTranslatef :: xMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->xMess ( (t_float)    arg0);
}
void GEMglTranslatef :: yMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->yMess ( (t_float)    arg0);
}
void GEMglTranslatef :: zMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->zMess ( (t_float)    arg0);
}
