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

#include "GEMglGetPointerv.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglGetPointerv , t_floatarg, A_DEFFLOAT )

/////////////////////////////////////////////////////////
//
// GEMglGetPointerv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglGetPointerv :: GEMglGetPointerv	(t_floatarg arg0=0) {
#ifndef GL_VERSION_1_1
        error("GEMglGetPointerv: GEM was compiled without GL_VERSION_1_1");
        error("GEMglGetPointerv: therefore this object will do nothing");
#endif
	pnameMess(arg0);
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglGetPointerv :: ~GEMglGetPointerv () {
	inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglGetPointerv :: render(GemState *state) {
#ifdef GL_VERSION_1_1
  glGetPointerv (pname,params);
  post("glGetPointerv: not really implemented:: got data @ %X, what should i do with it?",
       params);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglGetPointerv :: pnameMess (t_float arg0) {	// FUN
  pname=(GLenum)arg0;
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglGetPointerv :: obj_setupCallback(t_class *classPtr) {
  class_addmethod(classPtr, (t_method)&GEMglGetPointerv::pnameMessCallback,  	gensym("pname"), A_DEFFLOAT, A_NULL);
}

void GEMglGetPointerv :: pnameMessCallback (void* data, t_floatarg arg0) {
	GetMyClass(data)->pnameMess ( arg0 );
}
