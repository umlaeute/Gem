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

#include "GEMglDepthFunc.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglDepthFunc , t_symbol*, A_DEFSYMBOL)

/////////////////////////////////////////////////////////
//
// GEMglDepthFunc
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglDepthFunc :: GEMglDepthFunc(t_symbol* arg1=0) :
             		func((GLenum)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("func"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglDepthFunc :: ~GEMglDepthFunc(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglDepthFunc :: render(GemState *state)
{ glDepthFunc(func); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglDepthFunc :: funcMess (int arg1) {
	func = (GLenum)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglDepthFunc :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglDepthFunc,gensym("glDepthFunc"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglDepthFunc::funcMessCallback, gensym("func"), A_NULL);
}


void GEMglDepthFunc :: funcMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->funcMess (getGLdefine(arg0->s_name));
}
