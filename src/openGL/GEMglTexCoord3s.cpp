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

#include "GEMglTexCoord3s.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglTexCoord3s , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTexCoord3s :: GEMglTexCoord3s	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		s((GLshort)arg0), 
		t((GLshort)arg1), 
		r((GLshort)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("s"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("t"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("r"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglTexCoord3s :: ~GEMglTexCoord3s () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglTexCoord3s :: render(GemState *state) {
	glTexCoord3s (s, t, r);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglTexCoord3s :: sMess (t_float arg1) {	// FUN
	s = (GLshort)arg1;
	setModified();
}

void GEMglTexCoord3s :: tMess (t_float arg1) {	// FUN
	t = (GLshort)arg1;
	setModified();
}

void GEMglTexCoord3s :: rMess (t_float arg1) {	// FUN
	r = (GLshort)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTexCoord3s :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglTexCoord3s::sMessCallback),  	gensym("s"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglTexCoord3s::tMessCallback),  	gensym("t"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglTexCoord3s::rMessCallback),  	gensym("r"), A_DEFFLOAT, A_NULL);
};

void GEMglTexCoord3s :: sMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->sMess ( (t_float)    arg0);
}
void GEMglTexCoord3s :: tMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->tMess ( (t_float)    arg0);
}
void GEMglTexCoord3s :: rMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->rMess ( (t_float)    arg0);
}
