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

#include "GEMglBitmap.h"

CPPEXTERN_NEW_WITH_GIMME(GEMglBitmap)

/////////////////////////////////////////////////////////
//
// GEMglBitmap
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglBitmap :: GEMglBitmap (int argc, t_atom *argv)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("width"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("height"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("xorig"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("yorig"));
	m_inlet[4] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("xmove"));
	m_inlet[5] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ymove"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglBitmap :: ~GEMglBitmap(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
inlet_free(m_inlet[4]);
inlet_free(m_inlet[5]);
inlet_free(m_inlet[6]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglBitmap :: render(GemState *state)
{ glBitmap(width, height, xorig, yorig, xmove, ymove, bitmap); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglBitmap :: widthMess (int arg1) {
	width = (GLsizei)arg1;
	setModified();
}


void GEMglBitmap :: heightMess (int arg1) {
	height = (GLsizei)arg1;
	setModified();
}


void GEMglBitmap :: xorigMess (t_float arg1) {
	xorig = (GLfloat)arg1;
	setModified();
}


void GEMglBitmap :: yorigMess (t_float arg1) {
	yorig = (GLfloat)arg1;
	setModified();
}


void GEMglBitmap :: xmoveMess (t_float arg1) {
	xmove = (GLfloat)arg1;
	setModified();
}


void GEMglBitmap :: ymoveMess (t_float arg1) {
	ymove = (GLfloat)arg1;
	setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglBitmap :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglBitmap,gensym("glBitmap"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglBitmap::widthMessCallback, gensym("width"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglBitmap::heightMessCallback, gensym("height"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglBitmap::xorigMessCallback, gensym("xorig"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglBitmap::yorigMessCallback, gensym("yorig"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglBitmap::xmoveMessCallback, gensym("xmove"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglBitmap::ymoveMessCallback, gensym("ymove"), A_NULL);
}


void GEMglBitmap :: widthMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->widthMess ( (t_int)    arg0);
}
void GEMglBitmap :: heightMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->heightMess ( (t_int)    arg0);
}
void GEMglBitmap :: xorigMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->xorigMess ( (t_int)    arg0);
}
void GEMglBitmap :: yorigMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->yorigMess ( (t_int)    arg0);
}
void GEMglBitmap :: xmoveMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->xmoveMess ( (t_int)    arg0);
}
void GEMglBitmap :: ymoveMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->ymoveMess ( (t_int)    arg0);
}
