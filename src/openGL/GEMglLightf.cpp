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

#include "GEMglLightf.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglLightf , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglLightf :: GEMglLightf	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		light((GLenum)arg0), 
		pname((GLenum)arg1), 
		param((GLfloat)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("light"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("param"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglLightf :: ~GEMglLightf () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglLightf :: render(GemState *state) {
	glLightf (light, pname, param);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglLightf :: lightMess (t_float arg1) {	// FUN
	light = (GLenum)arg1;
	setModified();
}

void GEMglLightf :: pnameMess (t_float arg1) {	// FUN
	pname = (GLenum)arg1;
	setModified();
}

void GEMglLightf :: paramMess (t_float arg1) {	// FUN
	param = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglLightf :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglLightf::lightMessCallback,  	gensym("light"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglLightf::pnameMessCallback,  	gensym("pname"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglLightf::paramMessCallback,  	gensym("param"), A_DEFFLOAT, A_NULL);
};

void GEMglLightf :: lightMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->lightMess ( (t_float)    arg0);
}
void GEMglLightf :: pnameMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->pnameMess ( (t_float)    arg0);
}
void GEMglLightf :: paramMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->paramMess ( (t_float)    arg0);
}
