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

#include "GEMglColor4f.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS (GEMglColor4f , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglColor4f
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglColor4f :: GEMglColor4f(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0, t_floatarg arg4=0) :
             		red((GLfloat)arg1),
		green((GLfloat)arg2),
		blue((GLfloat)arg3),
		alpha((GLfloat)arg4)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("red"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("green"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("blue"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("alpha"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglColor4f :: ~GEMglColor4f(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglColor4f :: render(GemState *state)
{ glColor4f(red, green, blue, alpha); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglColor4f :: redMess (t_float arg1) {
	red = (GLfloat)arg1;
	setModified();
}


void GEMglColor4f :: greenMess (t_float arg1) {
	green = (GLfloat)arg1;
	setModified();
}


void GEMglColor4f :: blueMess (t_float arg1) {
	blue = (GLfloat)arg1;
	setModified();
}


void GEMglColor4f :: alphaMess (t_float arg1) {
	alpha = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglColor4f :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglColor4f,gensym("glColor4f"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglColor4f::redMessCallback, gensym("red"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglColor4f::greenMessCallback, gensym("green"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglColor4f::blueMessCallback, gensym("blue"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglColor4f::alphaMessCallback, gensym("alpha"), A_NULL);
}


void GEMglColor4f :: redMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->redMess ( (t_int)    arg0);
}
void GEMglColor4f :: greenMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->greenMess ( (t_int)    arg0);
}
void GEMglColor4f :: blueMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->blueMess ( (t_int)    arg0);
}
void GEMglColor4f :: alphaMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->alphaMess ( (t_int)    arg0);
}
