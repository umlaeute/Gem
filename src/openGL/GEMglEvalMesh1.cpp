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

#include "GEMglEvalMesh1.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglEvalMesh1 , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglEvalMesh1 :: GEMglEvalMesh1	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		mode((GLenum)arg0), 
		i1((GLint)arg1), 
		i2((GLint)arg2)
{
#ifndef GL_VERSION_1_1
        error("GEMglEvalMesh1: GEM was compiled without GL_VERSION_1_1");
        error("GEMglEvalMesh1: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mode"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("i1"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("i2"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglEvalMesh1 :: ~GEMglEvalMesh1 () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglEvalMesh1 :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glEvalMesh1 (mode, i1, i2);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglEvalMesh1 :: modeMess (t_float arg1) {	// FUN
	mode = (GLenum)arg1;
	setModified();
}

void GEMglEvalMesh1 :: i1Mess (t_float arg1) {	// FUN
	i1 = (GLint)arg1;
	setModified();
}

void GEMglEvalMesh1 :: i2Mess (t_float arg1) {	// FUN
	i2 = (GLint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglEvalMesh1 :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglEvalMesh1::modeMessCallback,  	gensym("mode"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglEvalMesh1::i1MessCallback,  	gensym("i1"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglEvalMesh1::i2MessCallback,  	gensym("i2"), A_DEFFLOAT, A_NULL);
}

void GEMglEvalMesh1 :: modeMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->modeMess ( (t_float)    arg0);
}
void GEMglEvalMesh1 :: i1MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->i1Mess ( (t_float)    arg0);
}
void GEMglEvalMesh1 :: i2MessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->i2Mess ( (t_float)    arg0);
}
