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

#include "GEMglColor4us.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglColor4us , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglColor4us :: GEMglColor4us	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
		red((GLushort)arg0), 
		green((GLushort)arg1), 
		blue((GLushort)arg2), 
		alpha((GLushort)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("red"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("green"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("blue"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("alpha"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglColor4us :: ~GEMglColor4us () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglColor4us :: render(GemState *state) {
	glColor4us (red, green, blue, alpha);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglColor4us :: redMess (t_float arg1) {	// FUN
	red = (GLushort)arg1;
	setModified();
}

void GEMglColor4us :: greenMess (t_float arg1) {	// FUN
	green = (GLushort)arg1;
	setModified();
}

void GEMglColor4us :: blueMess (t_float arg1) {	// FUN
	blue = (GLushort)arg1;
	setModified();
}

void GEMglColor4us :: alphaMess (t_float arg1) {	// FUN
	alpha = (GLushort)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglColor4us :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglColor4us::redMessCallback,  	gensym("red"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglColor4us::greenMessCallback,  	gensym("green"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglColor4us::blueMessCallback,  	gensym("blue"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglColor4us::alphaMessCallback,  	gensym("alpha"), A_DEFFLOAT, A_NULL);
};

void GEMglColor4us :: redMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->redMess ( (t_float)    arg0);
}
void GEMglColor4us :: greenMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->greenMess ( (t_float)    arg0);
}
void GEMglColor4us :: blueMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->blueMess ( (t_float)    arg0);
}
void GEMglColor4us :: alphaMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->alphaMess ( (t_float)    arg0);
}
