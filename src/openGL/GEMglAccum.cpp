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

#include "GEMglAccum.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglAccum , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglAccum :: GEMglAccum	(t_floatarg arg0=0, t_floatarg arg1=0) :
		op((GLenum)arg0), 
		value((GLfloat)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("op"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("value"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglAccum :: ~GEMglAccum () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglAccum :: render(GemState *state) {
	glAccum (op, value);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglAccum :: opMess (t_float arg1) {	// FUN
	op = (GLenum)arg1;
	setModified();
}

void GEMglAccum :: valueMess (t_float arg1) {	// FUN
	value = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglAccum :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglAccum::opMessCallback,  	gensym("op"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglAccum::valueMessCallback,  	gensym("value"), A_DEFFLOAT, A_NULL);
}

void GEMglAccum :: opMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->opMess ( (t_float)    arg0);
}
void GEMglAccum :: valueMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->valueMess ( (t_float)    arg0);
}
