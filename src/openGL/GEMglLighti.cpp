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

#include "GEMglLighti.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglLighti , t_symbol*, A_DEFSYMBOL, t_symbol*, A_DEFSYMBOL, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglLighti
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglLighti :: GEMglLighti(t_symbol* arg1=0, t_symbol* arg2=0, t_floatarg arg3=0) :
             		light((GLenum)arg1),
		pname((GLenum)arg2),
		param((GLint)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("light"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("param"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglLighti :: ~GEMglLighti(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglLighti :: render(GemState *state)
{ glLighti(light, pname, param); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglLighti :: lightMess (int arg1) {
	light = (GLenum)arg1;
	setModified();
}


void GEMglLighti :: pnameMess (int arg1) {
	pname = (GLenum)arg1;
	setModified();
}


void GEMglLighti :: paramMess (int arg1) {
	param = (GLint)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglLighti :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglLighti,gensym("glLighti"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglLighti::lightMessCallback, gensym("light"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglLighti::pnameMessCallback, gensym("pname"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglLighti::paramMessCallback, gensym("param"), A_NULL);
}


void GEMglLighti :: lightMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->lightMess (getGLdefine(arg0->s_name));
}
void GEMglLighti :: pnameMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->pnameMess (getGLdefine(arg0->s_name));
}
void GEMglLighti :: paramMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->paramMess ( (t_int)    arg0);
}
