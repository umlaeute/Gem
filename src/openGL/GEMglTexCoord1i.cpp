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

#include "GEMglTexCoord1i.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglTexCoord1i , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTexCoord1i :: GEMglTexCoord1i	(t_floatarg arg0=0) :
		s((GLint)arg0)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("s"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglTexCoord1i :: ~GEMglTexCoord1i () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglTexCoord1i :: render(GemState *state) {
	glTexCoord1i (s);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglTexCoord1i :: sMess (t_float arg1) {	// FUN
	s = (GLint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTexCoord1i :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglTexCoord1i::sMessCallback,  	gensym("s"), A_DEFFLOAT, A_NULL);
};

void GEMglTexCoord1i :: sMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->sMess ( (t_float)    arg0);
}
