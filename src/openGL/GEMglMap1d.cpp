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

#include "GEMglMap1d.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglMap1d )

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglMap1d :: GEMglMap1d	(int argc, t_atom*argv){
#ifndef GL_VERSION_1_1
        error("GEMglMap1d: GEM was compiled without GL_VERSION_1_1");
        error("GEMglMap1d: therefore this object will do nothing");
#endif
	if (argc>0)target=(GLenum  )atom_getint(argv+0);
	if (argc>1)u1    =(GLdouble)atom_getint(argv+1);
	if (argc>2)u2    =(GLdouble)atom_getint(argv+2);
	if (argc>3)stride=(GLint   )atom_getint(argv+3);
	if (argc>4)order =(GLint   )atom_getint(argv+4);

	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("target"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("u1"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("u2"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("stride"));
	m_inlet[4] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("order"));
	m_inlet[5] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("points"));

	len=128;
	points = new GLdouble[len];
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglMap1d :: ~GEMglMap1d () {
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
void GEMglMap1d :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glMap1d (target, u1, u2, stride, order, points);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglMap1d :: targetMess (t_float arg1) {	// FUN
	target = (GLenum)arg1;
	setModified();
}

void GEMglMap1d :: u1Mess (t_float arg1) {	// FUN
	u1 = (GLdouble)arg1;
	setModified();
}

void GEMglMap1d :: u2Mess (t_float arg1) {	// FUN
	u2 = (GLdouble)arg1;
	setModified();
}

void GEMglMap1d :: strideMess (t_float arg1) {	// FUN
	stride = (GLint)arg1;
	setModified();
}

void GEMglMap1d :: orderMess (t_float arg1) {	// FUN
	order = (GLint)arg1;
	setModified();
}

void GEMglMap1d :: pointsMess (int argc, t_atom*argv) {	// FUN
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

void GEMglMap1d :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglMap1d::targetMessCallback,  	gensym("target"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMap1d::u1MessCallback,  	gensym("u1"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMap1d::u2MessCallback,  	gensym("u2"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMap1d::strideMessCallback,  	gensym("stride"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMap1d::orderMessCallback,  	gensym("order"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMap1d::pointsMessCallback,  	gensym("points"), A_GIMME, A_NULL);
}

void GEMglMap1d :: targetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->targetMess ( arg0);
}
void GEMglMap1d :: u1MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->u1Mess ( arg0);
}
void GEMglMap1d :: u2MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->u2Mess ( arg0);
}
void GEMglMap1d :: strideMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->strideMess ( arg0);
}
void GEMglMap1d :: orderMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->orderMess (arg0);
}
void GEMglMap1d :: pointsMessCallback (void* data, int argc, t_atom*argv){
	GetMyClass(data)->pointsMess (argc, argv);
}
