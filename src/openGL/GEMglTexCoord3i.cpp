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

#include "GEMglTexCoord3i.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglTexCoord3i , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTexCoord3i :: GEMglTexCoord3i	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		s((GLint)arg0), 
		t((GLint)arg1), 
		r((GLint)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("s"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("t"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("r"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglTexCoord3i :: ~GEMglTexCoord3i () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglTexCoord3i :: render(GemState *state) {
	glTexCoord3i (s, t, r);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglTexCoord3i :: sMess (t_float arg1) {	// FUN
	s = (GLint)arg1;
	setModified();
}

void GEMglTexCoord3i :: tMess (t_float arg1) {	// FUN
	t = (GLint)arg1;
	setModified();
}

void GEMglTexCoord3i :: rMess (t_float arg1) {	// FUN
	r = (GLint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTexCoord3i :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglTexCoord3i::sMessCallback,  	gensym("s"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexCoord3i::tMessCallback,  	gensym("t"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexCoord3i::rMessCallback,  	gensym("r"), A_DEFFLOAT, A_NULL);
};

void GEMglTexCoord3i :: sMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->sMess ( (t_float)    arg0);
}
void GEMglTexCoord3i :: tMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->tMess ( (t_float)    arg0);
}
void GEMglTexCoord3i :: rMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->rMess ( (t_float)    arg0);
}
