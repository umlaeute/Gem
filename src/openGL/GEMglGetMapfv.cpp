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

#include "GEMglGetMapfv.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglGetMapfv, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglGetMapfv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglGetMapfv :: GEMglGetMapfv	(t_floatarg arg0=0, t_floatarg arg1=0) {
#ifndef GL_VERSION_1_1
        error("GEMglGetMapfv: GEM was compiled without GL_VERSION_1_1");
        error("GEMglGetMapfv: therefore this object will do nothing");
#endif
	targetMess(arg0);
	queryMess(arg1);
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("target"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("query"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglGetMapfv :: ~GEMglGetMapfv () {
	inlet_free(m_inlet[0]);
	inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglGetMapfv :: render(GemState *state) {
#ifdef GL_VERSION_1_1
  glGetMapfv (target, query, v);
  post("glGetMapfv: not really implemented:: got data @ %X, what should i do with it", v);
#endif // GL_VERSION_1_1
}


/////////////////////////////////////////////////////////
// variable
//
void GEMglGetMapfv :: targetMess (t_float arg0) {	// FUN
  target=(GLenum)arg0;
  setModified();
}
void GEMglGetMapfv :: queryMess (t_float arg0) {	// FUN
  query=(GLenum)arg0;
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglGetMapfv :: obj_setupCallback(t_class *classPtr) {
  class_addmethod(classPtr, (t_method)&GEMglGetMapfv::targetMessCallback, gensym("target"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&GEMglGetMapfv::queryMessCallback, gensym("query"), A_DEFFLOAT, A_NULL);
}

void GEMglGetMapfv :: targetMessCallback (void* data, t_floatarg arg0) {
	GetMyClass(data)->targetMess (arg0 );
}
void GEMglGetMapfv :: queryMessCallback (void* data, t_floatarg arg0) {
	GetMyClass(data)->queryMess (arg0 );
}
