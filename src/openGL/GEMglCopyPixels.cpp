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

#include "GEMglCopyPixels.h"

CPPEXTERN_NEW_WITH_GIMME(GEMglCopyPixels)

/////////////////////////////////////////////////////////
//
// GEMglCopyPixels
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglCopyPixels :: GEMglCopyPixels (int argc, t_atom *argv)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("x"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("y"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("width"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("height"));
	m_inlet[4] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("type"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglCopyPixels :: ~GEMglCopyPixels(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
inlet_free(m_inlet[4]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglCopyPixels :: render(GemState *state)
{ glCopyPixels(x, y, width, height, type); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglCopyPixels :: xMess (int arg1) {
	x = (GLint)arg1;
	setModified();
}


void GEMglCopyPixels :: yMess (int arg1) {
	y = (GLint)arg1;
	setModified();
}


void GEMglCopyPixels :: widthMess (int arg1) {
	width = (GLsizei)arg1;
	setModified();
}


void GEMglCopyPixels :: heightMess (int arg1) {
	height = (GLsizei)arg1;
	setModified();
}


void GEMglCopyPixels :: typeMess (int arg1) {
	type = (GLenum)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglCopyPixels :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglCopyPixels,gensym("glCopyPixels"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglCopyPixels::xMessCallback, gensym("x"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglCopyPixels::yMessCallback, gensym("y"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglCopyPixels::widthMessCallback, gensym("width"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglCopyPixels::heightMessCallback, gensym("height"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglCopyPixels::typeMessCallback, gensym("type"), A_NULL);
}


void GEMglCopyPixels :: xMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->xMess ( (t_int)    arg0);
}
void GEMglCopyPixels :: yMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->yMess ( (t_int)    arg0);
}
void GEMglCopyPixels :: widthMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->widthMess ( (t_int)    arg0);
}
void GEMglCopyPixels :: heightMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->heightMess ( (t_int)    arg0);
}
void GEMglCopyPixels :: typeMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->typeMess (getGLdefine(arg0->s_name));
}
