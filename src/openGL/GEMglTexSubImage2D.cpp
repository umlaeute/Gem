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

#include "GEMglTexSubImage2D.h"
#include "Base/GemPixUtil.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglTexSubImage2D )

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTexSubImage2D :: GEMglTexSubImage2D	(int argc, t_atom*argv){
#ifndef GL_VERSION_1_1
        error("GEMglTexSubImage2D: GEM was compiled without GL_VERSION_1_1");
        error("GEMglTexSubImage2D: therefore this object will do nothing");
#endif
	if (argc>0)level  =atom_getint(argv+0);
	if (argc>1)xoffset=atom_getint(argv+1);
	if (argc>2)yoffset=atom_getint(argv+2);
	if (argc>3)width  =atom_getint(argv+3);
	if (argc>4)height =atom_getint(argv+4);
	
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("level"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("xoffset"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("yoffset"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("width"));
	m_inlet[4] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("height"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglTexSubImage2D :: ~GEMglTexSubImage2D () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
inlet_free(m_inlet[4]);
inlet_free(m_inlet[5]);
inlet_free(m_inlet[6]);
inlet_free(m_inlet[7]);
inlet_free(m_inlet[8]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglTexSubImage2D :: render(GemState *state) {
  if (!state||!state->image||!&state->image->image)return;
#ifdef GL_VERSION_1_1
  target=GL_TEXTURE_2D;
  glTexSubImage2D (target, level, xoffset, yoffset, width, height, 
		   state->image->image.format, 
		   state->image->image.type, 
		   state->image->image.data);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglTexSubImage2D :: targetMess (t_float arg1) {	// FUN
  post("GEMglTexSubImage2D: target has to be GL_TEXTURE_2D");
}

void GEMglTexSubImage2D :: levelMess (t_float arg1) {	// FUN
	level = (GLint)arg1;
	setModified();
}

void GEMglTexSubImage2D :: xoffsetMess (t_float arg1) {	// FUN
	xoffset = (GLint)arg1;
	setModified();
}

void GEMglTexSubImage2D :: yoffsetMess (t_float arg1) {	// FUN
	yoffset = (GLint)arg1;
	setModified();
}

void GEMglTexSubImage2D :: widthMess (t_float arg1) {	// FUN
	width = (GLsizei)arg1;
	setModified();
}

void GEMglTexSubImage2D :: heightMess (t_float arg1) {	// FUN
	height = (GLsizei)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTexSubImage2D :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglTexSubImage2D::targetMessCallback,  	gensym("target"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexSubImage2D::levelMessCallback,  	gensym("level"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexSubImage2D::xoffsetMessCallback,  	gensym("xoffset"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexSubImage2D::yoffsetMessCallback,  	gensym("yoffset"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexSubImage2D::widthMessCallback,  	gensym("width"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexSubImage2D::heightMessCallback,  	gensym("height"), A_DEFFLOAT, A_NULL);
}

void GEMglTexSubImage2D :: targetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->targetMess ( (t_float)    arg0);
}
void GEMglTexSubImage2D :: levelMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->levelMess ( (t_float)    arg0);
}
void GEMglTexSubImage2D :: xoffsetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->xoffsetMess ( (t_float)    arg0);
}
void GEMglTexSubImage2D :: yoffsetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->yoffsetMess ( (t_float)    arg0);
}
void GEMglTexSubImage2D :: widthMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->widthMess ( (t_float)    arg0);
}
void GEMglTexSubImage2D :: heightMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->heightMess ( (t_float)    arg0);
}
