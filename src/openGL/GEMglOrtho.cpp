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

#include "GEMglOrtho.h"

CPPEXTERN_NEW_WITH_GIMME(GEMglOrtho)

/////////////////////////////////////////////////////////
//
// GEMglOrtho
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglOrtho :: GEMglOrtho (int argc, t_atom *argv)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("left"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("right"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("bottom"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("top"));
	m_inlet[4] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("zNear"));
	m_inlet[5] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("zFar"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglOrtho :: ~GEMglOrtho(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
inlet_free(m_inlet[4]);
inlet_free(m_inlet[5]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglOrtho :: render(GemState *state)
{ glOrtho(left, right, bottom, top, zNear, zFar); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglOrtho :: leftMess (double arg1) {
	left = (GLdouble)arg1;
	setModified();
}


void GEMglOrtho :: rightMess (double arg1) {
	right = (GLdouble)arg1;
	setModified();
}


void GEMglOrtho :: bottomMess (double arg1) {
	bottom = (GLdouble)arg1;
	setModified();
}


void GEMglOrtho :: topMess (double arg1) {
	top = (GLdouble)arg1;
	setModified();
}


void GEMglOrtho :: zNearMess (double arg1) {
	zNear = (GLdouble)arg1;
	setModified();
}


void GEMglOrtho :: zFarMess (double arg1) {
	zFar = (GLdouble)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglOrtho :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglOrtho,gensym("glOrtho"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglOrtho::leftMessCallback, gensym("left"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglOrtho::rightMessCallback, gensym("right"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglOrtho::bottomMessCallback, gensym("bottom"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglOrtho::topMessCallback, gensym("top"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglOrtho::zNearMessCallback, gensym("zNear"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglOrtho::zFarMessCallback, gensym("zFar"), A_NULL);
}


void GEMglOrtho :: leftMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->leftMess ( (t_int)    arg0);
}
void GEMglOrtho :: rightMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->rightMess ( (t_int)    arg0);
}
void GEMglOrtho :: bottomMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->bottomMess ( (t_int)    arg0);
}
void GEMglOrtho :: topMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->topMess ( (t_int)    arg0);
}
void GEMglOrtho :: zNearMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->zNearMess ( (t_int)    arg0);
}
void GEMglOrtho :: zFarMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->zFarMess ( (t_int)    arg0);
}
