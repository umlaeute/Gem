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

#include "GEMglTexCoord4d.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglTexCoord4d , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTexCoord4d :: GEMglTexCoord4d	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
		s((GLdouble)arg0), 
		t((GLdouble)arg1), 
		r((GLdouble)arg2), 
		q((GLdouble)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("s"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("t"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("r"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("q"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglTexCoord4d :: ~GEMglTexCoord4d () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglTexCoord4d :: render(GemState *state) {
	glTexCoord4d (s, t, r, q);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglTexCoord4d :: sMess (t_float arg1) {	// FUN
	s = (GLdouble)arg1;
	setModified();
}

void GEMglTexCoord4d :: tMess (t_float arg1) {	// FUN
	t = (GLdouble)arg1;
	setModified();
}

void GEMglTexCoord4d :: rMess (t_float arg1) {	// FUN
	r = (GLdouble)arg1;
	setModified();
}

void GEMglTexCoord4d :: qMess (t_float arg1) {	// FUN
	q = (GLdouble)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTexCoord4d :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglTexCoord4d::sMessCallback,  	gensym("s"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexCoord4d::tMessCallback,  	gensym("t"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexCoord4d::rMessCallback,  	gensym("r"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexCoord4d::qMessCallback,  	gensym("q"), A_DEFFLOAT, A_NULL);
};

void GEMglTexCoord4d :: sMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->sMess ( (t_float)    arg0);
}
void GEMglTexCoord4d :: tMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->tMess ( (t_float)    arg0);
}
void GEMglTexCoord4d :: rMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->rMess ( (t_float)    arg0);
}
void GEMglTexCoord4d :: qMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->qMess ( (t_float)    arg0);
}
