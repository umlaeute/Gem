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

#include "GEMglColor3b.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglColor3b , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglColor3b
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglColor3b :: GEMglColor3b(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
             		red((GLbyte)arg1),
		green((GLbyte)arg2),
		blue((GLbyte)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("red"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("green"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("blue"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglColor3b :: ~GEMglColor3b(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglColor3b :: render(GemState *state)
{ glColor3b(red, green, blue); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglColor3b :: redMess (int arg1) {
	red = (GLbyte)arg1;
	setModified();
}


void GEMglColor3b :: greenMess (int arg1) {
	green = (GLbyte)arg1;
	setModified();
}


void GEMglColor3b :: blueMess (int arg1) {
	blue = (GLbyte)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglColor3b :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglColor3b,gensym("glColor3b"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglColor3b::redMessCallback, gensym("red"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglColor3b::greenMessCallback, gensym("green"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglColor3b::blueMessCallback, gensym("blue"), A_NULL);
}


void GEMglColor3b :: redMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->redMess ( (t_int)    arg0);
}
void GEMglColor3b :: greenMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->greenMess ( (t_int)    arg0);
}
void GEMglColor3b :: blueMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->blueMess ( (t_int)    arg0);
}
