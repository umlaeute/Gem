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

#include "GEMglBlendFunc.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglBlendFunc , t_symbol*, A_DEFSYMBOL, t_symbol*, A_DEFSYMBOL)

/////////////////////////////////////////////////////////
//
// GEMglBlendFunc
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglBlendFunc :: GEMglBlendFunc(t_symbol* arg1=0, t_symbol* arg2=0) :
             		sfactor((GLenum)arg1),
		dfactor((GLenum)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("sfactor"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("dfactor"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglBlendFunc :: ~GEMglBlendFunc(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglBlendFunc :: render(GemState *state)
{ glBlendFunc(sfactor, dfactor); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglBlendFunc :: sfactorMess (int arg1) {
	sfactor = (GLenum)arg1;
	setModified();
}


void GEMglBlendFunc :: dfactorMess (int arg1) {
	dfactor = (GLenum)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglBlendFunc :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglBlendFunc,gensym("glBlendFunc"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglBlendFunc::sfactorMessCallback, gensym("sfactor"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglBlendFunc::dfactorMessCallback, gensym("dfactor"), A_NULL);
}


void GEMglBlendFunc :: sfactorMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->sfactorMess (getGLdefine(arg0->s_name));
}
void GEMglBlendFunc :: dfactorMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->dfactorMess (getGLdefine(arg0->s_name));
}
