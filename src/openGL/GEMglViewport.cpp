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

#include "GEMglViewport.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglViewport , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglViewport :: GEMglViewport	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
		x((GLint)arg0), 
		y((GLint)arg1), 
		width((GLsizei)arg2), 
		height((GLsizei)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("width"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("height"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglViewport :: ~GEMglViewport () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglViewport :: render(GemState *state) {
	glViewport (x, y, width, height);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglViewport :: xMess (t_float arg1) {	// FUN
	x = (GLint)arg1;
	setModified();
}

void GEMglViewport :: yMess (t_float arg1) {	// FUN
	y = (GLint)arg1;
	setModified();
}

void GEMglViewport :: widthMess (t_float arg1) {	// FUN
	width = (GLsizei)arg1;
	setModified();
}

void GEMglViewport :: heightMess (t_float arg1) {	// FUN
	height = (GLsizei)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglViewport :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglViewport::xMessCallback,  	gensym("x"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglViewport::yMessCallback,  	gensym("y"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglViewport::widthMessCallback,  	gensym("width"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglViewport::heightMessCallback,  	gensym("height"), A_DEFFLOAT, A_NULL);
};

void GEMglViewport :: xMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->xMess ( (t_float)    arg0);
}
void GEMglViewport :: yMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->yMess ( (t_float)    arg0);
}
void GEMglViewport :: widthMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->widthMess ( (t_float)    arg0);
}
void GEMglViewport :: heightMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->heightMess ( (t_float)    arg0);
}
