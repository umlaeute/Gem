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

#include "GEMglVertex2i.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglVertex2i , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglVertex2i :: GEMglVertex2i	(t_floatarg arg0=0, t_floatarg arg1=0) :
		x((GLint)arg0), 
		y((GLint)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglVertex2i :: ~GEMglVertex2i () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglVertex2i :: render(GemState *state) {
	glVertex2i (x, y);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglVertex2i :: xMess (t_float arg1) {	// FUN
	x = (GLint)arg1;
	setModified();
}

void GEMglVertex2i :: yMess (t_float arg1) {	// FUN
	y = (GLint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglVertex2i :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglVertex2i::xMessCallback,  	gensym("x"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglVertex2i::yMessCallback,  	gensym("y"), A_DEFFLOAT, A_NULL);
};

void GEMglVertex2i :: xMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->xMess ( (t_float)    arg0);
}
void GEMglVertex2i :: yMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->yMess ( (t_float)    arg0);
}
