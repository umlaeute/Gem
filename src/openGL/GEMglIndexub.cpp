////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//	zmoelnig@iem.kug.ac.at
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
//  this file has been generated...
////////////////////////////////////////////////////////

#include "GEMglIndexub.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglIndexub , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglIndexub :: GEMglIndexub	(t_floatarg arg0=0) :
		c((GLubyte)arg0)
{
#ifndef GL_VERSION_1_1
        error("GEMglIndexub: GEM was compiled without GL_VERSION_1_1");
        error("GEMglIndexub: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("c"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglIndexub :: ~GEMglIndexub () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglIndexub :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glIndexub (c);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglIndexub :: cMess (t_float arg1) {	// FUN
	c = (GLubyte)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglIndexub :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglIndexub::cMessCallback,  	gensym("c"), A_DEFFLOAT, A_NULL);
}

void GEMglIndexub :: cMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->cMess ( (t_float)    arg0);
}
