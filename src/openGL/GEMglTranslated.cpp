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

#include "GEMglTranslated.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglTranslated , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglTranslated
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglTranslated :: GEMglTranslated(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
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
GEMglTranslated :: ~GEMglTranslated(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglTranslated :: render(GemState *state)
{ glTranslated(x, y, z); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglTranslated :: xMess (double arg1) {
	x = (GLdouble)arg1;
	setModified();
}


void GEMglTranslated :: yMess (double arg1) {
	y = (GLdouble)arg1;
	setModified();
}


void GEMglTranslated :: zMess (double arg1) {
	z = (GLdouble)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglTranslated :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglTranslated,gensym("glTranslated"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglTranslated::xMessCallback, gensym("x"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTranslated::yMessCallback, gensym("y"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTranslated::zMessCallback, gensym("z"), A_NULL);
}


void GEMglTranslated :: xMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->xMess ( (t_int)    arg0);
}
void GEMglTranslated :: yMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->yMess ( (t_int)    arg0);
}
void GEMglTranslated :: zMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->zMess ( (t_int)    arg0);
}
