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

#include "GEMglEvalPoint1.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglEvalPoint1 , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglEvalPoint1 :: GEMglEvalPoint1	(t_floatarg arg0=0) :
		i((GLint)arg0)
{
#ifndef GL_VERSION_1_1
        error("GEMglEvalPoint1: GEM was compiled without GL_VERSION_1_1");
        error("GEMglEvalPoint1: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("i"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglEvalPoint1 :: ~GEMglEvalPoint1 () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglEvalPoint1 :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glEvalPoint1 (i);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglEvalPoint1 :: iMess (t_float arg1) {	// FUN
	i = (GLint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglEvalPoint1 :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglEvalPoint1::iMessCallback,  	gensym("i"), A_DEFFLOAT, A_NULL);
}

void GEMglEvalPoint1 :: iMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->iMess ( (t_float)    arg0);
}
