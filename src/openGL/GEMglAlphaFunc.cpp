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

#include "GEMglAlphaFunc.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglAlphaFunc , t_symbol*, A_DEFSYMBOL, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglAlphaFunc
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglAlphaFunc :: GEMglAlphaFunc(t_symbol* arg1=0, t_floatarg arg2=0) :
             		func((GLenum)arg1),
		ref((GLclampf)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("func"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ref"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglAlphaFunc :: ~GEMglAlphaFunc(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglAlphaFunc :: render(GemState *state)
{ glAlphaFunc(func, ref); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglAlphaFunc :: funcMess (int arg1) {
	func = (GLenum)arg1;
	setModified();
}


void GEMglAlphaFunc :: refMess (t_float arg1) {
	ref = (GLclampf)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglAlphaFunc :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglAlphaFunc,gensym("glAlphaFunc"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglAlphaFunc::funcMessCallback, gensym("func"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglAlphaFunc::refMessCallback, gensym("ref"), A_NULL);
}


void GEMglAlphaFunc :: funcMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->funcMess (getGLdefine(arg0->s_name));
}
void GEMglAlphaFunc :: refMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->refMess ( (t_int)    arg0);
}
