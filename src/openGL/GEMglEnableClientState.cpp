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

#include "GEMglEnableClientState.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglEnableClientState , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglEnableClientState :: GEMglEnableClientState	(t_floatarg arg0=0) :
		array((GLenum)arg0)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("array"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglEnableClientState :: ~GEMglEnableClientState () {
inlet_free(m_inlet[0]);
}
//////////////////
// extension check
bool GEMglEnableClientState :: isRunnable(void) {
  if(GLEW_VERSION_1_1)return true;
  error("your system does not support OpenGL-1.1");
  return false;
}
/////////////////////////////////////////////////////////
// Render
//
void GEMglEnableClientState :: render(GemState *state) {
	glEnableClientState (array);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglEnableClientState :: arrayMess (t_float arg1) {	// FUN
	array = (GLenum)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglEnableClientState :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglEnableClientState::arrayMessCallback,  	gensym("array"), A_DEFFLOAT, A_NULL);
}

void GEMglEnableClientState :: arrayMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->arrayMess ( (t_float)    arg0);
}
