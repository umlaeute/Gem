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

#include "GEMglEnable.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglEnable , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglEnable :: GEMglEnable	(t_floatarg arg0=0) :
		cap((GLenum)arg0)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("cap"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglEnable :: ~GEMglEnable () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglEnable :: render(GemState *state) {
	glEnable (cap);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglEnable :: capMess (t_float arg1) {	// FUN
	cap = (GLenum)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglEnable :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglEnable::capMessCallback,  	gensym("cap"), A_DEFFLOAT, A_NULL);
};

void GEMglEnable :: capMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->capMess ( (t_float)    arg0);
}
