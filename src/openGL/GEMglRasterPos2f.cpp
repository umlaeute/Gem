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

#include "GEMglRasterPos2f.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglRasterPos2f , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglRasterPos2f
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglRasterPos2f :: GEMglRasterPos2f(t_floatarg arg1=0, t_floatarg arg2=0) :
             		x((GLfloat)arg1),
		y((GLfloat)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglRasterPos2f :: ~GEMglRasterPos2f(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglRasterPos2f :: render(GemState *state)
{ glRasterPos2f(x, y); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglRasterPos2f :: xMess (t_float arg1) {
	x = (GLfloat)arg1;
	setModified();
}


void GEMglRasterPos2f :: yMess (t_float arg1) {
	y = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglRasterPos2f :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglRasterPos2f,gensym("glRasterPos2f"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglRasterPos2f::xMessCallback, gensym("x"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglRasterPos2f::yMessCallback, gensym("y"), A_NULL);
}


void GEMglRasterPos2f :: xMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->xMess ( (t_int)    arg0);
}
void GEMglRasterPos2f :: yMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->yMess ( (t_int)    arg0);
}
