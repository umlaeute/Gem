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

#include "GEMglBlendFunc.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglBlendFunc , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglBlendFunc :: GEMglBlendFunc	(t_floatarg arg0=0, t_floatarg arg1=0) :
		sfactor((GLenum)arg0), 
		dfactor((GLenum)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("sfactor"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("dfactor"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglBlendFunc :: ~GEMglBlendFunc () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglBlendFunc :: render(GemState *state) {
	glBlendFunc (sfactor, dfactor);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglBlendFunc :: sfactorMess (t_float arg1) {	// FUN
	sfactor = (GLenum)arg1;
	setModified();
}

void GEMglBlendFunc :: dfactorMess (t_float arg1) {	// FUN
	dfactor = (GLenum)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglBlendFunc :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglBlendFunc::sfactorMessCallback,  	gensym("sfactor"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglBlendFunc::dfactorMessCallback,  	gensym("dfactor"), A_DEFFLOAT, A_NULL);
};

void GEMglBlendFunc :: sfactorMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->sfactorMess ( (t_float)    arg0);
}
void GEMglBlendFunc :: dfactorMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->dfactorMess ( (t_float)    arg0);
}
