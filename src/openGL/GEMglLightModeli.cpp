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

#include "GEMglLightModeli.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglLightModeli , t_symbol*, A_DEFSYMBOL, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglLightModeli
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglLightModeli :: GEMglLightModeli(t_symbol* arg1=0, t_floatarg arg2=0) :
             		pname((GLenum)arg1),
		param((GLint)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("param"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglLightModeli :: ~GEMglLightModeli(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglLightModeli :: render(GemState *state)
{ glLightModeli(pname, param); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglLightModeli :: pnameMess (int arg1) {
	pname = (GLenum)arg1;
	setModified();
}


void GEMglLightModeli :: paramMess (int arg1) {
	param = (GLint)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglLightModeli :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglLightModeli,gensym("glLightModeli"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglLightModeli::pnameMessCallback, gensym("pname"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglLightModeli::paramMessCallback, gensym("param"), A_NULL);
}


void GEMglLightModeli :: pnameMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->pnameMess (getGLdefine(arg0->s_name));
}
void GEMglLightModeli :: paramMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->paramMess ( (t_int)    arg0);
}
