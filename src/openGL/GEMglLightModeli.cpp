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

#include "GEMglLightModeli.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglLightModeli , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglLightModeli :: GEMglLightModeli	(t_floatarg arg0=0, t_floatarg arg1=0) :
		pname((GLenum)arg0), 
		param((GLint)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("param"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglLightModeli :: ~GEMglLightModeli () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglLightModeli :: render(GemState *state) {
	glLightModeli (pname, param);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglLightModeli :: pnameMess (t_float arg1) {	// FUN
	pname = (GLenum)arg1;
	setModified();
}

void GEMglLightModeli :: paramMess (t_float arg1) {	// FUN
	param = (GLint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglLightModeli :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglLightModeli::pnameMessCallback,  	gensym("pname"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglLightModeli::paramMessCallback,  	gensym("param"), A_DEFFLOAT, A_NULL);
};

void GEMglLightModeli :: pnameMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->pnameMess ( (t_float)    arg0);
}
void GEMglLightModeli :: paramMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->paramMess ( (t_float)    arg0);
}
