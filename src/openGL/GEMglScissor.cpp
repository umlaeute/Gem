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

#include "GEMglScissor.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS (GEMglScissor , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglScissor
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglScissor :: GEMglScissor(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0, t_floatarg arg4=0) :
             		x((GLint)arg1),
		y((GLint)arg2),
		width((GLsizei)arg3),
		height((GLsizei)arg4)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("width"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("height"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglScissor :: ~GEMglScissor(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglScissor :: render(GemState *state)
{ glScissor(x, y, width, height); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglScissor :: xMess (int arg1) {
	x = (GLint)arg1;
	setModified();
}


void GEMglScissor :: yMess (int arg1) {
	y = (GLint)arg1;
	setModified();
}


void GEMglScissor :: widthMess (int arg1) {
	width = (GLsizei)arg1;
	setModified();
}


void GEMglScissor :: heightMess (int arg1) {
	height = (GLsizei)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglScissor :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglScissor,gensym("glScissor"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglScissor::xMessCallback, gensym("x"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglScissor::yMessCallback, gensym("y"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglScissor::widthMessCallback, gensym("width"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglScissor::heightMessCallback, gensym("height"), A_NULL);
}


void GEMglScissor :: xMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->xMess ( (t_int)    arg0);
}
void GEMglScissor :: yMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->yMess ( (t_int)    arg0);
}
void GEMglScissor :: widthMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->widthMess ( (t_int)    arg0);
}
void GEMglScissor :: heightMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->heightMess ( (t_int)    arg0);
}
