////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2004-2005 tigital@mac.com
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
////////////////////////////////////////////////////////

#include "GEMglMultiTexCoord2fARB.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglMultiTexCoord2fARB , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglMultiTexCoord2fARB
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglMultiTexCoord2fARB :: GEMglMultiTexCoord2fARB	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		texUnit((GLenum)arg0), 
		s((GLfloat)arg1), 
		t((GLfloat)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("texUnit"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("s"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("t"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglMultiTexCoord2fARB :: ~GEMglMultiTexCoord2fARB () {
	inlet_free(m_inlet[0]);
	inlet_free(m_inlet[1]);
	inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglMultiTexCoord2fARB :: render(GemState *state) {
#ifdef GL_ARB_multitexture
	glMultiTexCoord2fARB (texUnit, s, t);
#endif
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglMultiTexCoord2fARB :: texUnitMess (t_float arg1) {	// FUN
	texUnit = (GLenum)arg1;
	setModified();
}

void GEMglMultiTexCoord2fARB :: sMess (t_float arg1) {	// FUN
	s = (GLfloat)arg1;
	setModified();
}

void GEMglMultiTexCoord2fARB :: tMess (t_float arg1) {	// FUN
	t = (GLfloat)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglMultiTexCoord2fARB :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglMultiTexCoord2fARB::texUnitMessCallback,  	
					gensym("texUnit"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMultiTexCoord2fARB::sMessCallback,  	
					gensym("s"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglMultiTexCoord2fARB::tMessCallback,  	
					gensym("t"), A_DEFFLOAT, A_NULL);
};

void GEMglMultiTexCoord2fARB :: texUnitMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->texUnitMess ( (t_float)    arg0);
}
void GEMglMultiTexCoord2fARB :: sMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->sMess ( (t_float)    arg0);
}
void GEMglMultiTexCoord2fARB :: tMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->tMess ( (t_float)    arg0);
}
