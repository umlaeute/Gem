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

#include "GEMglRasterPos4i.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS (GEMglRasterPos4i , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglRasterPos4i
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglRasterPos4i :: GEMglRasterPos4i(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0, t_floatarg arg4=0) :
             		x((GLint)arg1),
		y((GLint)arg2),
		z((GLint)arg3),
		w((GLint)arg4)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("z"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("w"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglRasterPos4i :: ~GEMglRasterPos4i(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglRasterPos4i :: render(GemState *state)
{ glRasterPos4i(x, y, z, w); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglRasterPos4i :: xMess (int arg1) {
	x = (GLint)arg1;
	setModified();
}


void GEMglRasterPos4i :: yMess (int arg1) {
	y = (GLint)arg1;
	setModified();
}


void GEMglRasterPos4i :: zMess (int arg1) {
	z = (GLint)arg1;
	setModified();
}


void GEMglRasterPos4i :: wMess (int arg1) {
	w = (GLint)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglRasterPos4i :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglRasterPos4i,gensym("glRasterPos4i"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglRasterPos4i::xMessCallback, gensym("x"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRasterPos4i::yMessCallback, gensym("y"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRasterPos4i::zMessCallback, gensym("z"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRasterPos4i::wMessCallback, gensym("w"), A_NULL);
}


void GEMglRasterPos4i :: xMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->xMess ( (t_int)    arg0);
}
void GEMglRasterPos4i :: yMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->yMess ( (t_int)    arg0);
}
void GEMglRasterPos4i :: zMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->zMess ( (t_int)    arg0);
}
void GEMglRasterPos4i :: wMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->wMess ( (t_int)    arg0);
}
