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

#include "GEMglTexCoord2f.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglTexCoord2f , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTexCoord2f :: GEMglTexCoord2f	(t_floatarg arg0=0, t_floatarg arg1=0) :
		s((GLfloat)arg0), 
		t((GLfloat)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("s"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("t"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglTexCoord2f :: ~GEMglTexCoord2f () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglTexCoord2f :: render(GemState *state) {
	glTexCoord2f (s, t);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglTexCoord2f :: sMess (t_float arg1) {	// FUN
	s = (GLfloat)arg1;
	setModified();
}

void GEMglTexCoord2f :: tMess (t_float arg1) {	// FUN
	t = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTexCoord2f :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglTexCoord2f::sMessCallback),  	gensym("s"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, reinterpret_cast<t_method>(&GEMglTexCoord2f::tMessCallback),  	gensym("t"), A_DEFFLOAT, A_NULL);
};

void GEMglTexCoord2f :: sMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->sMess ( (t_float)    arg0);
}
void GEMglTexCoord2f :: tMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->tMess ( (t_float)    arg0);
}
