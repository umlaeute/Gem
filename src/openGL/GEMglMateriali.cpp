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

#include "GEMglMateriali.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglMateriali , t_symbol*, A_DEFSYMBOL, t_symbol*, A_DEFSYMBOL, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglMateriali
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglMateriali :: GEMglMateriali(t_symbol* arg1=0, t_symbol* arg2=0, t_floatarg arg3=0) :
             		face((GLenum)arg1),
		pname((GLenum)arg2),
		param((GLint)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("face"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("param"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglMateriali :: ~GEMglMateriali(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglMateriali :: render(GemState *state)
{ glMateriali(face, pname, param); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglMateriali :: faceMess (int arg1) {
	face = (GLenum)arg1;
	setModified();
}


void GEMglMateriali :: pnameMess (int arg1) {
	pname = (GLenum)arg1;
	setModified();
}


void GEMglMateriali :: paramMess (int arg1) {
	param = (GLint)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglMateriali :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglMateriali,gensym("glMateriali"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglMateriali::faceMessCallback, gensym("face"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglMateriali::pnameMessCallback, gensym("pname"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglMateriali::paramMessCallback, gensym("param"), A_NULL);
}


void GEMglMateriali :: faceMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->faceMess (getGLdefine(arg0->s_name));
}
void GEMglMateriali :: pnameMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->pnameMess (getGLdefine(arg0->s_name));
}
void GEMglMateriali :: paramMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->paramMess ( (t_int)    arg0);
}
