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

#include "GEMglFogi.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglFogi , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglFogi :: GEMglFogi	(t_floatarg arg0=0, t_floatarg arg1=0) :
		pname((GLenum)arg0), 
		param((GLint)arg1)
{
#ifndef GL_VERSION_1_1
        error("GEMglFogi: GEM was compiled without GL_VERSION_1_1");
        error("GEMglFogi: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("param"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglFogi :: ~GEMglFogi () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglFogi :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glFogi (pname, param);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglFogi :: pnameMess (t_float arg1) {	// FUN
	pname = (GLenum)arg1;
	setModified();
}

void GEMglFogi :: paramMess (t_float arg1) {	// FUN
	param = (GLint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglFogi :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglFogi::pnameMessCallback,  	gensym("pname"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglFogi::paramMessCallback,  	gensym("param"), A_DEFFLOAT, A_NULL);
}

void GEMglFogi :: pnameMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->pnameMess ( (t_float)    arg0);
}
void GEMglFogi :: paramMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->paramMess ( (t_float)    arg0);
}
