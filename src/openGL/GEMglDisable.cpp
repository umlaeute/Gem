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

#include "GEMglDisable.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglDisable , t_symbol*, A_DEFSYMBOL)

/////////////////////////////////////////////////////////
//
// GEMglDisable
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglDisable :: GEMglDisable(t_symbol* arg1=0) :
             		cap((GLenum)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("cap"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglDisable :: ~GEMglDisable(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglDisable :: render(GemState *state)
{ glDisable(cap); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglDisable :: capMess (int arg1) {
	cap = (GLenum)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglDisable :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglDisable,gensym("glDisable"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglDisable::capMessCallback, gensym("cap"), A_NULL);
}


void GEMglDisable :: capMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->capMess (getGLdefine(arg0->s_name));
}
