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

#include "GEMglSelectBuffer.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglSelectBuffer , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglSelectBuffer :: GEMglSelectBuffer	(t_floatarg arg0=16){
#ifndef GL_VERSION_1_1
        error("GEMglSelectBuffer: GEM was compiled without GL_VERSION_1_1");
        error("GEMglSelectBuffer: therefore this object will do nothing");
#endif
	len=-1;
	buffer=0;
	sizeMess(arg0);
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("size"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglSelectBuffer :: ~GEMglSelectBuffer () {
inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglSelectBuffer :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glSelectBuffer (size, buffer);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglSelectBuffer :: sizeMess (t_float arg1) {	// FUN
  if (arg1<1)return;
  size = (GLsizei)arg1;
  if (len<size){
    len=size;
    delete[]buffer;
    buffer = new GLuint[len];

  }
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglSelectBuffer :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglSelectBuffer::sizeMessCallback,  	gensym("size"), A_DEFFLOAT, A_NULL);
}

void GEMglSelectBuffer :: sizeMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->sizeMess (arg0);
}
