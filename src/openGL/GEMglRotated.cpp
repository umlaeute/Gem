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

#include "GEMglRotated.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS (GEMglRotated , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglRotated
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglRotated :: GEMglRotated(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0, t_floatarg arg4=0) :
             		angle((GLdouble)arg1),
		x((GLdouble)arg2),
		y((GLdouble)arg3),
		z((GLdouble)arg4)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("angle"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("z"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglRotated :: ~GEMglRotated(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglRotated :: render(GemState *state)
{ glRotated(angle, x, y, z); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglRotated :: angleMess (double arg1) {
	angle = (GLdouble)arg1;
	setModified();
}


void GEMglRotated :: xMess (double arg1) {
	x = (GLdouble)arg1;
	setModified();
}


void GEMglRotated :: yMess (double arg1) {
	y = (GLdouble)arg1;
	setModified();
}


void GEMglRotated :: zMess (double arg1) {
	z = (GLdouble)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglRotated :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglRotated,gensym("glRotated"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglRotated::angleMessCallback, gensym("angle"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRotated::xMessCallback, gensym("x"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRotated::yMessCallback, gensym("y"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRotated::zMessCallback, gensym("z"), A_NULL);
}


void GEMglRotated :: angleMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->angleMess ( (t_int)    arg0);
}
void GEMglRotated :: xMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->xMess ( (t_int)    arg0);
}
void GEMglRotated :: yMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->yMess ( (t_int)    arg0);
}
void GEMglRotated :: zMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->zMess ( (t_int)    arg0);
}
