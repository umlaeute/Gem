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

#include "GEMglPixelTransferi.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglPixelTransferi , t_symbol*, A_DEFSYMBOL, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglPixelTransferi
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglPixelTransferi :: GEMglPixelTransferi(t_symbol* arg1=0, t_floatarg arg2=0) :
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
GEMglPixelTransferi :: ~GEMglPixelTransferi(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglPixelTransferi :: render(GemState *state)
{ glPixelTransferi(pname, param); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglPixelTransferi :: pnameMess (int arg1) {
	pname = (GLenum)arg1;
	setModified();
}


void GEMglPixelTransferi :: paramMess (int arg1) {
	param = (GLint)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglPixelTransferi :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglPixelTransferi,gensym("glPixelTransferi"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglPixelTransferi::pnameMessCallback, gensym("pname"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglPixelTransferi::paramMessCallback, gensym("param"), A_NULL);
}


void GEMglPixelTransferi :: pnameMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->pnameMess (getGLdefine(arg0->s_name));
}
void GEMglPixelTransferi :: paramMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->paramMess ( (t_int)    arg0);
}
