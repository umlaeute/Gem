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

#include "GEMglAlphaFunc.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglAlphaFunc , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglAlphaFunc :: GEMglAlphaFunc	(t_floatarg arg0=0, t_floatarg arg1=0) :
		func((GLenum)arg0), 
		ref((GLclampf)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("func"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ref"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglAlphaFunc :: ~GEMglAlphaFunc () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglAlphaFunc :: render(GemState *state) {
	glAlphaFunc (func, ref);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglAlphaFunc :: funcMess (t_float arg1) {	// FUN
	func = (GLenum)arg1;
	setModified();
}

void GEMglAlphaFunc :: refMess (t_float arg1) {	// FUN
	ref = (GLclampf)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglAlphaFunc :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglAlphaFunc::funcMessCallback,  	gensym("func"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglAlphaFunc::refMessCallback,  	gensym("ref"), A_DEFFLOAT, A_NULL);
};

void GEMglAlphaFunc :: funcMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->funcMess ( (t_float)    arg0);
}
void GEMglAlphaFunc :: refMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->refMess ( (t_float)    arg0);
}
