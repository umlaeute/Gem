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

#include "GEMglViewport.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS (GEMglViewport , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglViewport :: GEMglViewport(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0, t_floatarg arg4=0) :
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
GEMglViewport :: ~GEMglViewport(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglViewport :: render(GemState *state)
{ glViewport(x, y, width, height); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglViewport :: xMess (int arg1) {
	x = (GLint)arg1;
	setModified();
}


void GEMglViewport :: yMess (int arg1) {
	y = (GLint)arg1;
	setModified();
}


void GEMglViewport :: widthMess (int arg1) {
	width = (GLsizei)arg1;
	setModified();
}


void GEMglViewport :: heightMess (int arg1) {
	height = (GLsizei)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglViewport :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglViewport,gensym("glViewport"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglViewport::xMessCallback, gensym("x"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglViewport::yMessCallback, gensym("y"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglViewport::widthMessCallback, gensym("width"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglViewport::heightMessCallback, gensym("height"), A_NULL);
}


void GEMglViewport :: xMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->xMess ( (t_int)    arg0);
}
void GEMglViewport :: yMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->yMess ( (t_int)    arg0);
}
void GEMglViewport :: widthMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->widthMess ( (t_int)    arg0);
}
void GEMglViewport :: heightMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->heightMess ( (t_int)    arg0);
}
