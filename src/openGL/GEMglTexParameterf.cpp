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

#include "GEMglTexParameterf.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglTexParameterf , t_symbol*, A_DEFSYMBOL, t_symbol*, A_DEFSYMBOL, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglTexParameterf
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglTexParameterf :: GEMglTexParameterf(t_symbol* arg1=0, t_symbol* arg2=0, t_floatarg arg3=0) :
             		target((GLenum)arg1),
		pname((GLenum)arg2),
		param((GLfloat)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("target"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("param"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglTexParameterf :: ~GEMglTexParameterf(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglTexParameterf :: render(GemState *state)
{ glTexParameterf(target, pname, param); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglTexParameterf :: targetMess (int arg1) {
	target = (GLenum)arg1;
	setModified();
}


void GEMglTexParameterf :: pnameMess (int arg1) {
	pname = (GLenum)arg1;
	setModified();
}


void GEMglTexParameterf :: paramMess (t_float arg1) {
	param = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglTexParameterf :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglTexParameterf,gensym("glTexParameterf"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglTexParameterf::targetMessCallback, gensym("target"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexParameterf::pnameMessCallback, gensym("pname"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexParameterf::paramMessCallback, gensym("param"), A_NULL);
}


void GEMglTexParameterf :: targetMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->targetMess (getGLdefine(arg0->s_name));
}
void GEMglTexParameterf :: pnameMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->pnameMess (getGLdefine(arg0->s_name));
}
void GEMglTexParameterf :: paramMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->paramMess ( (t_int)    arg0);
}
