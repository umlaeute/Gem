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

#include "GEMglLoadTransposeMatrixd.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglLoadTransposeMatrixd , t_floatarg, A_DEFFLOAT )

/////////////////////////////////////////////////////////
//
// GEMglLoadTransposeMatrixf
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglLoadTransposeMatrixd :: GEMglLoadTransposeMatrixd	(t_floatarg arg0=0)
{
#ifndef GL_VERSION_1_3
        error("GEMglLoadTransposeMatrixd: GEM was compiled without GL_VERSION_1_3");
        error("GEMglLoadTransposeMatrixd: therefore this object will do nothing");
#endif
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("list"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglLoadTransposeMatrixd :: ~GEMglLoadTransposeMatrixd () {
inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglLoadTransposeMatrixd :: render(GemState *state) {
#ifdef GL_VERSION_1_3
	glLoadTransposeMatrixd (m_matrix);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglLoadTransposeMatrixd :: matrixMess (int argc, t_atom*argv) {	// FUN
	if(argc!=16){
		post("GEMglLoadMatrixd: need 16 (4x4) elements");
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

void GEMglLoadTransposeMatrixd :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglLoadTransposeMatrixd::matrixMessCallback,  	gensym("list"), A_GIMME, A_NULL);
}

void GEMglLoadTransposeMatrixd :: matrixMessCallback (void* data, t_symbol*, int argc, t_atom*argv){
	GetMyClass(data)->matrixMess ( argc, argv);
}
