////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2004 tigital@mac.com
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
////////////////////////////////////////////////////////

#include "GEMglBindProgramARB.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglBindProgramARB , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglBindProgramARB :: GEMglBindProgramARB	(t_floatarg arg0=0, t_floatarg arg1=0) :
		target((GLenum)arg0), 
		program((GLuint)arg1)
{
#ifndef GL_ARB_vertex_program
        error("GEMglBindProgramARB: GEM was compiled without GL_ARB_vertex_program");
        error("GEMglBindProgramARB: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("target"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("program"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglBindProgramARB :: ~GEMglBindProgramARB () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglBindProgramARB :: render(GemState *state) {
#ifdef GL_ARB_vertex_program
	glBindProgramARB (target, program);
#endif // GL_ARB_vertex_program
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglBindProgramARB :: targetMess (t_float arg1) {	// FUN
	target = (GLenum)arg1;
	setModified();
}

void GEMglBindProgramARB :: programMess (t_float arg1) {	// FUN
	program = (GLuint)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglBindProgramARB :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglBindProgramARB::targetMessCallback,  	gensym("target"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglBindProgramARB::programMessCallback,  	gensym("program"), A_DEFFLOAT, A_NULL);
}

void GEMglBindProgramARB :: targetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->targetMess ( (t_float)    arg0);
}
void GEMglBindProgramARB :: programMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->programMess ( (t_float)    arg0);
}
