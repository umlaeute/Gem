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

#include "GEMglTexGenf.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglTexGenf , t_symbol*, A_DEFSYMBOL, t_symbol*, A_DEFSYMBOL, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglTexGenf
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglTexGenf :: GEMglTexGenf(t_symbol* arg1=0, t_symbol* arg2=0, t_floatarg arg3=0) :
             		coord((GLenum)arg1),
		pname((GLenum)arg2),
		param((GLfloat)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("coord"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pname"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("param"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglTexGenf :: ~GEMglTexGenf(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglTexGenf :: render(GemState *state)
{ glTexGenf(coord, pname, param); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglTexGenf :: coordMess (int arg1) {
	coord = (GLenum)arg1;
	setModified();
}


void GEMglTexGenf :: pnameMess (int arg1) {
	pname = (GLenum)arg1;
	setModified();
}


void GEMglTexGenf :: paramMess (t_float arg1) {
	param = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglTexGenf :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglTexGenf,gensym("glTexGenf"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglTexGenf::coordMessCallback, gensym("coord"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexGenf::pnameMessCallback, gensym("pname"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexGenf::paramMessCallback, gensym("param"), A_NULL);
}


void GEMglTexGenf :: coordMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->coordMess (getGLdefine(arg0->s_name));
}
void GEMglTexGenf :: pnameMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->pnameMess (getGLdefine(arg0->s_name));
}
void GEMglTexGenf :: paramMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->paramMess ( (t_int)    arg0);
}
