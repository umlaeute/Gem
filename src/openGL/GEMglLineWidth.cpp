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

#include "GEMglLineWidth.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglLineWidth , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglLineWidth :: GEMglLineWidth	(t_floatarg arg0=0) :
		width((GLfloat)arg0)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("width"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglLineWidth :: ~GEMglLineWidth () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglLineWidth :: render(GemState *state) {
	glLineWidth (width);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglLineWidth :: widthMess (t_float arg1) {	// FUN
	width = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglLineWidth :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglLineWidth::widthMessCallback,  	gensym("width"), A_DEFFLOAT, A_NULL);
};

void GEMglLineWidth :: widthMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->widthMess ( (t_float)    arg0);
}
