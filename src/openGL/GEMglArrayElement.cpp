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

#include "GEMglArrayElement.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglArrayElement , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglArrayElement :: GEMglArrayElement	(t_floatarg arg0=0) :
		i((GLint)arg0)
{
#ifndef GL_VERSION_1_1
        error("GEMglArrayElement: GEM was compiled without GL_VERSION_1_1");
        error("GEMglArrayElement: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("i"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglArrayElement :: ~GEMglArrayElement () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglArrayElement :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glArrayElement (i);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglArrayElement :: iMess (t_float arg1) {	// FUN
	i = (GLint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglArrayElement :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglArrayElement::iMessCallback,  	gensym("i"), A_DEFFLOAT, A_NULL);
}

void GEMglArrayElement :: iMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->iMess ( (t_float)    arg0);
}
