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

#include "GLdefine.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GLdefine , t_symbol*, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GLdefine :: GLdefine	(t_symbol *s) {
  m_symbol = s;
  m_outlet = outlet_new(this->x_obj, &s_float);
}
/////////////////////////////////////////////////////////
// Destructor
//
GLdefine :: ~GLdefine () {
  outlet_free(m_outlet);
}

/////////////////////////////////////////////////////////
// Variables
//
void GLdefine :: symMess (t_symbol *s) {	// FUN
  m_symbol = s;
  bangMess();
}
void GLdefine :: bangMess () {
  t_float x = getGLdefine(m_symbol);
  outlet_float(m_outlet, x);
}


/////////////////////////////////////////////////////////
// static member functions
//

void GLdefine :: obj_setupCallback(t_class *classPtr) {
	 class_addsymbol(classPtr, GLdefine::symMessCallback);
	 class_addbang(classPtr, GLdefine::bangMessCallback);
	 class_addanything(classPtr, GLdefine::anyMessCallback);

};

void GLdefine :: symMessCallback (void* data, t_symbol *arg0){
	GetMyClass(data)->symMess (arg0);
}
void GLdefine :: anyMessCallback (void* data, t_symbol *arg0, int argc, t_atom*argv){
	GetMyClass(data)->symMess (arg0);
}
void GLdefine :: bangMessCallback (void* data){
	GetMyClass(data)->bangMess ();
}
