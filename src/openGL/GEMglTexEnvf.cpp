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

#include "GEMglTexEnvf.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglTexEnvf , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTexEnvf :: GEMglTexEnvf	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		target((GLenum)arg0), 
		pname((GLenum)arg1), 
		param((GLfloat)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("target"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("param"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglTexEnvf :: ~GEMglTexEnvf () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglTexEnvf :: render(GemState *state) {
	glTexEnvf (target, pname, param);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglTexEnvf :: targetMess (t_float arg1) {	// FUN
	target = (GLenum)arg1;
	setModified();
}

void GEMglTexEnvf :: pnameMess (t_float arg1) {	// FUN
	pname = (GLenum)arg1;
	setModified();
}

void GEMglTexEnvf :: paramMess (t_float arg1) {	// FUN
	param = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTexEnvf :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglTexEnvf::targetMessCallback,  	gensym("target"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexEnvf::pnameMessCallback,  	gensym("pname"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexEnvf::paramMessCallback,  	gensym("param"), A_DEFFLOAT, A_NULL);
};

void GEMglTexEnvf :: targetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->targetMess ( (t_float)    arg0);
}
void GEMglTexEnvf :: pnameMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->pnameMess ( (t_float)    arg0);
}
void GEMglTexEnvf :: paramMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->paramMess ( (t_float)    arg0);
}
