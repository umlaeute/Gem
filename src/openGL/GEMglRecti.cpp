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

#include "GEMglRecti.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS (GEMglRecti , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglRecti
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglRecti :: GEMglRecti(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0, t_floatarg arg4=0) :
             		x1((GLint)arg1),
		y1((GLint)arg2),
		x2((GLint)arg3),
		y2((GLint)arg4)
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
GEMglRecti :: ~GEMglRecti(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglRecti :: render(GemState *state)
{ glRecti(x1, y1, x2, y2); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglRecti :: x1Mess (int arg1) {
	x1 = (GLint)arg1;
	setModified();
}


void GEMglRecti :: y1Mess (int arg1) {
	y1 = (GLint)arg1;
	setModified();
}


void GEMglRecti :: x2Mess (int arg1) {
	x2 = (GLint)arg1;
	setModified();
}


void GEMglRecti :: y2Mess (int arg1) {
	y2 = (GLint)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglRecti :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglRecti,gensym("glRecti"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglRecti::x1MessCallback, gensym("x1"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRecti::y1MessCallback, gensym("y1"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRecti::x2MessCallback, gensym("x2"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRecti::y2MessCallback, gensym("y2"), A_NULL);
}


void GEMglRecti :: x1MessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->x1Mess ( (t_int)    arg0);
}
void GEMglRecti :: y1MessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->y1Mess ( (t_int)    arg0);
}
void GEMglRecti :: x2MessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->x2Mess ( (t_int)    arg0);
}
void GEMglRecti :: y2MessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->y2Mess ( (t_int)    arg0);
}
