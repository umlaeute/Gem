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
////////////////////////////////////////////////////////

#include "GEMglUniform1fARB.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglUniform1fARB , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglUniform1fARB
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglUniform1fARB :: GEMglUniform1fARB	(t_floatarg arg0=0, t_floatarg arg1=0) :
		location((GLint)arg0), 
		val((GLfloat)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("location"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("val"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglUniform1fARB :: ~GEMglUniform1fARB () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglUniform1fARB :: render(GemState *state) {
#ifdef GL_ARB_shader_objects
	glUniform1fARB (location, val);
#endif
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglUniform1fARB :: locMess (t_float arg1) {	// FUN
	location = (GLint)arg1;
	setModified();
}

void GEMglUniform1fARB :: valMess (t_float arg1) {	// FUN
	val = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglUniform1fARB :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglUniform1fARB::locMessCallback,
		gensym("location"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglUniform1fARB::valMessCallback,
		gensym("val"), A_DEFFLOAT, A_NULL);
};

void GEMglUniform1fARB :: locMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->locMess ( (t_float)    arg0);
}
void GEMglUniform1fARB :: valMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->valMess ( (t_float)    arg0);
}
