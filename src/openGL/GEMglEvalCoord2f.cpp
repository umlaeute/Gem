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

#include "GEMglEvalCoord2f.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglEvalCoord2f , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglEvalCoord2f :: GEMglEvalCoord2f	(t_floatarg arg0=0, t_floatarg arg1=0) :
		u((GLfloat)arg0), 
		v((GLfloat)arg1)
{
#ifndef GL_VERSION_1_1
        error("GEMglEvalCoord2f: GEM was compiled without GL_VERSION_1_1");
        error("GEMglEvalCoord2f: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("u"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglEvalCoord2f :: ~GEMglEvalCoord2f () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglEvalCoord2f :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glEvalCoord2f (u, v);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglEvalCoord2f :: uMess (t_float arg1) {	// FUN
	u = (GLfloat)arg1;
	setModified();
}

void GEMglEvalCoord2f :: vMess (t_float arg1) {	// FUN
	v = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglEvalCoord2f :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglEvalCoord2f::uMessCallback,  	gensym("u"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglEvalCoord2f::vMessCallback,  	gensym("v"), A_DEFFLOAT, A_NULL);
}

void GEMglEvalCoord2f :: uMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->uMess ( (t_float)    arg0);
}
void GEMglEvalCoord2f :: vMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->vMess ( (t_float)    arg0);
}
