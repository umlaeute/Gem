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

#include "GEMglEnable.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglEnable , t_symbol*, A_DEFSYMBOL)

/////////////////////////////////////////////////////////
//
// GEMglEnable
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglEnable :: GEMglEnable(t_symbol* arg1=0) :
             		cap((GLenum)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("cap"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglEnable :: ~GEMglEnable(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglEnable :: render(GemState *state)
{ glEnable(cap); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglEnable :: capMess (int arg1) {
	cap = (GLenum)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglEnable :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglEnable,gensym("glEnable"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglEnable::capMessCallback, gensym("cap"), A_NULL);
}


void GEMglEnable :: capMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->capMess (getGLdefine(arg0->s_name));
}
