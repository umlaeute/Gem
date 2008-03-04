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

#include "GEMglPassThrough.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglPassThrough , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglPassThrough :: GEMglPassThrough	(t_floatarg arg0=0) :
		token((GLfloat)arg0)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("token"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglPassThrough :: ~GEMglPassThrough () {
inlet_free(m_inlet[0]);
}

//////////////////
// extension check
bool GEMglPassThrough :: isRunnable(void) {
  if(GLEW_VERSION_1_1)return true;
  error("your system does not support OpenGL-1.1");
  return false;
}


/////////////////////////////////////////////////////////
// Render
//
void GEMglPassThrough :: render(GemState *state) {
	glPassThrough (token);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglPassThrough :: tokenMess (t_float arg1) {	// FUN
	token = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglPassThrough :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglPassThrough::tokenMessCallback,  	gensym("token"), A_DEFFLOAT, A_NULL);
}

void GEMglPassThrough :: tokenMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->tokenMess ( (t_float)    arg0);
}
