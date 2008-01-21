////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//	zmoelnig@iem.kug.ac.at
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
//  this file has been generated...
////////////////////////////////////////////////////////

#include "GEMglFogiv.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglFogiv )

/////////////////////////////////////////////////////////
//
// GEMglFogiv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglFogiv :: GEMglFogiv	(int argc, t_atom *argv) {
#ifndef GL_VERSION_1_1
        error("GEMglFogiv: GEM was compiled without GL_VERSION_1_1");
        error("GEMglFogiv: therefore this object will do nothing");
#endif
	int i=FOG_ARRAY_LENGTH;
	while(i--)params[i]=0;

	pnameMess(atom_getfloat(argv));
	if (argc>0)paramsMess(argc-1, argv+1);

	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("params"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglFogiv :: ~GEMglFogiv () {
	inlet_free(m_inlet[0]);
	inlet_free(m_inlet[1]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglFogiv :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glFogiv (pname, params);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglFogiv :: pnameMess (t_float arg0) {	// FUN
  pname=(GLenum)arg0;
  setModified();
}
void GEMglFogiv :: paramsMess (int argc, t_atom*argv) {	// FUN
  int i = (argc<FOG_ARRAY_LENGTH)?argc:FOG_ARRAY_LENGTH;
  while(i--)params[i]=atom_getint(argv+i);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglFogiv :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglFogiv::pnameMessCallback,  	gensym("pname"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglFogiv::paramsMessCallback,  	gensym("params"), A_GIMME, A_NULL);
}

void GEMglFogiv :: pnameMessCallback (void* data, t_floatarg arg0) {
	GetMyClass(data)->pnameMess (arg0);
}
void GEMglFogiv :: paramsMessCallback (void* data, t_symbol*, int argc, t_atom* argv) {
	GetMyClass(data)->paramsMess (argc, argv);
}
