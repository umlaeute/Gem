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

#include "GEMglMapGrid1d.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglMapGrid1d , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglMapGrid1d :: GEMglMapGrid1d	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		un((GLint)arg0), 
		u1((GLdouble)arg1), 
		u2((GLdouble)arg2)
{
#ifndef GL_VERSION_1_1
        error("GEMglMapGrid1d: GEM was compiled without GL_VERSION_1_1");
        error("GEMglMapGrid1d: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("un"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("u1"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("u2"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglMapGrid1d :: ~GEMglMapGrid1d () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglMapGrid1d :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glMapGrid1d (un, u1, u2);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglMapGrid1d :: unMess (t_float arg1) {	// FUN
	un = (GLint)arg1;
	setModified();
}

void GEMglMapGrid1d :: u1Mess (t_float arg1) {	// FUN
	u1 = (GLdouble)arg1;
	setModified();
}

void GEMglMapGrid1d :: u2Mess (t_float arg1) {	// FUN
	u2 = (GLdouble)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglMapGrid1d :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid1d::unMessCallback,  	gensym("un"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid1d::u1MessCallback,  	gensym("u1"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid1d::u2MessCallback,  	gensym("u2"), A_DEFFLOAT, A_NULL);
}

void GEMglMapGrid1d :: unMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->unMess ( (t_float)    arg0);
}
void GEMglMapGrid1d :: u1MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->u1Mess ( (t_float)    arg0);
}
void GEMglMapGrid1d :: u2MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->u2Mess ( (t_float)    arg0);
}
