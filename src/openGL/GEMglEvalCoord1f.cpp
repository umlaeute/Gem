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

#include "GEMglEvalCoord1f.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglEvalCoord1f , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglEvalCoord1f :: GEMglEvalCoord1f	(t_floatarg arg0=0) :
		u((GLfloat)arg0)
{
#ifndef GL_VERSION_1_1
        error("GEMglEvalCoord1f: GEM was compiled without GL_VERSION_1_1");
        error("GEMglEvalCoord1f: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("u"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglEvalCoord1f :: ~GEMglEvalCoord1f () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglEvalCoord1f :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glEvalCoord1f (u);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglEvalCoord1f :: uMess (t_float arg1) {	// FUN
	u = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglEvalCoord1f :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglEvalCoord1f::uMessCallback,  	gensym("u"), A_DEFFLOAT, A_NULL);
}

void GEMglEvalCoord1f :: uMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->uMess ( (t_float)    arg0);
}
