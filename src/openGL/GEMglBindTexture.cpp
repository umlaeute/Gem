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

#include "GEMglBindTexture.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglBindTexture , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglBindTexture :: GEMglBindTexture	(t_floatarg arg0=0, t_floatarg arg1=0) :
		target((GLenum)arg0), 
		texture((GLuint)arg1)
{
#ifndef GL_VERSION_1_1
        error("GEMglBindTexture: GEM was compiled without GL_VERSION_1_1");
        error("GEMglBindTexture: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("target"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("texture"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglBindTexture :: ~GEMglBindTexture () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglBindTexture :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glBindTexture (target, texture);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglBindTexture :: targetMess (t_float arg1) {	// FUN
	target = (GLenum)arg1;
	setModified();
}

void GEMglBindTexture :: textureMess (t_float arg1) {	// FUN
	texture = (GLuint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglBindTexture :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglBindTexture::targetMessCallback,  	gensym("target"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglBindTexture::textureMessCallback,  	gensym("texture"), A_DEFFLOAT, A_NULL);
}

void GEMglBindTexture :: targetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->targetMess ( (t_float)    arg0);
}
void GEMglBindTexture :: textureMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->textureMess ( (t_float)    arg0);
}
