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

#include "GEMglRectd.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS (GEMglRectd , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglRectd
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglRectd :: GEMglRectd(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0, t_floatarg arg4=0) :
             		x1((GLdouble)arg1),
		y1((GLdouble)arg2),
		x2((GLdouble)arg3),
		y2((GLdouble)arg4)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x1"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y1"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x2"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y2"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglRectd :: ~GEMglRectd(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglRectd :: render(GemState *state)
{ glRectd(x1, y1, x2, y2); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglRectd :: x1Mess (double arg1) {
	x1 = (GLdouble)arg1;
	setModified();
}


void GEMglRectd :: y1Mess (double arg1) {
	y1 = (GLdouble)arg1;
	setModified();
}


void GEMglRectd :: x2Mess (double arg1) {
	x2 = (GLdouble)arg1;
	setModified();
}


void GEMglRectd :: y2Mess (double arg1) {
	y2 = (GLdouble)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglRectd :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglRectd,gensym("glRectd"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglRectd::x1MessCallback, gensym("x1"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRectd::y1MessCallback, gensym("y1"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRectd::x2MessCallback, gensym("x2"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRectd::y2MessCallback, gensym("y2"), A_NULL);
}


void GEMglRectd :: x1MessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->x1Mess ( (t_int)    arg0);
}
void GEMglRectd :: y1MessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->y1Mess ( (t_int)    arg0);
}
void GEMglRectd :: x2MessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->x2Mess ( (t_int)    arg0);
}
void GEMglRectd :: y2MessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->y2Mess ( (t_int)    arg0);
}
