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

#include "GEMglColor3s.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglColor3s , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglColor3s :: GEMglColor3s	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		red((GLshort)arg0), 
		green((GLshort)arg1), 
		blue((GLshort)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("red"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("green"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("blue"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglColor3s :: ~GEMglColor3s () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglColor3s :: render(GemState *state) {
	glColor3s (red, green, blue);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglColor3s :: redMess (t_float arg1) {	// FUN
	red = (GLshort)arg1;
	setModified();
}

void GEMglColor3s :: greenMess (t_float arg1) {	// FUN
	green = (GLshort)arg1;
	setModified();
}

void GEMglColor3s :: blueMess (t_float arg1) {	// FUN
	blue = (GLshort)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglColor3s :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglColor3s::redMessCallback,  	gensym("red"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglColor3s::greenMessCallback,  	gensym("green"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglColor3s::blueMessCallback,  	gensym("blue"), A_DEFFLOAT, A_NULL);
};

void GEMglColor3s :: redMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->redMess ( (t_float)    arg0);
}
void GEMglColor3s :: greenMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->greenMess ( (t_float)    arg0);
}
void GEMglColor3s :: blueMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->blueMess ( (t_float)    arg0);
}
