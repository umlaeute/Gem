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

#include "GEMglRects.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS (GEMglRects , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglRects
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglRects :: GEMglRects(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0, t_floatarg arg4=0) :
             		x1((GLshort)arg1),
		y1((GLshort)arg2),
		x2((GLshort)arg3),
		y2((GLshort)arg4)
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
GEMglRects :: ~GEMglRects(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglRects :: render(GemState *state)
{ glRects(x1, y1, x2, y2); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglRects :: x1Mess (int arg1) {
	x1 = (GLshort)arg1;
	setModified();
}


void GEMglRects :: y1Mess (int arg1) {
	y1 = (GLshort)arg1;
	setModified();
}


void GEMglRects :: x2Mess (int arg1) {
	x2 = (GLshort)arg1;
	setModified();
}


void GEMglRects :: y2Mess (int arg1) {
	y2 = (GLshort)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglRects :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglRects,gensym("glRects"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglRects::x1MessCallback, gensym("x1"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRects::y1MessCallback, gensym("y1"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRects::x2MessCallback, gensym("x2"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRects::y2MessCallback, gensym("y2"), A_NULL);
}


void GEMglRects :: x1MessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->x1Mess ( (t_int)    arg0);
}
void GEMglRects :: y1MessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->y1Mess ( (t_int)    arg0);
}
void GEMglRects :: x2MessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->x2Mess ( (t_int)    arg0);
}
void GEMglRects :: y2MessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->y2Mess ( (t_int)    arg0);
}
