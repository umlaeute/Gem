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

#include "GEMglFrontFace.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglFrontFace , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglFrontFace :: GEMglFrontFace	(t_floatarg arg0=0) :
		mode((GLenum)arg0)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mode"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglFrontFace :: ~GEMglFrontFace () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglFrontFace :: render(GemState *state) {
	glFrontFace (mode);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglFrontFace :: modeMess (t_float arg1) {	// FUN
	mode = (GLenum)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglFrontFace :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglFrontFace::modeMessCallback,  	gensym("mode"), A_DEFFLOAT, A_NULL);
};

void GEMglFrontFace :: modeMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->modeMess ( (t_float)    arg0);
}
