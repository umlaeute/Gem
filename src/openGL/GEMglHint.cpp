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

#include "GEMglHint.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglHint , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglHint :: GEMglHint	(t_floatarg arg0=0, t_floatarg arg1=0) :
		target((GLenum)arg0), 
		mode((GLenum)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("target"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mode"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglHint :: ~GEMglHint () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglHint :: render(GemState *state) {
	glHint (target, mode);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglHint :: targetMess (t_float arg1) {	// FUN
	target = (GLenum)arg1;
	setModified();
}

void GEMglHint :: modeMess (t_float arg1) {	// FUN
	mode = (GLenum)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglHint :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglHint::targetMessCallback,  	gensym("target"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglHint::modeMessCallback,  	gensym("mode"), A_DEFFLOAT, A_NULL);
};

void GEMglHint :: targetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->targetMess ( (t_float)    arg0);
}
void GEMglHint :: modeMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->modeMess ( (t_float)    arg0);
}
