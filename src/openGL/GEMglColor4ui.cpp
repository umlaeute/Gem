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

#include "GEMglColor4ui.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS (GEMglColor4ui , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglColor4ui
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglColor4ui :: GEMglColor4ui(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0, t_floatarg arg4=0) :
             		red((GLuint)arg1),
		green((GLuint)arg2),
		blue((GLuint)arg3),
		alpha((GLuint)arg4)
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
GEMglColor4ui :: ~GEMglColor4ui(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglColor4ui :: render(GemState *state)
{ glColor4ui(red, green, blue, alpha); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglColor4ui :: redMess (int arg1) {
	red = (GLuint)arg1;
	setModified();
}


void GEMglColor4ui :: greenMess (int arg1) {
	green = (GLuint)arg1;
	setModified();
}


void GEMglColor4ui :: blueMess (int arg1) {
	blue = (GLuint)arg1;
	setModified();
}


void GEMglColor4ui :: alphaMess (int arg1) {
	alpha = (GLuint)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglColor4ui :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglColor4ui,gensym("glColor4ui"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglColor4ui::redMessCallback, gensym("red"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglColor4ui::greenMessCallback, gensym("green"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglColor4ui::blueMessCallback, gensym("blue"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglColor4ui::alphaMessCallback, gensym("alpha"), A_NULL);
}


void GEMglColor4ui :: redMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->redMess ( (t_int)    arg0);
}
void GEMglColor4ui :: greenMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->greenMess ( (t_int)    arg0);
}
void GEMglColor4ui :: blueMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->blueMess ( (t_int)    arg0);
}
void GEMglColor4ui :: alphaMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->alphaMess ( (t_int)    arg0);
}
