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

#include "GEMglDepthFunc.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglDepthFunc , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglDepthFunc :: GEMglDepthFunc	(t_floatarg arg0=0) :
		func((GLenum)arg0)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("func"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglDepthFunc :: ~GEMglDepthFunc () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglDepthFunc :: render(GemState *state) {
	glDepthFunc (func);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglDepthFunc :: funcMess (t_float arg1) {	// FUN
	func = (GLenum)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglDepthFunc :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglDepthFunc::funcMessCallback,  	gensym("func"), A_DEFFLOAT, A_NULL);
};

void GEMglDepthFunc :: funcMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->funcMess ( (t_float)    arg0);
}
