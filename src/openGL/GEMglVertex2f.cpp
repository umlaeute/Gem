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

#include "GEMglVertex2f.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglVertex2f , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglVertex2f :: GEMglVertex2f	(t_floatarg arg0=0, t_floatarg arg1=0) :
		x((GLfloat)arg0), 
		y((GLfloat)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglVertex2f :: ~GEMglVertex2f () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglVertex2f :: render(GemState *state) {
	glVertex2f (x, y);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglVertex2f :: xMess (t_float arg1) {	// FUN
	x = (GLfloat)arg1;
	setModified();
}

void GEMglVertex2f :: yMess (t_float arg1) {	// FUN
	y = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglVertex2f :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglVertex2f::xMessCallback,  	gensym("x"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglVertex2f::yMessCallback,  	gensym("y"), A_DEFFLOAT, A_NULL);
};

void GEMglVertex2f :: xMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->xMess ( (t_float)    arg0);
}
void GEMglVertex2f :: yMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->yMess ( (t_float)    arg0);
}
