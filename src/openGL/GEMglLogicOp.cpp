////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    this file has been generated automatically
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GEMglLogicOp.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglLogicOp , t_symbol*, A_DEFSYMBOL)

/////////////////////////////////////////////////////////
//
// GEMglLogicOp
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglLogicOp :: GEMglLogicOp(t_symbol* arg1=0) :
             		opcode((GLenum)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("opcode"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglLogicOp :: ~GEMglLogicOp(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglLogicOp :: render(GemState *state)
{ glLogicOp(opcode); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglLogicOp :: opcodeMess (int arg1) {
	opcode = (GLenum)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglLogicOp :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglLogicOp,gensym("glLogicOp"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglLogicOp::opcodeMessCallback, gensym("opcode"), A_NULL);
}


void GEMglLogicOp :: opcodeMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->opcodeMess (getGLdefine(arg0->s_name));
}
