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

#include "GEMglColor3ub.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglColor3ub , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglColor3ub
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglColor3ub :: GEMglColor3ub(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
             		red((GLubyte)arg1),
		green((GLubyte)arg2),
		blue((GLubyte)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("red"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("green"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("blue"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglColor3ub :: ~GEMglColor3ub(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglColor3ub :: render(GemState *state)
{ glColor3ub(red, green, blue); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglColor3ub :: redMess (int arg1) {
	red = (GLubyte)arg1;
	setModified();
}


void GEMglColor3ub :: greenMess (int arg1) {
	green = (GLubyte)arg1;
	setModified();
}


void GEMglColor3ub :: blueMess (int arg1) {
	blue = (GLubyte)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglColor3ub :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglColor3ub,gensym("glColor3ub"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglColor3ub::redMessCallback, gensym("red"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglColor3ub::greenMessCallback, gensym("green"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglColor3ub::blueMessCallback, gensym("blue"), A_NULL);
}


void GEMglColor3ub :: redMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->redMess ( (t_int)    arg0);
}
void GEMglColor3ub :: greenMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->greenMess ( (t_int)    arg0);
}
void GEMglColor3ub :: blueMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->blueMess ( (t_int)    arg0);
}
