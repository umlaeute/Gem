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

#include "GEMglLightf.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglLightf , t_symbol*, A_DEFSYMBOL, t_symbol*, A_DEFSYMBOL, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglLightf
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglLightf :: GEMglLightf(t_symbol* arg1=0, t_symbol* arg2=0, t_floatarg arg3=0) :
             		light((GLenum)arg1),
		pname((GLenum)arg2),
		param((GLfloat)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("light"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("param"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglLightf :: ~GEMglLightf(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglLightf :: render(GemState *state)
{ glLightf(light, pname, param); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglLightf :: lightMess (int arg1) {
	light = (GLenum)arg1;
	setModified();
}


void GEMglLightf :: pnameMess (int arg1) {
	pname = (GLenum)arg1;
	setModified();
}


void GEMglLightf :: paramMess (t_float arg1) {
	param = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglLightf :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglLightf,gensym("glLightf"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglLightf::lightMessCallback, gensym("light"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglLightf::pnameMessCallback, gensym("pname"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglLightf::paramMessCallback, gensym("param"), A_NULL);
}


void GEMglLightf :: lightMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->lightMess (getGLdefine(arg0->s_name));
}
void GEMglLightf :: pnameMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->pnameMess (getGLdefine(arg0->s_name));
}
void GEMglLightf :: paramMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->paramMess ( (t_int)    arg0);
}
