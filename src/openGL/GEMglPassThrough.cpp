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

#include "GEMglPassThrough.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglPassThrough , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglPassThrough :: GEMglPassThrough	(t_floatarg arg0=0) :
		token((GLfloat)arg0)
{
#ifndef GL_VERSION_1_1
        error("GEMglPassThrough: GEM was compiled without GL_VERSION_1_1");
        error("GEMglPassThrough: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("token"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglPassThrough :: ~GEMglPassThrough () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglPassThrough :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glPassThrough (token);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglPassThrough :: tokenMess (t_float arg1) {	// FUN
	token = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglPassThrough :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglPassThrough::tokenMessCallback,  	gensym("token"), A_DEFFLOAT, A_NULL);
}

void GEMglPassThrough :: tokenMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->tokenMess ( (t_float)    arg0);
}
