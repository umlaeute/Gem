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

#include "GEMglStencilFunc.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglStencilFunc , t_symbol*, A_DEFSYMBOL, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglStencilFunc
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglStencilFunc :: GEMglStencilFunc(t_symbol* arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
             		func((GLenum)arg1),
		ref((GLint)arg2),
		mask((GLuint)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("func"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ref"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mask"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglStencilFunc :: ~GEMglStencilFunc(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglStencilFunc :: render(GemState *state)
{ glStencilFunc(func, ref, mask); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglStencilFunc :: funcMess (int arg1) {
	func = (GLenum)arg1;
	setModified();
}


void GEMglStencilFunc :: refMess (int arg1) {
	ref = (GLint)arg1;
	setModified();
}


void GEMglStencilFunc :: maskMess (int arg1) {
	mask = (GLuint)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglStencilFunc :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglStencilFunc,gensym("glStencilFunc"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglStencilFunc::funcMessCallback, gensym("func"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglStencilFunc::refMessCallback, gensym("ref"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglStencilFunc::maskMessCallback, gensym("mask"), A_NULL);
}


void GEMglStencilFunc :: funcMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->funcMess (getGLdefine(arg0->s_name));
}
void GEMglStencilFunc :: refMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->refMess ( (t_int)    arg0);
}
void GEMglStencilFunc :: maskMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->maskMess ( (t_int)    arg0);
}
