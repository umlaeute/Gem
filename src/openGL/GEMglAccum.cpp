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

#include "GEMglAccum.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglAccum , t_symbol *, A_DEFSYM, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglAccum
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglAccum :: GEMglAccum(t_symbol *arg1, t_floatarg arg2=0) :
		value((GLfloat)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("op"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("value"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglAccum :: ~GEMglAccum(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglAccum :: render(GemState *state)
{ glAccum(op, value); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglAccum :: opMess (int arg1) {
	op = (GLenum)arg1;
	setModified();
}


void GEMglAccum :: valueMess (t_float arg1) {
	value = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglAccum :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglAccum,gensym("glAccum"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglAccum::opMessCallback, gensym("op"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglAccum::valueMessCallback, gensym("value"), A_NULL);
}


void GEMglAccum :: opMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->opMess (getGLdefine(arg0->s_name));
}
void GEMglAccum :: valueMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->valueMess ( (t_int)    arg0);
}
