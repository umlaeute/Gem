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

#include "GEMglMap2d.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglMap2d )

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglMap2d :: GEMglMap2d	(int argc, t_atom *argv){
#ifndef GL_VERSION_1_1
        error("GEMglMap2d: GEM was compiled without GL_VERSION_1_1");
        error("GEMglMap2d: therefore this object will do nothing");
#endif
	if (argc>0)target =(GLenum  )atom_getint(argv+0);
	if (argc>1)u1     =(GLdouble)atom_getint(argv+1);
	if (argc>2)u2     =(GLdouble)atom_getint(argv+2);
	if (argc>3)ustride=(GLint   )atom_getint(argv+3);
	if (argc>4)uorder =(GLint   )atom_getint(argv+4);
	if (argc>5)v1     =(GLdouble)atom_getint(argv+5);
	if (argc>6)v2     =(GLdouble)atom_getint(argv+6);
	if (argc>7)vstride=(GLint   )atom_getint(argv+7);
	if (argc>8)vorder =(GLint   )atom_getint(argv+8);

	len=128;
	points = new GLdouble[len];

	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("target"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("u1"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("u2"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ustride"));
	m_inlet[4] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("uorder"));
	m_inlet[5] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v1"));
	m_inlet[6] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v2"));
	m_inlet[7] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("vstride"));
	m_inlet[8] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("vorder"));
	m_inlet[9] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("points"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglMap2d :: ~GEMglMap2d () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
inlet_free(m_inlet[4]);
inlet_free(m_inlet[5]);
inlet_free(m_inlet[6]);
inlet_free(m_inlet[7]);
inlet_free(m_inlet[8]);
inlet_free(m_inlet[9]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglMap2d :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glMap2d (target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglMap2d :: targetMess (t_float arg1) {	// FUN
	target = (GLenum)arg1;
	setModified();
}

void GEMglMap2d :: u1Mess (t_float arg1) {	// FUN
	u1 = (GLdouble)arg1;
	setModified();
}

void GEMglMap2d :: u2Mess (t_float arg1) {	// FUN
	u2 = (GLdouble)arg1;
	setModified();
}

void GEMglMap2d :: ustrideMess (t_float arg1) {	// FUN
	ustride = (GLint)arg1;
	setModified();
}

void GEMglMap2d :: uorderMess (t_float arg1) {	// FUN
	uorder = (GLint)arg1;
	setModified();
}

void GEMglMap2d :: v1Mess (t_float arg1) {	// FUN
	v1 = (GLdouble)arg1;
	setModified();
}

void GEMglMap2d :: v2Mess (t_float arg1) {	// FUN
	v2 = (GLdouble)arg1;
	setModified();
}

void GEMglMap2d :: vstrideMess (t_float arg1) {	// FUN
	vstride = (GLint)arg1;
	setModified();
}

void GEMglMap2d :: vorderMess (t_float arg1) {	// FUN
	vorder = (GLint)arg1;
	setModified();
}

void GEMglMap2d :: pointsMess (int argc, t_atom*argv) {	// FUN
  if (argc>len){
    len=argc;
    delete [] points;
    points = new GLdouble[len];
  }
  while(argc--)points[argc]=atom_getfloat(argv+argc);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglMap2d :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglMap2d::targetMessCallback,  	gensym("target"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMap2d::u1MessCallback,  	gensym("u1"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMap2d::u2MessCallback,  	gensym("u2"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMap2d::ustrideMessCallback,  	gensym("ustride"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMap2d::uorderMessCallback,  	gensym("uorder"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMap2d::v1MessCallback,  	gensym("v1"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMap2d::v2MessCallback,  	gensym("v2"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMap2d::vstrideMessCallback,  	gensym("vstride"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMap2d::vorderMessCallback,  	gensym("vorder"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMap2d::pointsMessCallback,  	gensym("points"), A_GIMME, A_NULL);
}

void GEMglMap2d :: targetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->targetMess ( (t_float)    arg0);
}
void GEMglMap2d :: u1MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->u1Mess ( (t_float)    arg0);
}
void GEMglMap2d :: u2MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->u2Mess ( (t_float)    arg0);
}
void GEMglMap2d :: ustrideMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->ustrideMess ( (t_float)    arg0);
}
void GEMglMap2d :: uorderMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->uorderMess ( (t_float)    arg0);
}
void GEMglMap2d :: v1MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->v1Mess ( (t_float)    arg0);
}
void GEMglMap2d :: v2MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->v2Mess ( (t_float)    arg0);
}
void GEMglMap2d :: vstrideMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->vstrideMess ( (t_float)    arg0);
}
void GEMglMap2d :: vorderMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->vorderMess ( (t_float)    arg0);
}
void GEMglMap2d :: pointsMessCallback (void* data, int argc, t_atom*argv){
	GetMyClass(data)->pointsMess (argc, argv);
}
