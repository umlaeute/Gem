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

#include "GEMglPixelStorei.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglPixelStorei , t_symbol*, A_DEFSYMBOL, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglPixelStorei
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglPixelStorei :: GEMglPixelStorei(t_symbol* arg1=0, t_floatarg arg2=0) :
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
GEMglPixelStorei :: ~GEMglPixelStorei(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglPixelStorei :: render(GemState *state)
{ glPixelStorei(pname, param); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglPixelStorei :: pnameMess (int arg1) {
	pname = (GLenum)arg1;
	setModified();
}


void GEMglPixelStorei :: paramMess (int arg1) {
	param = (GLint)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglPixelStorei :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglPixelStorei,gensym("glPixelStorei"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglPixelStorei::pnameMessCallback, gensym("pname"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglPixelStorei::paramMessCallback, gensym("param"), A_NULL);
}


void GEMglPixelStorei :: pnameMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->pnameMess (getGLdefine(arg0->s_name));
}
void GEMglPixelStorei :: paramMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->paramMess ( (t_int)    arg0);
}
