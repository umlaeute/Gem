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

#include "GEMglPushClientAttrib.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglPushClientAttrib , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglPushClientAttrib :: GEMglPushClientAttrib	(t_floatarg arg0=0) :
		mask((GLbitfield)arg0)
{
#ifndef GL_VERSION_1_1
        error("GEMglPushClientAttrib: GEM was compiled without GL_VERSION_1_1");
        error("GEMglPushClientAttrib: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mask"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglPushClientAttrib :: ~GEMglPushClientAttrib () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglPushClientAttrib :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glPushClientAttrib (mask);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglPushClientAttrib :: maskMess (t_float arg1) {	// FUN
	mask = (GLbitfield)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglPushClientAttrib :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglPushClientAttrib::maskMessCallback,  	gensym("mask"), A_DEFFLOAT, A_NULL);
}

void GEMglPushClientAttrib :: maskMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->maskMess ( (t_float)    arg0);
}
