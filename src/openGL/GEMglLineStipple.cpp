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

#include "GEMglLineStipple.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglLineStipple , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglLineStipple
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglLineStipple :: GEMglLineStipple(t_floatarg arg1=0, t_floatarg arg2=0) :
             		factor((GLint)arg1),
		pattern((GLushort)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("factor"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("pattern"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglLineStipple :: ~GEMglLineStipple(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglLineStipple :: render(GemState *state)
{ glLineStipple(factor, pattern); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglLineStipple :: factorMess (int arg1) {
	factor = (GLint)arg1;
	setModified();
}


void GEMglLineStipple :: patternMess (int arg1) {
	pattern = (GLushort)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglLineStipple :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglLineStipple,gensym("glLineStipple"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglLineStipple::factorMessCallback, gensym("factor"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglLineStipple::patternMessCallback, gensym("pattern"), A_NULL);
}


void GEMglLineStipple :: factorMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->factorMess ( (t_int)    arg0);
}
void GEMglLineStipple :: patternMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->patternMess ( (t_int)    arg0);
}
