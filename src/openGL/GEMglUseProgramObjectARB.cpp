////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2005 tigital@mac.com
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
////////////////////////////////////////////////////////

#include "GEMglUseProgramObjectARB.h"

CPPEXTERN_NEW ( GEMglUseProgramObjectARB )

/////////////////////////////////////////////////////////
//
// GEMglUseProgramObjectARB
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglUseProgramObjectARB :: GEMglUseProgramObjectARB() :
		m_program(0)
{
#ifndef GL_ARB_shader_objects
        error("GEMglUseProgramObjectARB: GEM was compiled without GL_ARB_shader_objects");
        error("GEMglUseProgramObjectARB: therefore this object will do nothing");
#endif
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("program"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglUseProgramObjectARB :: ~GEMglUseProgramObjectARB () {
	inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglUseProgramObjectARB :: render(GemState *state) {
#ifdef GL_ARB_shader_objects
	glUseProgramObjectARB ( m_program );
#endif // GL_ARB_shader_objects
}

/////////////////////////////////////////////////////////
// postrender
//
void GEMglUseProgramObjectARB :: postrender(GemState *state) {
#ifdef GL_ARB_shader_objects
	glUseProgramObjectARB (0);
#endif // GL_ARB_shader_objects
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglUseProgramObjectARB :: programMess (int program) {	// FUN
	m_program = (t_GLshaderObj)program;
	setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglUseProgramObjectARB :: obj_setupCallback(t_class *classPtr)
{
	 class_addmethod(classPtr, (t_method)&GEMglUseProgramObjectARB::programMessCallback,  	
									gensym("program"), A_FLOAT, A_NULL);
}

void GEMglUseProgramObjectARB :: programMessCallback (void* data, t_floatarg program){
	GetMyClass(data)->programMess ( (int)program );
}
