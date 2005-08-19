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

#include "GEMglProgramEnvParameter4fvARB.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglProgramEnvParameter4fvARB , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglProgramEnvParameter4fvARB
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglProgramEnvParameter4fvARB :: GEMglProgramEnvParameter4fvARB	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		target((GLenum)arg0), 
		index((GLenum)arg1) 
		//params((GLfloat)arg2)
{
#ifndef GL_ARB_vertex_program
        error("GEMglProgramEnvParameter4fvARB: GEM was compiled without GL_ARB_vertex_program");
        error("GEMglProgramEnvParameter4fvARB: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("target"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("index"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("params"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglProgramEnvParameter4fvARB :: ~GEMglProgramEnvParameter4fvARB () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglProgramEnvParameter4fvARB :: render(GemState *state) {
#ifdef GL_ARB_vertex_program
	glProgramEnvParameter4fvARB (target, index, params);
#endif
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglProgramEnvParameter4fvARB :: targetMess (t_float arg1) {	// FUN
	target = (GLenum)arg1;
	setModified();
}

void GEMglProgramEnvParameter4fvARB :: indexMess (t_float arg1) {	// FUN
	index = (GLenum)arg1;
	setModified();
}

void GEMglProgramEnvParameter4fvARB :: paramsMess (int argc, t_atom*argv) {	// FUN
	if(argc!=4){
		post("GEMglProgramEnvParameter4vARB:  needs 4 elements");
		return;
	}
	int i;
	for (i=0;i<4;i++){
		params[i] = (GLfloat)atom_getfloat(argv+i);
		//post("params[%i] = %f\n",i,params[i]);
	}
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglProgramEnvParameter4fvARB :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglProgramEnvParameter4fvARB::targetMessCallback,  	gensym("target"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglProgramEnvParameter4fvARB::indexMessCallback,  	gensym("index"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglProgramEnvParameter4fvARB::paramsMessCallback,  	gensym("params"), A_GIMME, A_NULL);
};

void GEMglProgramEnvParameter4fvARB :: targetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->targetMess ( (t_float)    arg0);
}
void GEMglProgramEnvParameter4fvARB :: indexMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->indexMess ( (t_float)    arg0);
}
void GEMglProgramEnvParameter4fvARB :: paramsMessCallback (void* data, t_symbol*,int argc, t_atom*argv){
	GetMyClass(data)->paramsMess ( argc, argv );
}
