////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    this file has been generated automatically
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GEMglLightModelf.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglLightModelf , t_symbol*, A_DEFSYMBOL, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglLightModelf
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglLightModelf :: GEMglLightModelf(t_symbol* arg1=0, t_floatarg arg2=0) :
             		pname((GLenum)arg1),
		param((GLfloat)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("param"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglLightModelf :: ~GEMglLightModelf(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglLightModelf :: render(GemState *state)
{ glLightModelf(pname, param); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglLightModelf :: pnameMess (int arg1) {
	pname = (GLenum)arg1;
	setModified();
}


void GEMglLightModelf :: paramMess (t_float arg1) {
	param = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglLightModelf :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglLightModelf,gensym("glLightModelf"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglLightModelf::pnameMessCallback, gensym("pname"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglLightModelf::paramMessCallback, gensym("param"), A_NULL);
}


void GEMglLightModelf :: pnameMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->pnameMess (getGLdefine(arg0->s_name));
}
void GEMglLightModelf :: paramMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->paramMess ( (t_int)    arg0);
}
