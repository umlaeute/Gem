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

#include "GEMglEvalCoord2fv.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglEvalCoord2fv , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglEvalCoord2fv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglEvalCoord2fv :: GEMglEvalCoord2fv	(t_floatarg arg0=0, t_floatarg arg1=0) {
#ifndef GL_VERSION_1_1
        error("GEMglEvalCoord2fv: GEM was compiled without GL_VERSION_1_1");
        error("GEMglEvalCoord2fv: therefore this object will do nothing");
#endif
vMess(arg0, arg1);
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglEvalCoord2fv :: ~GEMglEvalCoord2fv () {
	inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglEvalCoord2fv :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glEvalCoord2fv (v);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglEvalCoord2fv :: vMess (t_float arg0, t_float arg1) {	// FUN
	v[0]=(GLfloat )arg0;
	v[1]=(GLfloat )arg1;
	setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglEvalCoord2fv :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglEvalCoord2fv::vMessCallback,  	gensym("v"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void GEMglEvalCoord2fv :: vMessCallback (void* data, t_floatarg arg0, t_floatarg arg1) {
	GetMyClass(data)->vMess ( arg0, arg1);
}
