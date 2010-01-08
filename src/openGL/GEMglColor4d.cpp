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

#include "GEMglColor4d.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglColor4d , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglColor4d :: GEMglColor4d	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
		red((GLdouble)arg0), 
		green((GLdouble)arg1), 
		blue((GLdouble)arg2), 
		alpha((GLdouble)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("red"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("green"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("blue"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("alpha"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglColor4d :: ~GEMglColor4d () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglColor4d :: render(GemState *state) {
	glColor4d (red, green, blue, alpha);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglColor4d :: redMess (t_float arg1) {	// FUN
	red = (GLdouble)arg1;
	setModified();
}

void GEMglColor4d :: greenMess (t_float arg1) {	// FUN
	green = (GLdouble)arg1;
	setModified();
}

void GEMglColor4d :: blueMess (t_float arg1) {	// FUN
	blue = (GLdouble)arg1;
	setModified();
}

void GEMglColor4d :: alphaMess (t_float arg1) {	// FUN
	alpha = (GLdouble)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglColor4d :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglColor4d::redMessCallback),  	gensym("red"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglColor4d::greenMessCallback),  	gensym("green"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglColor4d::blueMessCallback),  	gensym("blue"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglColor4d::alphaMessCallback),  	gensym("alpha"), A_DEFFLOAT, A_NULL);
};

void GEMglColor4d :: redMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->redMess ( (t_float)    arg0);
}
void GEMglColor4d :: greenMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->greenMess ( (t_float)    arg0);
}
void GEMglColor4d :: blueMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->blueMess ( (t_float)    arg0);
}
void GEMglColor4d :: alphaMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->alphaMess ( (t_float)    arg0);
}
