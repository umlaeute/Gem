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

#include "GEMglRotatef.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS (GEMglRotatef , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglRotatef
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglRotatef :: GEMglRotatef(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0, t_floatarg arg4=0) :
             		angle((GLfloat)arg1),
		x((GLfloat)arg2),
		y((GLfloat)arg3),
		z((GLfloat)arg4)
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
GEMglRotatef :: ~GEMglRotatef(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglRotatef :: render(GemState *state)
{ glRotatef(angle, x, y, z); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglRotatef :: angleMess (t_float arg1) {
	angle = (GLfloat)arg1;
	setModified();
}


void GEMglRotatef :: xMess (t_float arg1) {
	x = (GLfloat)arg1;
	setModified();
}


void GEMglRotatef :: yMess (t_float arg1) {
	y = (GLfloat)arg1;
	setModified();
}


void GEMglRotatef :: zMess (t_float arg1) {
	z = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglRotatef :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglRotatef,gensym("glRotatef"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglRotatef::angleMessCallback, gensym("angle"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRotatef::xMessCallback, gensym("x"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRotatef::yMessCallback, gensym("y"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRotatef::zMessCallback, gensym("z"), A_NULL);
}


void GEMglRotatef :: angleMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->angleMess ( (t_int)    arg0);
}
void GEMglRotatef :: xMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->xMess ( (t_int)    arg0);
}
void GEMglRotatef :: yMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->yMess ( (t_int)    arg0);
}
void GEMglRotatef :: zMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->zMess ( (t_int)    arg0);
}
