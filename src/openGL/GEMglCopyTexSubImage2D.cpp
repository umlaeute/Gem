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

#include "GEMglCopyTexSubImage2D.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglCopyTexSubImage2D )

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglCopyTexSubImage2D :: GEMglCopyTexSubImage2D(int argc, t_atom*argv){
#ifndef GL_VERSION_1_1
        error("GEMglCopyTexSubImage2D: GEM was compiled without GL_VERSION_1_1");
        error("GEMglCopyTexSubImage2D: therefore this object will do nothing");
#endif
	target=0;
	level=0;
	xoffset=yoffset=0;
	x=y=0;
	height=width=0;
	if (argc>0)target        =atom_getint(argv+0);
	if (argc>1)level         =atom_getint(argv+1);
	if (argc>2)xoffset       =atom_getint(argv+2);
	if (argc>3)yoffset       =atom_getint(argv+3);
	if (argc>4)x             =atom_getint(argv+4);
	if (argc>5)y             =atom_getint(argv+5);
	if (argc>6)width         =atom_getint(argv+6);
	if (argc>7)height        =atom_getint(argv+7);

	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("target"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("level"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("xoffset"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("yoffset"));
	m_inlet[4] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[5] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
	m_inlet[6] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("width"));
	m_inlet[7] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("height"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglCopyTexSubImage2D :: ~GEMglCopyTexSubImage2D () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
inlet_free(m_inlet[4]);
inlet_free(m_inlet[5]);
inlet_free(m_inlet[6]);
inlet_free(m_inlet[7]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglCopyTexSubImage2D :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glCopyTexSubImage2D (target, level, xoffset, yoffset, x, y, width, height);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglCopyTexSubImage2D :: targetMess (t_float arg1) {	// FUN
	target = (GLenum)arg1;
	setModified();
}

void GEMglCopyTexSubImage2D :: levelMess (t_float arg1) {	// FUN
	level = (GLint)arg1;
	setModified();
}

void GEMglCopyTexSubImage2D :: xoffsetMess (t_float arg1) {	// FUN
	xoffset = (GLint)arg1;
	setModified();
}

void GEMglCopyTexSubImage2D :: yoffsetMess (t_float arg1) {	// FUN
	yoffset = (GLint)arg1;
	setModified();
}

void GEMglCopyTexSubImage2D :: xMess (t_float arg1) {	// FUN
	x = (GLint)arg1;
	setModified();
}

void GEMglCopyTexSubImage2D :: yMess (t_float arg1) {	// FUN
	y = (GLint)arg1;
	setModified();
}

void GEMglCopyTexSubImage2D :: widthMess (t_float arg1) {	// FUN
	width = (GLsizei)arg1;
	setModified();
}

void GEMglCopyTexSubImage2D :: heightMess (t_float arg1) {	// FUN
	height = (GLsizei)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglCopyTexSubImage2D :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglCopyTexSubImage2D::targetMessCallback,  	gensym("target"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglCopyTexSubImage2D::levelMessCallback,  	gensym("level"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglCopyTexSubImage2D::xoffsetMessCallback,  	gensym("xoffset"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglCopyTexSubImage2D::yoffsetMessCallback,  	gensym("yoffset"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglCopyTexSubImage2D::xMessCallback,  	gensym("x"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglCopyTexSubImage2D::yMessCallback,  	gensym("y"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglCopyTexSubImage2D::widthMessCallback,  	gensym("width"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglCopyTexSubImage2D::heightMessCallback,  	gensym("height"), A_DEFFLOAT, A_NULL);
}

void GEMglCopyTexSubImage2D :: targetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->targetMess ( (t_float)    arg0);
}
void GEMglCopyTexSubImage2D :: levelMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->levelMess ( (t_float)    arg0);
}
void GEMglCopyTexSubImage2D :: xoffsetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->xoffsetMess ( (t_float)    arg0);
}
void GEMglCopyTexSubImage2D :: yoffsetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->yoffsetMess ( (t_float)    arg0);
}
void GEMglCopyTexSubImage2D :: xMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->xMess ( (t_float)    arg0);
}
void GEMglCopyTexSubImage2D :: yMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->yMess ( (t_float)    arg0);
}
void GEMglCopyTexSubImage2D :: widthMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->widthMess ( (t_float)    arg0);
}
void GEMglCopyTexSubImage2D :: heightMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->heightMess ( (t_float)    arg0);
}
