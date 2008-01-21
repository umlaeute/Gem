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

#include "GEMglGenProgramsARB.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglGenProgramsARB)

/////////////////////////////////////////////////////////
//
// GEMglGenProgramsARB
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglGenProgramsARB :: GEMglGenProgramsARB	(int argc, t_atom*argv) :
  n(0), programs(NULL)
{
#ifndef GL_ARB_vertex_program
        error("GEMglGenProgramsARB: GEM was compiled without GL_ARB_vertex_program");
        error("GEMglGenProgramsARB: therefore this object will do nothing");
#endif
	programsMess(argc, argv);
	
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("programs"));

}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglGenProgramsARB :: ~GEMglGenProgramsARB () {
inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglGenProgramsARB :: render(GemState *state) {
#ifdef GL_ARB_vertex_program
	glGenProgramsARB (n, programs);
#endif // GL_ARB_vertex_program
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglGenProgramsARB :: programsMess (int argc, t_atom*argv) {	// FUN
  n=0;
  delete [] programs;
  programs = new GLuint[argc];
  while(argc--){
    if(argv->a_type == A_FLOAT)programs[n++] = (GLuint)atom_getint(argv);
    argv++;
  }
  setModified();
}



/////////////////////////////////////////////////////////
// static member functions
//

void GEMglGenProgramsARB :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglGenProgramsARB::programsMessCallback,  	gensym("programs"), A_GIMME, A_NULL);
}
void GEMglGenProgramsARB :: programsMessCallback (void* data, t_symbol*, int argc, t_atom*argv){
	GetMyClass(data)->programsMess (argc,argv);
}
