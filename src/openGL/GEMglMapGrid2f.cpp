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

#include "GEMglMapGrid2f.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglMapGrid2f )

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglMapGrid2f :: GEMglMapGrid2f	(int argc, t_atom *argv){
#ifndef GL_VERSION_1_1
        error("GEMglMapGrid2f: GEM was compiled without GL_VERSION_1_1");
        error("GEMglMapGrid2f: therefore this object will do nothing");
#endif
	if (argc>0)un=atom_getint (argv+0);
	if (argc>1)u1=atom_getfloat(argv+1);
	if (argc>2)u2=atom_getfloat(argv+2);
	if (argc>3)vn=atom_getint (argv+3);
	if (argc>4)v1=atom_getfloat(argv+4);
	if (argc>5)v2=atom_getfloat(argv+5);


	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("un"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("u1"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("u2"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("vn"));
	m_inlet[4] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v1"));
	m_inlet[5] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v2"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglMapGrid2f :: ~GEMglMapGrid2f () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
inlet_free(m_inlet[4]);
inlet_free(m_inlet[5]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglMapGrid2f :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glMapGrid2f (un, u1, u2, vn, v1, v2);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglMapGrid2f :: unMess (t_float arg1) {	// FUN
	un = (GLint)arg1;
	setModified();
}

void GEMglMapGrid2f :: u1Mess (t_float arg1) {	// FUN
	u1 = (GLfloat)arg1;
	setModified();
}

void GEMglMapGrid2f :: u2Mess (t_float arg1) {	// FUN
	u2 = (GLfloat)arg1;
	setModified();
}

void GEMglMapGrid2f :: vnMess (t_float arg1) {	// FUN
	vn = (GLint)arg1;
	setModified();
}

void GEMglMapGrid2f :: v1Mess (t_float arg1) {	// FUN
	v1 = (GLfloat)arg1;
	setModified();
}

void GEMglMapGrid2f :: v2Mess (t_float arg1) {	// FUN
	v2 = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglMapGrid2f :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid2f::unMessCallback,  	gensym("un"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid2f::u1MessCallback,  	gensym("u1"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid2f::u2MessCallback,  	gensym("u2"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid2f::vnMessCallback,  	gensym("vn"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid2f::v1MessCallback,  	gensym("v1"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid2f::v2MessCallback,  	gensym("v2"), A_DEFFLOAT, A_NULL);
}

void GEMglMapGrid2f :: unMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->unMess ( (t_float)    arg0);
}
void GEMglMapGrid2f :: u1MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->u1Mess ( (t_float)    arg0);
}
void GEMglMapGrid2f :: u2MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->u2Mess ( (t_float)    arg0);
}
void GEMglMapGrid2f :: vnMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->vnMess ( (t_float)    arg0);
}
void GEMglMapGrid2f :: v1MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->v1Mess ( (t_float)    arg0);
}
void GEMglMapGrid2f :: v2MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->v2Mess ( (t_float)    arg0);
}
