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

#include "GEMglVertex3d.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglVertex3d , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglVertex3d
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglVertex3d :: GEMglVertex3d(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
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
GEMglVertex3d :: ~GEMglVertex3d(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglVertex3d :: render(GemState *state)
{ glVertex3d(x, y, z); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglVertex3d :: xMess (t_float arg1) {
	x = (GLdouble)arg1;
	setModified();
}


void GEMglVertex3d :: yMess (t_float arg1) {
	y = (GLdouble)arg1;
	setModified();
}


void GEMglVertex3d :: zMess (t_float arg1) {
	z = (GLdouble)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglVertex3d :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglVertex3d,gensym("glVertex3d"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglVertex3d::xMessCallback, gensym("x"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglVertex3d::yMessCallback, gensym("y"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglVertex3d::zMessCallback, gensym("z"), A_NULL);
}


void GEMglVertex3d :: xMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->xMess (arg0);
}
void GEMglVertex3d :: yMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->yMess (arg0);
}
void GEMglVertex3d :: zMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->zMess (arg0);
}
