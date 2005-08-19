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

#include "GEMglLoadMatrixf.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglLoadMatrixf , t_floatarg, A_DEFFLOAT )

/////////////////////////////////////////////////////////
//
// GEMglLoadMatrixf
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglLoadMatrixf :: GEMglLoadMatrixf	(t_floatarg arg0=0) //:
		//matrix((GLfloat)arg0)
{
#ifndef GL_VERSION_1_1
        error("GEMglLoadMatrixf: GEM was compiled without GL_VERSION_1_1");
        error("GEMglLoadMatrixf: therefore this object will do nothing");
#endif
	//m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("matrix"));
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("list"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglLoadMatrixf :: ~GEMglLoadMatrixf () {
inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglLoadMatrixf :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glLoadMatrixf (m_matrix);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglLoadMatrixf :: matrixMess (int argc, t_atom*argv) {	// FUN
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

void GEMglLoadMatrixf :: obj_setupCallback(t_class *classPtr) {
	 //class_addmethod(classPtr, (t_method)&GEMglLoadMatrixf::matrixMessCallback,  	gensym("matrix"), A_DEFFLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglLoadMatrixf::matrixMessCallback,  	
						gensym("list"), A_GIMME, A_NULL);
}

void GEMglLoadMatrixf :: matrixMessCallback (void* data, t_symbol*,int argc, t_atom*argv){
	GetMyClass(data)->matrixMess ( argc, argv );
}
