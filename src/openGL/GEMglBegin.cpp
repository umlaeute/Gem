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

#include "GEMglBegin.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglBegin , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglBegin :: GEMglBegin	(t_floatarg arg0=0) :
		mode((GLenum)arg0)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mode"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglBegin :: ~GEMglBegin () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglBegin :: render(GemState *state) {
	glBegin (mode);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglBegin :: modeMess (t_float arg1) {	// FUN
	mode = (GLenum)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglBegin :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglBegin::modeMessCallback,  	gensym("mode"), A_DEFFLOAT, A_NULL);
};

void GEMglBegin :: modeMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->modeMess ( (t_float)    arg0);
}
