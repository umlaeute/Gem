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

#include "GEMglColor4ub.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS (GEMglColor4ub , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglColor4ub
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglColor4ub :: GEMglColor4ub(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0, t_floatarg arg4=0) :
             		red((GLubyte)arg1),
		green((GLubyte)arg2),
		blue((GLubyte)arg3),
		alpha((GLubyte)arg4)
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
GEMglColor4ub :: ~GEMglColor4ub(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglColor4ub :: render(GemState *state)
{ glColor4ub(red, green, blue, alpha); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglColor4ub :: redMess (int arg1) {
	red = (GLubyte)arg1;
	setModified();
}


void GEMglColor4ub :: greenMess (int arg1) {
	green = (GLubyte)arg1;
	setModified();
}


void GEMglColor4ub :: blueMess (int arg1) {
	blue = (GLubyte)arg1;
	setModified();
}


void GEMglColor4ub :: alphaMess (int arg1) {
	alpha = (GLubyte)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglColor4ub :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglColor4ub,gensym("glColor4ub"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglColor4ub::redMessCallback, gensym("red"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglColor4ub::greenMessCallback, gensym("green"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglColor4ub::blueMessCallback, gensym("blue"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglColor4ub::alphaMessCallback, gensym("alpha"), A_NULL);
}


void GEMglColor4ub :: redMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->redMess ( (t_int)    arg0);
}
void GEMglColor4ub :: greenMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->greenMess ( (t_int)    arg0);
}
void GEMglColor4ub :: blueMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->blueMess ( (t_int)    arg0);
}
void GEMglColor4ub :: alphaMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->alphaMess ( (t_int)    arg0);
}
