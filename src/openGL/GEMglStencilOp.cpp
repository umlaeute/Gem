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

#include "GEMglStencilOp.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglStencilOp , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglStencilOp :: GEMglStencilOp	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		fail((GLenum)arg0), 
		zfail((GLenum)arg1), 
		zpass((GLenum)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("fail"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("zfail"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("zpass"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglStencilOp :: ~GEMglStencilOp () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglStencilOp :: render(GemState *state) {
	glStencilOp (fail, zfail, zpass);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglStencilOp :: failMess (t_float arg1) {	// FUN
	fail = (GLenum)arg1;
	setModified();
}

void GEMglStencilOp :: zfailMess (t_float arg1) {	// FUN
	zfail = (GLenum)arg1;
	setModified();
}

void GEMglStencilOp :: zpassMess (t_float arg1) {	// FUN
	zpass = (GLenum)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglStencilOp :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglStencilOp::failMessCallback,  	gensym("fail"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglStencilOp::zfailMessCallback,  	gensym("zfail"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglStencilOp::zpassMessCallback,  	gensym("zpass"), A_DEFFLOAT, A_NULL);
};

void GEMglStencilOp :: failMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->failMess ( (t_float)    arg0);
}
void GEMglStencilOp :: zfailMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->zfailMess ( (t_float)    arg0);
}
void GEMglStencilOp :: zpassMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->zpassMess ( (t_float)    arg0);
}
