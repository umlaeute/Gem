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

#include "GEMglLogicOp.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglLogicOp , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglLogicOp :: GEMglLogicOp	(t_floatarg arg0=0) :
		opcode((GLenum)arg0)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("opcode"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglLogicOp :: ~GEMglLogicOp () {
inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglLogicOp :: render(GemState *state) {
	glLogicOp (opcode);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglLogicOp :: opcodeMess (t_float arg1) {	// FUN
	opcode = (GLenum)arg1;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglLogicOp :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglLogicOp::opcodeMessCallback,  	gensym("opcode"), A_DEFFLOAT, A_NULL);
};

void GEMglLogicOp :: opcodeMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->opcodeMess ( (t_float)    arg0);
}
