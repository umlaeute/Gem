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

#include "GEMglDisableClientState.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglDisableClientState , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglDisableClientState :: GEMglDisableClientState	(t_floatarg arg0=0) :
		array((GLenum)arg0)
{
#ifndef GL_VERSION_1_1
        error("GEMglDisableClientState: GEM was compiled without GL_VERSION_1_1");
        error("GEMglDisableClientState: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("array"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglDisableClientState :: ~GEMglDisableClientState () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglDisableClientState :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glDisableClientState (array);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglDisableClientState :: arrayMess (t_float arg1) {	// FUN
	array = (GLenum)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglDisableClientState :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglDisableClientState::arrayMessCallback,  	gensym("array"), A_DEFFLOAT, A_NULL);
}

void GEMglDisableClientState :: arrayMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->arrayMess ( (t_float)    arg0);
}
