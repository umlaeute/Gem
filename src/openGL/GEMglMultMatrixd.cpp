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

#include "GEMglMultMatrixd.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglMultMatrixd , t_floatarg, A_DEFFLOAT )

/////////////////////////////////////////////////////////
//
// GEMglMultMatrixd
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglMultMatrixd :: GEMglMultMatrixd	(t_floatarg arg0=0)
{
#ifndef GL_VERSION_1_1
        error("GEMglMultMatrixd: GEM was compiled without GL_VERSION_1_1");
        error("GEMglMultMatrixd: therefore this object will do nothing");
#endif
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("list"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglMultMatrixd :: ~GEMglMultMatrixd () {
	inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglMultMatrixd :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glMultMatrixd (m_matrix);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglMultMatrixd :: matrixMess (int argc, t_atom*argv) {	// FUN
	if(argc!=16){
		post("GEMglMultMatrixd: need 16 (4x4) elements");
		return;
		}
	int i;
	for (i=0;i<16;i++) {
	  m_matrix[i]=(GLdouble)atom_getfloat(argv+i);
	}
	setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglMultMatrixd :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglMultMatrixd::matrixMessCallback,  	
										gensym("list"), A_GIMME, A_NULL);
}

void GEMglMultMatrixd :: matrixMessCallback (void* data, t_symbol*,int argc, t_atom*argv){
	GetMyClass(data)->matrixMess ( argc, argv);
}
