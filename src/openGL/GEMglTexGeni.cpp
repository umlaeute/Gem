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

#include "GEMglTexGeni.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglTexGeni , t_symbol*, A_DEFSYMBOL, t_symbol*, A_DEFSYMBOL, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglTexGeni
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglTexGeni :: GEMglTexGeni(t_symbol* arg1=0, t_symbol* arg2=0, t_floatarg arg3=0) :
             		coord((GLenum)arg1),
		pname((GLenum)arg2),
		param((GLint)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("coord"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("param"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglTexGeni :: ~GEMglTexGeni(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglTexGeni :: render(GemState *state)
{ glTexGeni(coord, pname, param); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglTexGeni :: coordMess (int arg1) {
	coord = (GLenum)arg1;
	setModified();
}


void GEMglTexGeni :: pnameMess (int arg1) {
	pname = (GLenum)arg1;
	setModified();
}


void GEMglTexGeni :: paramMess (int arg1) {
	param = (GLint)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglTexGeni :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglTexGeni,gensym("glTexGeni"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglTexGeni::coordMessCallback, gensym("coord"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexGeni::pnameMessCallback, gensym("pname"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexGeni::paramMessCallback, gensym("param"), A_NULL);
}


void GEMglTexGeni :: coordMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->coordMess (getGLdefine(arg0->s_name));
}
void GEMglTexGeni :: pnameMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->pnameMess (getGLdefine(arg0->s_name));
}
void GEMglTexGeni :: paramMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->paramMess ( (t_int)    arg0);
}
