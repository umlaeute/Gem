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

#include "GEMglClearAccum.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglClearAccum , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglClearAccum :: GEMglClearAccum	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
		red((GLfloat)arg0), 
		green((GLfloat)arg1), 
		blue((GLfloat)arg2), 
		alpha((GLfloat)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("red"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("green"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("blue"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("alpha"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglClearAccum :: ~GEMglClearAccum () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglClearAccum :: render(GemState *state) {
	glClearAccum (red, green, blue, alpha);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglClearAccum :: redMess (t_float arg1) {	// FUN
	red = (GLfloat)arg1;
	setModified();
}

void GEMglClearAccum :: greenMess (t_float arg1) {	// FUN
	green = (GLfloat)arg1;
	setModified();
}

void GEMglClearAccum :: blueMess (t_float arg1) {	// FUN
	blue = (GLfloat)arg1;
	setModified();
}

void GEMglClearAccum :: alphaMess (t_float arg1) {	// FUN
	alpha = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglClearAccum :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglClearAccum::redMessCallback,  	gensym("red"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglClearAccum::greenMessCallback,  	gensym("green"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglClearAccum::blueMessCallback,  	gensym("blue"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglClearAccum::alphaMessCallback,  	gensym("alpha"), A_DEFFLOAT, A_NULL);
};

void GEMglClearAccum :: redMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->redMess ( (t_float)    arg0);
}
void GEMglClearAccum :: greenMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->greenMess ( (t_float)    arg0);
}
void GEMglClearAccum :: blueMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->blueMess ( (t_float)    arg0);
}
void GEMglClearAccum :: alphaMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->alphaMess ( (t_float)    arg0);
}
