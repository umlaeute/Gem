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

#include "GEMglLoadName.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglLoadName , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglLoadName :: GEMglLoadName	(t_floatarg arg0=0) :
		name((GLuint)arg0)
{
#ifndef GL_VERSION_1_1
        error("GEMglLoadName: GEM was compiled without GL_VERSION_1_1");
        error("GEMglLoadName: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("name"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglLoadName :: ~GEMglLoadName () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglLoadName :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glLoadName (name);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglLoadName :: nameMess (t_float arg1) {	// FUN
	name = (GLuint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglLoadName :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglLoadName::nameMessCallback,  	gensym("name"), A_DEFFLOAT, A_NULL);
}

void GEMglLoadName :: nameMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->nameMess ( (t_float)    arg0);
}
