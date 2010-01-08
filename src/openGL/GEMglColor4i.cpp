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

#include "GEMglColor4i.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglColor4i , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglColor4i :: GEMglColor4i	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
		red((GLint)arg0), 
		green((GLint)arg1), 
		blue((GLint)arg2), 
		alpha((GLint)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("red"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("green"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("blue"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("alpha"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglColor4i :: ~GEMglColor4i () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglColor4i :: render(GemState *state) {
	glColor4i (red, green, blue, alpha);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglColor4i :: redMess (t_float arg1) {	// FUN
	red = (GLint)arg1;
	setModified();
}

void GEMglColor4i :: greenMess (t_float arg1) {	// FUN
	green = (GLint)arg1;
	setModified();
}

void GEMglColor4i :: blueMess (t_float arg1) {	// FUN
	blue = (GLint)arg1;
	setModified();
}

void GEMglColor4i :: alphaMess (t_float arg1) {	// FUN
	alpha = (GLint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglColor4i :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglColor4i::redMessCallback),  	gensym("red"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglColor4i::greenMessCallback),  	gensym("green"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglColor4i::blueMessCallback),  	gensym("blue"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglColor4i::alphaMessCallback),  	gensym("alpha"), A_DEFFLOAT, A_NULL);
};

void GEMglColor4i :: redMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->redMess ( (t_float)    arg0);
}
void GEMglColor4i :: greenMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->greenMess ( (t_float)    arg0);
}
void GEMglColor4i :: blueMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->blueMess ( (t_float)    arg0);
}
void GEMglColor4i :: alphaMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->alphaMess ( (t_float)    arg0);
}
