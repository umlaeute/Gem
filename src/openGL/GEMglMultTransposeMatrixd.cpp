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

#include "GEMglMultTransposeMatrixd.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglMultTransposeMatrixd , t_floatarg, A_DEFFLOAT )

/////////////////////////////////////////////////////////
//
// GEMglMultTransposeMatrixd
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglMultTransposeMatrixd :: GEMglMultTransposeMatrixd	(t_floatarg arg0=0)
{
#ifndef GL_VERSION_1_3
        error("GEMglMultTransposeMatrixd: GEM was compiled without GL_VERSION_1_3");
        error("GEMglMultTransposeMatrixd: therefore this object will do nothing");
#endif
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("matrix"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglMultTransposeMatrixd :: ~GEMglMultTransposeMatrixd () {
	inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglMultTransposeMatrixd :: render(GemState *state) {
#ifdef GL_VERSION_1_3
	glMultTransposeMatrixd (m_matrix);
#endif // GL_VERSION_1_3
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglMultTransposeMatrixd :: matrixMess (int argc, t_atom* argv) {	// FUN
	if(argc!=16){
		post("GEMglMultTransposeMatrixd: need 16 (4x4) elements");
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

void GEMglMultTransposeMatrixd :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglMultTransposeMatrixd::matrixMessCallback,  	
							gensym("list"), A_GIMME, A_NULL);
}

void GEMglMultTransposeMatrixd :: matrixMessCallback (void* data, t_symbol*,int argc, t_atom*argv){
	GetMyClass(data)->matrixMess ( argc, argv);
}
