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

#include "GEMglEvalCoord2d.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglEvalCoord2d , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglEvalCoord2d :: GEMglEvalCoord2d	(t_floatarg arg0=0, t_floatarg arg1=0) :
		u((GLdouble)arg0), 
		v((GLdouble)arg1)
{
#ifndef GL_VERSION_1_1
        error("GEMglEvalCoord2d: GEM was compiled without GL_VERSION_1_1");
        error("GEMglEvalCoord2d: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("u"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglEvalCoord2d :: ~GEMglEvalCoord2d () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglEvalCoord2d :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glEvalCoord2d (u, v);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglEvalCoord2d :: uMess (t_float arg1) {	// FUN
	u = (GLdouble)arg1;
	setModified();
}

void GEMglEvalCoord2d :: vMess (t_float arg1) {	// FUN
	v = (GLdouble)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglEvalCoord2d :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglEvalCoord2d::uMessCallback,  	gensym("u"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglEvalCoord2d::vMessCallback,  	gensym("v"), A_DEFFLOAT, A_NULL);
}

void GEMglEvalCoord2d :: uMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->uMess ( (t_float)    arg0);
}
void GEMglEvalCoord2d :: vMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->vMess ( (t_float)    arg0);
}
