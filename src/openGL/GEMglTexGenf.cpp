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

#include "GEMglTexGenf.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglTexGenf , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTexGenf :: GEMglTexGenf	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		coord((GLenum)arg0), 
		pname((GLenum)arg1), 
		param((GLfloat)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("coord"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("param"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglTexGenf :: ~GEMglTexGenf () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglTexGenf :: render(GemState *state) {
	glTexGenf (coord, pname, param);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglTexGenf :: coordMess (t_float arg1) {	// FUN
	coord = (GLenum)arg1;
	setModified();
}

void GEMglTexGenf :: pnameMess (t_float arg1) {	// FUN
	pname = (GLenum)arg1;
	setModified();
}

void GEMglTexGenf :: paramMess (t_float arg1) {	// FUN
	param = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTexGenf :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglTexGenf::coordMessCallback,  	gensym("coord"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexGenf::pnameMessCallback,  	gensym("pname"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexGenf::paramMessCallback,  	gensym("param"), A_DEFFLOAT, A_NULL);
};

void GEMglTexGenf :: coordMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->coordMess ( (t_float)    arg0);
}
void GEMglTexGenf :: pnameMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->pnameMess ( (t_float)    arg0);
}
void GEMglTexGenf :: paramMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->paramMess ( (t_float)    arg0);
}
