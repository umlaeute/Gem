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

#include "GEMglClearColor.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglClearColor , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglClearColor :: GEMglClearColor	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
		red((GLclampf)arg0), 
		green((GLclampf)arg1), 
		blue((GLclampf)arg2), 
		alpha((GLclampf)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("red"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("green"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("blue"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("alpha"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglClearColor :: ~GEMglClearColor () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglClearColor :: render(GemState *state) {
	glClearColor (red, green, blue, alpha);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglClearColor :: redMess (t_float arg1) {	// FUN
	red = (GLclampf)arg1;
	setModified();
}

void GEMglClearColor :: greenMess (t_float arg1) {	// FUN
	green = (GLclampf)arg1;
	setModified();
}

void GEMglClearColor :: blueMess (t_float arg1) {	// FUN
	blue = (GLclampf)arg1;
	setModified();
}

void GEMglClearColor :: alphaMess (t_float arg1) {	// FUN
	alpha = (GLclampf)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglClearColor :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglClearColor::redMessCallback,  	gensym("red"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglClearColor::greenMessCallback,  	gensym("green"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglClearColor::blueMessCallback,  	gensym("blue"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglClearColor::alphaMessCallback,  	gensym("alpha"), A_DEFFLOAT, A_NULL);
};

void GEMglClearColor :: redMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->redMess ( (t_float)    arg0);
}
void GEMglClearColor :: greenMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->greenMess ( (t_float)    arg0);
}
void GEMglClearColor :: blueMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->blueMess ( (t_float)    arg0);
}
void GEMglClearColor :: alphaMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->alphaMess ( (t_float)    arg0);
}
