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

#include "GEMglClearDepth.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglClearDepth , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglClearDepth :: GEMglClearDepth	(t_floatarg arg0=0) :
		depth((GLclampd)arg0)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("depth"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglClearDepth :: ~GEMglClearDepth () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglClearDepth :: render(GemState *state) {
	glClearDepth (depth);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglClearDepth :: depthMess (GLclampd arg1) {	// FUN
	depth = (GLclampd)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglClearDepth :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglClearDepth::depthMessCallback,  	gensym("depth"), A_DEFFLOAT, A_NULL);
};

void GEMglClearDepth :: depthMessCallback (void* data, GLclampd arg0){
	GetMyClass(data)->depthMess ( (GLclampd)    arg0);
}
