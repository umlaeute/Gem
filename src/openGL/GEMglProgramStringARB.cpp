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

#include "GEMglProgramStringARB.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglProgramStringARB )

/////////////////////////////////////////////////////////
//
// GEMglProgramStringARB
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglProgramStringARB :: GEMglProgramStringARB	(int argc, t_atom*argv) :
		target((GLenum)0), 
		format((GLenum)0), 
		len((GLsizei)0),
                string(NULL)
{
#ifndef GL_ARB_vertex_program
        error("GEMglProgramStringARB: GEM was compiled without GL_ARB_vertex_program");
        error("GEMglProgramStringARB: therefore this object will do nothing");
#endif

        switch (argc) {
        default:
        case 4:
          string = (GLvoid*)atom_getsymbol(argv+3)->s_name;
        case 3:
          len=atom_getint(argv+2);
        case 2:
          format=atom_getint(argv+1);
        case 1:
          target=atom_getint(argv+0);
        case 0:
          break;
        }

	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("target"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("format"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("len"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_symbol, gensym("string"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglProgramStringARB :: ~GEMglProgramStringARB ()
{
	inlet_free(m_inlet[0]);
	inlet_free(m_inlet[1]);
	inlet_free(m_inlet[2]);
	inlet_free(m_inlet[3]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglProgramStringARB :: render(GemState *state) 
{
#ifdef GL_ARB_vertex_program
	glProgramStringARB (target, format, len, string);
#endif
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglProgramStringARB :: targetMess (t_float arg1) {	// FUN
	target = (GLenum)arg1;
	setModified();
}

void GEMglProgramStringARB :: formatMess (t_float arg1) {	// FUN
	format = (GLenum)arg1;
	setModified();
}

void GEMglProgramStringARB :: lenMess (t_float arg1) {	// FUN
	len = (GLsizei)arg1;
	setModified();
}

void GEMglProgramStringARB :: stringMess (t_symbol* arg1) {	// FUN
	string = (GLvoid*)arg1->s_name;
	setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglProgramStringARB :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglProgramStringARB::targetMessCallback,
									gensym("target"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglProgramStringARB::formatMessCallback, 
									gensym("format"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglProgramStringARB::lenMessCallback, 
									gensym("len"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglProgramStringARB::stringMessCallback, 
									gensym("string"), A_DEFSYMBOL, A_NULL);
};

void GEMglProgramStringARB :: targetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->targetMess ( (t_float)    arg0);
}
void GEMglProgramStringARB :: formatMessCallback (void* data, t_floatarg arg1){
	GetMyClass(data)->formatMess ( (t_float)    arg1);
}
void GEMglProgramStringARB :: lenMessCallback (void* data, t_floatarg arg2){
	GetMyClass(data)->lenMess ( (t_float)    arg2);
}
void GEMglProgramStringARB :: stringMessCallback (void* data, t_symbol* arg3){
	GetMyClass(data)->stringMess ( (t_symbol*)    arg3);
}
