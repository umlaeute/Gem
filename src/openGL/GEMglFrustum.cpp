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

#include "GEMglFrustum.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglFrustum )

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglFrustum :: GEMglFrustum	(int argc, t_atom* argv)
{
  left=right=bottom=top=zNear=zFar=0.0;
  if (argc>0)left  =atom_getfloat(argv);
  if (argc>1)right =atom_getfloat(argv+1);
  if (argc>2)bottom=atom_getfloat(argv+2);
  if (argc>3)top   =atom_getfloat(argv+3);
  if (argc>4)zNear =atom_getfloat(argv+4);
  if (argc>5)zFar  =atom_getfloat(argv+5);

	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("left"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("right"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("bottom"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("top"));
	m_inlet[4] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("zNear"));
	m_inlet[5] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("zFar"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglFrustum :: ~GEMglFrustum () {
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
void GEMglFrustum :: render(GemState *state) {
	glFrustum (left, right, bottom, top, zNear, zFar);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglFrustum :: leftMess (t_float arg1) {	// FUN
	left = (GLdouble)arg1;
	setModified();
}

void GEMglFrustum :: rightMess (t_float arg1) {	// FUN
	right = (GLdouble)arg1;
	setModified();
}

void GEMglFrustum :: bottomMess (t_float arg1) {	// FUN
	bottom = (GLdouble)arg1;
	setModified();
}

void GEMglFrustum :: topMess (t_float arg1) {	// FUN
	top = (GLdouble)arg1;
	setModified();
}

void GEMglFrustum :: zNearMess (t_float arg1) {	// FUN
	zNear = (GLdouble)arg1;
	setModified();
}

void GEMglFrustum :: zFarMess (t_float arg1) {	// FUN
	zFar = (GLdouble)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglFrustum :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglFrustum::leftMessCallback,  	gensym("left"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglFrustum::rightMessCallback,  	gensym("right"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglFrustum::bottomMessCallback,  	gensym("bottom"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglFrustum::topMessCallback,  	gensym("top"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglFrustum::zNearMessCallback,  	gensym("zNear"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglFrustum::zFarMessCallback,  	gensym("zFar"), A_DEFFLOAT, A_NULL);
};

void GEMglFrustum :: leftMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->leftMess ( (t_float)    arg0);
}
void GEMglFrustum :: rightMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->rightMess ( (t_float)    arg0);
}
void GEMglFrustum :: bottomMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->bottomMess ( (t_float)    arg0);
}
void GEMglFrustum :: topMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->topMess ( (t_float)    arg0);
}
void GEMglFrustum :: zNearMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->zNearMess ( (t_float)    arg0);
}
void GEMglFrustum :: zFarMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->zFarMess ( (t_float)    arg0);
}
