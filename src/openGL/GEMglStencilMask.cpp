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

#include "GEMglStencilMask.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglStencilMask , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglStencilMask :: GEMglStencilMask	(t_floatarg arg0=0) :
		mask((GLuint)arg0)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mask"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglStencilMask :: ~GEMglStencilMask () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglStencilMask :: render(GemState *state) {
	glStencilMask (mask);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglStencilMask :: maskMess (t_float arg1) {	// FUN
	mask = (GLuint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglStencilMask :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglStencilMask::maskMessCallback,  	gensym("mask"), A_DEFFLOAT, A_NULL);
};

void GEMglStencilMask :: maskMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->maskMess ( (t_float)    arg0);
}
