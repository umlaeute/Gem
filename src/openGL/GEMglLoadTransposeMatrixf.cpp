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

#include "GEMglLoadTransposeMatrixf.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglLoadTransposeMatrixf , t_floatarg, A_DEFFLOAT )

/////////////////////////////////////////////////////////
//
// GEMglLoadTransposeMatrixf
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglLoadTransposeMatrixf :: GEMglLoadTransposeMatrixf	(t_floatarg arg0=0)
{
#ifndef GL_VERSION_1_1
        error("GEMglLoadTransposeMatrixf: GEM was compiled without GL_VERSION_1_1");
        error("GEMglLoadTransposeMatrixf: therefore this object will do nothing");
#endif
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("list"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglLoadTransposeMatrixf :: ~GEMglLoadTransposeMatrixf () {
inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglLoadTransposeMatrixf :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glLoadTransposeMatrixf (m_matrix);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglLoadTransposeMatrixf :: matrixMess (int argc, t_atom*argv) {	// FUN
	if(argc!=16){
		post("GEMglLoadMatrixf: need 16 (4x4) elements");
		return;
		}
	int i;
	for (i=0;i<16;i++) {
	  m_matrix[i]=(GLfloat)atom_getfloat(argv+i);
	}
	setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglLoadTransposeMatrixf :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglLoadTransposeMatrixf::matrixMessCallback,  	gensym("list"), A_GIMME, A_NULL);
}

void GEMglLoadTransposeMatrixf :: matrixMessCallback (void* data, t_symbol*, int argc, t_atom*argv){
	GetMyClass(data)->matrixMess ( argc, argv);
}
