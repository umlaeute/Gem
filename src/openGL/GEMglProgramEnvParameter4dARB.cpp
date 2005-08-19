////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2004 tigital@mac.com
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
////////////////////////////////////////////////////////

#include "GEMglProgramEnvParameter4dARB.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglProgramEnvParameter4dARB )

/////////////////////////////////////////////////////////
//
// GEMglProgramEnvParameter4dARB
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglProgramEnvParameter4dARB :: GEMglProgramEnvParameter4dARB	(int argc, t_atom*argv ) :
		target(0), index(0), 
		m_x(0), m_y(0), m_z(0), m_w(0)
{
#ifndef GL_ARB_vertex_program
        error("GEMglProgramEnvParameter4dARB: GEM was compiled without GL_ARB_vertex_program");
        error("GEMglProgramEnvParameter4dARB: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("target"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("index"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
	m_inlet[4] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("z"));
	m_inlet[5] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("w"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglProgramEnvParameter4dARB :: ~GEMglProgramEnvParameter4dARB () {
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
void GEMglProgramEnvParameter4dARB :: render(GemState *state) {
#ifdef GL_ARB_vertex_program
	glProgramEnvParameter4dARB (target, index, m_x, m_y, m_z, m_w);
#endif
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglProgramEnvParameter4dARB :: targetMess (t_float arg1) {	// FUN
	target = (GLenum)arg1;
	post("target = %i",target);
	setModified();
}

void GEMglProgramEnvParameter4dARB :: indexMess (t_float arg2) {	// FUN
	index = (GLenum)arg2;
	post("index = %i",index);
	setModified();
}

void GEMglProgramEnvParameter4dARB :: xMess (t_float arg2) {	// FUN
	m_x = (GLdouble)arg2;
	post("m_x = %d",m_x);
	setModified();
}

void GEMglProgramEnvParameter4dARB :: yMess (t_float arg3) {	// FUN
	m_y = (GLdouble)arg3;
	post("m_y = %d",m_y);
	setModified();
}

void GEMglProgramEnvParameter4dARB :: zMess (t_float arg4) {	// FUN
	m_z = (GLdouble)arg4;
	post("m_z = %d",m_z);
	setModified();
}
void GEMglProgramEnvParameter4dARB :: wMess (t_float arg5) {	// FUN
	m_w = (GLdouble)arg5;
	post("m_w = %d",m_w);
	setModified();
}
/////////////////////////////////////////////////////////
// static member functions
//

void GEMglProgramEnvParameter4dARB :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglProgramEnvParameter4dARB::targetMessCallback,  	gensym("target"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglProgramEnvParameter4dARB::indexMessCallback,  	gensym("index"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglProgramEnvParameter4dARB::xMessCallback,  	gensym("x"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglProgramEnvParameter4dARB::yMessCallback,  	gensym("y"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglProgramEnvParameter4dARB::zMessCallback,  	gensym("z"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglProgramEnvParameter4dARB::wMessCallback,  	gensym("w"), A_DEFFLOAT, A_NULL);
};

void GEMglProgramEnvParameter4dARB :: targetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->targetMess ( (t_float)    arg0);
}
void GEMglProgramEnvParameter4dARB :: indexMessCallback (void* data, t_floatarg arg1){
	GetMyClass(data)->indexMess ( (t_float)    arg1);
}
void GEMglProgramEnvParameter4dARB :: xMessCallback (void* data, t_floatarg arg2){
	GetMyClass(data)->xMess ( (t_float)    arg2);
}
void GEMglProgramEnvParameter4dARB :: yMessCallback (void* data, t_floatarg arg3){
	GetMyClass(data)->yMess ( (t_float)    arg3);
}
void GEMglProgramEnvParameter4dARB :: zMessCallback (void* data, t_floatarg arg4){
	GetMyClass(data)->zMess ( (t_float)    arg4);
}
void GEMglProgramEnvParameter4dARB :: wMessCallback (void* data, t_floatarg arg5){
	GetMyClass(data)->wMess ( (t_float)    arg5);
}
