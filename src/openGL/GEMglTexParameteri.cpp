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

#include "GEMglTexParameteri.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglTexParameteri , t_symbol*, A_DEFSYMBOL, t_symbol*, A_DEFSYMBOL, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglTexParameteri
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglTexParameteri :: GEMglTexParameteri(t_symbol* arg1=0, t_symbol* arg2=0, t_floatarg arg3=0) :
             		target((GLenum)arg1),
		pname((GLenum)arg2),
		param((GLint)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("target"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("param"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglTexParameteri :: ~GEMglTexParameteri(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglTexParameteri :: render(GemState *state)
{ glTexParameteri(target, pname, param); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglTexParameteri :: targetMess (int arg1) {
	target = (GLenum)arg1;
	setModified();
}


void GEMglTexParameteri :: pnameMess (int arg1) {
	pname = (GLenum)arg1;
	setModified();
}


void GEMglTexParameteri :: paramMess (int arg1) {
	param = (GLint)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglTexParameteri :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglTexParameteri,gensym("glTexParameteri"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglTexParameteri::targetMessCallback, gensym("target"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexParameteri::pnameMessCallback, gensym("pname"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexParameteri::paramMessCallback, gensym("param"), A_NULL);
}


void GEMglTexParameteri :: targetMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->targetMess (getGLdefine(arg0->s_name));
}
void GEMglTexParameteri :: pnameMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->pnameMess (getGLdefine(arg0->s_name));
}
void GEMglTexParameteri :: paramMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->paramMess ( (t_int)    arg0);
}
