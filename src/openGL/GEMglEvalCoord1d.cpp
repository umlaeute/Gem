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

#include "GEMglEvalCoord1d.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglEvalCoord1d , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglEvalCoord1d :: GEMglEvalCoord1d	(t_floatarg arg0=0) :
		u((GLdouble)arg0)
{
#ifndef GL_VERSION_1_1
        error("GEMglEvalCoord1d: GEM was compiled without GL_VERSION_1_1");
        error("GEMglEvalCoord1d: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("u"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglEvalCoord1d :: ~GEMglEvalCoord1d () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglEvalCoord1d :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glEvalCoord1d (u);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglEvalCoord1d :: uMess (t_float arg1) {	// FUN
	u = (GLdouble)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglEvalCoord1d :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglEvalCoord1d::uMessCallback,  	gensym("u"), A_DEFFLOAT, A_NULL);
}

void GEMglEvalCoord1d :: uMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->uMess ( (t_float)    arg0);
}
