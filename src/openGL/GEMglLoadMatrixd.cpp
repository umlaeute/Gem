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

#include "GEMglLoadMatrixd.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglLoadMatrixd , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglLoadMatrixd
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglLoadMatrixd :: GEMglLoadMatrixd	(t_floatarg arg0=0)
{
#ifndef GL_VERSION_1_1
        error("GEMglLoadMatrixd: GEM was compiled without GL_VERSION_1_1");
        error("GEMglLoadMatrixd: therefore this object will do nothing");
#endif
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("list"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglLoadMatrixd :: ~GEMglLoadMatrixd () {
inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglLoadMatrixd :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glLoadMatrixd (m_matrix);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglLoadMatrixd :: matrixMess (int argc, t_atom*argv) {	// FUN
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

void GEMglLoadMatrixd :: obj_setupCallback(t_class *classPtr) {
//	 class_addmethod(classPtr, (t_method)&GEMglLoadMatrixd::matrixMessCallback,  	gensym("matrix"), A_DEFPOINTER, A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglLoadMatrixd::matrixMessCallback,  	
									gensym("list"), A_GIMME, A_NULL);
}

void GEMglLoadMatrixd :: matrixMessCallback (void* data, t_symbol*,int argc, t_atom*argv){
	GetMyClass(data)->matrixMess ( argc, argv );
}
