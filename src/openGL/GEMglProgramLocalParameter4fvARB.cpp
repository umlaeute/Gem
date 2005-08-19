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

#include "GEMglProgramLocalParameter4fvARB.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglProgramLocalParameter4fvARB , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglProgramLocalParameter4fvARB
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglProgramLocalParameter4fvARB :: GEMglProgramLocalParameter4fvARB	(t_floatarg arg0=0, t_floatarg arg1=0, t_floatarg arg2=0) :
		target((GLenum)arg0), 
		index((GLenum)arg1)
		//params((GLfloat)arg2)
{
#ifndef GL_ARB_vertex_program
        error("GEMglProgramLocalParameter4fvARB: GEM was compiled without GL_ARB_vertex_program");
        error("GEMglProgramLocalParameter4fvARB: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("target"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("index"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("params"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglProgramLocalParameter4fvARB :: ~GEMglProgramLocalParameter4fvARB () {
	inlet_free(m_inlet[0]);
	inlet_free(m_inlet[1]);
	inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglProgramLocalParameter4fvARB :: render(GemState *state) {
#ifdef GL_ARB_vertex_program
	glProgramLocalParameter4fvARB (target, index, params);
#endif
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglProgramLocalParameter4fvARB :: targetMess (t_float arg1) {	// FUN
	target = (GLenum)arg1;
	setModified();
}

void GEMglProgramLocalParameter4fvARB :: indexMess (t_float arg1) {	// FUN
	index = (GLenum)arg1;
	setModified();
}

void GEMglProgramLocalParameter4fvARB :: paramsMess (int argc, t_atom*argv) {	// FUN
	if(argc!=4){
		post("GEMglProgramLocalParamter4vARB:  needs 4 elements");
		return;
	}
	int i;
	for (i=0;i<4;i++){
		params[i] = (GLfloat)atom_getfloat(argv+i);
		post("params[%i] = %f\n",i,params[i]);
	}
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglProgramLocalParameter4fvARB :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglProgramLocalParameter4fvARB::targetMessCallback,  	gensym("target"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglProgramLocalParameter4fvARB::indexMessCallback,  	gensym("index"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglProgramLocalParameter4fvARB::paramsMessCallback,  	gensym("params"), A_GIMME, A_NULL);
};

void GEMglProgramLocalParameter4fvARB :: targetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->targetMess ( (t_float)    arg0);
}
void GEMglProgramLocalParameter4fvARB :: indexMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->indexMess ( (t_float)    arg0);
}
void GEMglProgramLocalParameter4fvARB :: paramsMessCallback (void* data, t_symbol*, int argc, t_atom* argv){
	GetMyClass(data)->paramsMess ( argc, argv);
}
