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

#include "GEMglFeedbackBuffer.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglFeedbackBuffer , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglFeedbackBuffer :: GEMglFeedbackBuffer	(t_floatarg arg0=128, t_floatarg arg1=0) :
		size((GLsizei)arg0), type((GLenum)arg1)
{
#ifndef GL_VERSION_1_1
        error("GEMglFeedbackBuffer: GEM was compiled without GL_VERSION_1_1");
        error("GEMglFeedbackBuffer: therefore this object will do nothing");
#endif
	len=(size>0)?size:128;
	buffer = new float[len];

	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("size"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("type"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglFeedbackBuffer :: ~GEMglFeedbackBuffer () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglFeedbackBuffer :: render(GemState *state) {
#ifdef GL_VERSION_1_1
  glFeedbackBuffer (size, type, buffer);
  post("glFeedbackBuffer: i got data @ %X, but i don't know what to do with it!", buffer);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglFeedbackBuffer :: sizeMess (t_float arg1) {	// FUN
	size = (GLsizei)arg1;
	if (size>len){
	  len=size;
	  delete[]buffer;
	  buffer = new float[len];
	}	  
	setModified();
}

void GEMglFeedbackBuffer :: typeMess (t_float arg1) {	// FUN
	type = (GLenum)arg1;
	setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglFeedbackBuffer :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglFeedbackBuffer::sizeMessCallback,  	gensym("size"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglFeedbackBuffer::typeMessCallback,  	gensym("type"), A_DEFFLOAT, A_NULL);
}

void GEMglFeedbackBuffer :: sizeMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->sizeMess ( (t_float)    arg0);
}
void GEMglFeedbackBuffer :: typeMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->typeMess ( (t_float)    arg0);
}
