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

#include "GEMglGetFloatv.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglGetFloatv , t_floatarg, A_DEFFLOAT )

/////////////////////////////////////////////////////////
//
// GEMglGetFloatv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglGetFloatv :: GEMglGetFloatv	(t_floatarg arg0=0) {
#ifndef GL_VERSION_1_1
        error("GEMglGetFloatv: GEM was compiled without GL_VERSION_1_1");
        error("GEMglGetFloatv: therefore this object will do nothing");
#endif
	pnameMess(arg0);
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
    m_outlet = outlet_new(this->x_obj, 0);

}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglGetFloatv :: ~GEMglGetFloatv () {
	inlet_free(m_inlet);
	outlet_free(m_outlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglGetFloatv :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	  float mi[16]={0};

	glGetFloatv(pname,mi);

	SETFLOAT(m_alist+0, mi[0]);
    SETFLOAT(m_alist+1, mi[1]);
    SETFLOAT(m_alist+2, mi[2]);
    SETFLOAT(m_alist+3, mi[3]);
    SETFLOAT(m_alist+4, mi[4]);
    SETFLOAT(m_alist+5, mi[5]);
    SETFLOAT(m_alist+6, mi[6]);
    SETFLOAT(m_alist+7, mi[7]);
    SETFLOAT(m_alist+8, mi[8]);
    SETFLOAT(m_alist+9, mi[9]);
    SETFLOAT(m_alist+10, mi[10]);
    SETFLOAT(m_alist+11, mi[11]);
    SETFLOAT(m_alist+12, mi[12]);
    SETFLOAT(m_alist+13, mi[13]);
    SETFLOAT(m_alist+14, mi[14]);
    SETFLOAT(m_alist+15, mi[15]);

	outlet_list(m_outlet, gensym("list"), 16, m_alist);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglGetFloatv :: pnameMess (t_float arg0) {	// FUN
  pname=(GLenum)arg0;
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglGetFloatv :: obj_setupCallback(t_class *classPtr) {
  class_addmethod(classPtr, (t_method)&GEMglGetFloatv::pnameMessCallback,  	gensym("pname"), A_DEFFLOAT, A_NULL);
}

void GEMglGetFloatv :: pnameMessCallback (void* data, t_floatarg arg0) {
	GetMyClass(data)->pnameMess ( arg0 );
}
