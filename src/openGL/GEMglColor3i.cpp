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

#include "GEMglColor3i.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglColor3i , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglColor3i :: GEMglColor3i	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		red((GLint)arg0), 
		green((GLint)arg1), 
		blue((GLint)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("red"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("green"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("blue"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglColor3i :: ~GEMglColor3i () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglColor3i :: render(GemState *state) {
	glColor3i (red, green, blue);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglColor3i :: redMess (t_float arg1) {	// FUN
	red = (GLint)arg1;
	setModified();
}

void GEMglColor3i :: greenMess (t_float arg1) {	// FUN
	green = (GLint)arg1;
	setModified();
}

void GEMglColor3i :: blueMess (t_float arg1) {	// FUN
	blue = (GLint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglColor3i :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglColor3i::redMessCallback,  	gensym("red"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglColor3i::greenMessCallback,  	gensym("green"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglColor3i::blueMessCallback,  	gensym("blue"), A_DEFFLOAT, A_NULL);
};

void GEMglColor3i :: redMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->redMess ( (t_float)    arg0);
}
void GEMglColor3i :: greenMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->greenMess ( (t_float)    arg0);
}
void GEMglColor3i :: blueMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->blueMess ( (t_float)    arg0);
}
