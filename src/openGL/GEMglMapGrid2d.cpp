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

#include "GEMglMapGrid2d.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglMapGrid2d )

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglMapGrid2d :: GEMglMapGrid2d	(int argc, t_atom*argv)  {
#ifndef GL_VERSION_1_1
        error("GEMglMapGrid2d: GEM was compiled without GL_VERSION_1_1");
        error("GEMglMapGrid2d: therefore this object will do nothing");
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
GEMglMapGrid2d :: ~GEMglMapGrid2d () {
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
void GEMglMapGrid2d :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glMapGrid2d (un, u1, u2, vn, v1, v2);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglMapGrid2d :: unMess (t_float arg1) {	// FUN
	un = (GLint)arg1;
	setModified();
}

void GEMglMapGrid2d :: u1Mess (t_float arg1) {	// FUN
	u1 = (GLdouble)arg1;
	setModified();
}

void GEMglMapGrid2d :: u2Mess (t_float arg1) {	// FUN
	u2 = (GLdouble)arg1;
	setModified();
}

void GEMglMapGrid2d :: vnMess (t_float arg1) {	// FUN
	vn = (GLint)arg1;
	setModified();
}

void GEMglMapGrid2d :: v1Mess (t_float arg1) {	// FUN
	v1 = (GLdouble)arg1;
	setModified();
}

void GEMglMapGrid2d :: v2Mess (t_float arg1) {	// FUN
	v2 = (GLdouble)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglMapGrid2d :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid2d::unMessCallback,  	gensym("un"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid2d::u1MessCallback,  	gensym("u1"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid2d::u2MessCallback,  	gensym("u2"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid2d::vnMessCallback,  	gensym("vn"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid2d::v1MessCallback,  	gensym("v1"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMapGrid2d::v2MessCallback,  	gensym("v2"), A_DEFFLOAT, A_NULL);
}

void GEMglMapGrid2d :: unMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->unMess ( (t_float)    arg0);
}
void GEMglMapGrid2d :: u1MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->u1Mess ( (t_float)    arg0);
}
void GEMglMapGrid2d :: u2MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->u2Mess ( (t_float)    arg0);
}
void GEMglMapGrid2d :: vnMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->vnMess ( (t_float)    arg0);
}
void GEMglMapGrid2d :: v1MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->v1Mess ( (t_float)    arg0);
}
void GEMglMapGrid2d :: v2MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->v2Mess ( (t_float)    arg0);
}
