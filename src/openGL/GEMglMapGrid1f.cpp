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

#include "GEMglMapGrid1f.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglMapGrid1f , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglMapGrid1f :: GEMglMapGrid1f	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		un((GLint)arg0), 
		u1((GLfloat)arg1), 
		u2((GLfloat)arg2)
{
#ifndef GL_VERSION_1_1
        error("GEMglMapGrid1f: GEM was compiled without GL_VERSION_1_1");
        error("GEMglMapGrid1f: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("un"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("u1"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("u2"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglMapGrid1f :: ~GEMglMapGrid1f () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglMapGrid1f :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glMapGrid1f (un, u1, u2);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglMapGrid1f :: unMess (t_float arg1) {	// FUN
	un = (GLint)arg1;
	setModified();
}

void GEMglMapGrid1f :: u1Mess (t_float arg1) {	// FUN
	u1 = (GLfloat)arg1;
	setModified();
}

void GEMglMapGrid1f :: u2Mess (t_float arg1) {	// FUN
	u2 = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglMapGrid1f :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid1f::unMessCallback,  	gensym("un"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid1f::u1MessCallback,  	gensym("u1"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid1f::u2MessCallback,  	gensym("u2"), A_DEFFLOAT, A_NULL);
}

void GEMglMapGrid1f :: unMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->unMess ( (t_float)    arg0);
}
void GEMglMapGrid1f :: u1MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->u1Mess ( (t_float)    arg0);
}
void GEMglMapGrid1f :: u2MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->u2Mess ( (t_float)    arg0);
}
