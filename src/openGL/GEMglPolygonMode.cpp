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

#include "GEMglPolygonMode.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglPolygonMode , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglPolygonMode :: GEMglPolygonMode	(t_floatarg arg0=0, t_floatarg arg1=0) :
		face((GLenum)arg0), 
		mode((GLenum)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("face"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mode"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglPolygonMode :: ~GEMglPolygonMode () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglPolygonMode :: render(GemState *state) {
	glPolygonMode (face, mode);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglPolygonMode :: faceMess (t_float arg1) {	// FUN
	face = (GLenum)arg1;
	setModified();
}

void GEMglPolygonMode :: modeMess (t_float arg1) {	// FUN
	mode = (GLenum)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglPolygonMode :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglPolygonMode::faceMessCallback,  	gensym("face"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglPolygonMode::modeMessCallback,  	gensym("mode"), A_DEFFLOAT, A_NULL);
};

void GEMglPolygonMode :: faceMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->faceMess ( (t_float)    arg0);
}
void GEMglPolygonMode :: modeMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->modeMess ( (t_float)    arg0);
}
