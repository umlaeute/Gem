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

#include "GEMglScalef.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglScalef , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglScalef
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglScalef :: GEMglScalef(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
             		x((GLfloat)arg1),
		y((GLfloat)arg2),
		z((GLfloat)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("z"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglScalef :: ~GEMglScalef(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglScalef :: render(GemState *state)
{ glScalef(x, y, z); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglScalef :: xMess (t_float arg1) {
	x = (GLfloat)arg1;
	setModified();
}


void GEMglScalef :: yMess (t_float arg1) {
	y = (GLfloat)arg1;
	setModified();
}


void GEMglScalef :: zMess (t_float arg1) {
	z = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglScalef :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglScalef,gensym("glScalef"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglScalef::xMessCallback, gensym("x"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglScalef::yMessCallback, gensym("y"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglScalef::zMessCallback, gensym("z"), A_NULL);
}


void GEMglScalef :: xMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->xMess ( (t_int)    arg0);
}
void GEMglScalef :: yMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->yMess ( (t_int)    arg0);
}
void GEMglScalef :: zMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->zMess ( (t_int)    arg0);
}
