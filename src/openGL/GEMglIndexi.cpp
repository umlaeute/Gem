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

#include "GEMglIndexi.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglIndexi , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglIndexi :: GEMglIndexi	(t_floatarg arg0=0) :
		c((GLint)arg0)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("c"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglIndexi :: ~GEMglIndexi () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglIndexi :: render(GemState *state) {
	glIndexi (c);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglIndexi :: cMess (t_float arg1) {	// FUN
	c = (GLint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglIndexi :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglIndexi::cMessCallback,  	gensym("c"), A_DEFFLOAT, A_NULL);
};

void GEMglIndexi :: cMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->cMess ( (t_float)    arg0);
}
