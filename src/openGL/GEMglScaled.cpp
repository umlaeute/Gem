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

#include "GEMglScaled.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglScaled , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglScaled
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglScaled :: GEMglScaled(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
             		x((GLdouble)arg1),
		y((GLdouble)arg2),
		z((GLdouble)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("z"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglScaled :: ~GEMglScaled(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglScaled :: render(GemState *state)
{ glScaled(x, y, z); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglScaled :: xMess (double arg1) {
	x = (GLdouble)arg1;
	setModified();
}


void GEMglScaled :: yMess (double arg1) {
	y = (GLdouble)arg1;
	setModified();
}


void GEMglScaled :: zMess (double arg1) {
	z = (GLdouble)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglScaled :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglScaled,gensym("glScaled"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglScaled::xMessCallback, gensym("x"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglScaled::yMessCallback, gensym("y"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglScaled::zMessCallback, gensym("z"), A_NULL);
}


void GEMglScaled :: xMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->xMess ( (t_int)    arg0);
}
void GEMglScaled :: yMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->yMess ( (t_int)    arg0);
}
void GEMglScaled :: zMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->zMess ( (t_int)    arg0);
}
