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
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("array"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglDisableClientState :: ~GEMglDisableClientState () {
inlet_free(m_inlet[0]);
}
//////////////////
// extension check
bool GEMglDisableClientState :: isRunnable(void) {
  if(GLEW_VERSION_1_1)return true;
  error("your system does not support OpenGL-1.1");
  return false;
}
/////////////////////////////////////////////////////////
// Render
//
void GEMglDisableClientState :: render(GemState *state) {
	glDisableClientState (array);
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
