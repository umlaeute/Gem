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

#include "GEMglMaterialf.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglMaterialf , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglMaterialf :: GEMglMaterialf	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		face((GLenum)arg0), 
		pname((GLenum)arg1), 
		param((GLfloat)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("face"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("param"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglMaterialf :: ~GEMglMaterialf () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglMaterialf :: render(GemState *state) {
	glMaterialf (face, pname, param);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglMaterialf :: faceMess (t_float arg1) {	// FUN
	face = (GLenum)arg1;
	setModified();
}

void GEMglMaterialf :: pnameMess (t_float arg1) {	// FUN
	pname = (GLenum)arg1;
	setModified();
}

void GEMglMaterialf :: paramMess (t_float arg1) {	// FUN
	param = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglMaterialf :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglMaterialf::faceMessCallback,  	gensym("face"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMaterialf::pnameMessCallback,  	gensym("pname"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMaterialf::paramMessCallback,  	gensym("param"), A_DEFFLOAT, A_NULL);
};

void GEMglMaterialf :: faceMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->faceMess ( (t_float)    arg0);
}
void GEMglMaterialf :: pnameMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->pnameMess ( (t_float)    arg0);
}
void GEMglMaterialf :: paramMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->paramMess ( (t_float)    arg0);
}
