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

#include "GEMglHint.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglHint , t_symbol*, A_DEFSYMBOL, t_symbol*, A_DEFSYMBOL)

/////////////////////////////////////////////////////////
//
// GEMglHint
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglHint :: GEMglHint(t_symbol* arg1=0, t_symbol* arg2=0) :
             		target((GLenum)arg1),
		mode((GLenum)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("target"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mode"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglHint :: ~GEMglHint(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglHint :: render(GemState *state)
{ glHint(target, mode); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglHint :: targetMess (int arg1) {
	target = (GLenum)arg1;
	setModified();
}


void GEMglHint :: modeMess (int arg1) {
	mode = (GLenum)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglHint :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglHint,gensym("glHint"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglHint::targetMessCallback, gensym("target"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglHint::modeMessCallback, gensym("mode"), A_NULL);
}


void GEMglHint :: targetMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->targetMess (getGLdefine(arg0->s_name));
}
void GEMglHint :: modeMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->modeMess (getGLdefine(arg0->s_name));
}
