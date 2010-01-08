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

#include "GEMglColorMask.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglColorMask , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglColorMask :: GEMglColorMask	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
		red((GLboolean)arg0), 
		green((GLboolean)arg1), 
		blue((GLboolean)arg2), 
		alpha((GLboolean)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("red"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("green"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("blue"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("alpha"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglColorMask :: ~GEMglColorMask () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglColorMask :: render(GemState *state) {
	glColorMask (red, green, blue, alpha);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglColorMask :: redMess (t_float arg1) {	// FUN
	red = (GLboolean)arg1;
	setModified();
}

void GEMglColorMask :: greenMess (t_float arg1) {	// FUN
	green = (GLboolean)arg1;
	setModified();
}

void GEMglColorMask :: blueMess (t_float arg1) {	// FUN
	blue = (GLboolean)arg1;
	setModified();
}

void GEMglColorMask :: alphaMess (t_float arg1) {	// FUN
	alpha = (GLboolean)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglColorMask :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglColorMask::redMessCallback),  	gensym("red"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglColorMask::greenMessCallback),  	gensym("green"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglColorMask::blueMessCallback),  	gensym("blue"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglColorMask::alphaMessCallback),  	gensym("alpha"), A_DEFFLOAT, A_NULL);
};

void GEMglColorMask :: redMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->redMess ( (t_float)    arg0);
}
void GEMglColorMask :: greenMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->greenMess ( (t_float)    arg0);
}
void GEMglColorMask :: blueMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->blueMess ( (t_float)    arg0);
}
void GEMglColorMask :: alphaMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->alphaMess ( (t_float)    arg0);
}
