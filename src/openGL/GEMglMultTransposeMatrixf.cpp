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

#include "GEMglMultTransposeMatrixf.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglMultTransposeMatrixf , t_floatarg, A_DEFFLOAT )

/////////////////////////////////////////////////////////
//
// GEMglMultTransposeMatrixf
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglMultTransposeMatrixf :: GEMglMultTransposeMatrixf	(t_floatarg arg0=0)
{
#ifndef GL_VERSION_1_3
        error("GEMglMultTransposeMatrixf: GEM was compiled without GL_VERSION_1_3");
        error("GEMglMultTransposeMatrixf: therefore this object will do nothing");
#endif
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("matrix"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglMultTransposeMatrixf :: ~GEMglMultTransposeMatrixf () {
	inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglMultTransposeMatrixf :: render(GemState *state) {
#ifdef GL_VERSION_1_3
	glMultTransposeMatrixf (m_matrix);
#endif // GL_VERSION_1_3
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglMultTransposeMatrixf :: matrixMess (int argc, t_atom* argv) {	// FUN
	if(argc!=16){
		post("GEMglLoadMatrixd: need 16 (4x4) elements");
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

void GEMglMultTransposeMatrixf :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglMultTransposeMatrixf::matrixMessCallback,  	
							gensym("list"), A_GIMME, A_NULL);
}

void GEMglMultTransposeMatrixf :: matrixMessCallback (void* data, t_symbol*,int argc, t_atom*argv){
	GetMyClass(data)->matrixMess ( argc, argv);
}
