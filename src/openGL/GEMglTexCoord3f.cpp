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

#include "GEMglTexCoord3f.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglTexCoord3f , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglTexCoord3f
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglTexCoord3f :: GEMglTexCoord3f(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
             		s((GLfloat)arg1),
		t((GLfloat)arg2),
		r((GLfloat)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("s"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("t"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("r"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglTexCoord3f :: ~GEMglTexCoord3f(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglTexCoord3f :: render(GemState *state)
{ glTexCoord3f(s, t, r); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglTexCoord3f :: sMess (t_float arg1) {
	s = (GLfloat)arg1;
	setModified();
}


void GEMglTexCoord3f :: tMess (t_float arg1) {
	t = (GLfloat)arg1;
	setModified();
}


void GEMglTexCoord3f :: rMess (t_float arg1) {
	r = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglTexCoord3f :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglTexCoord3f,gensym("glTexCoord3f"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglTexCoord3f::sMessCallback, gensym("s"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexCoord3f::tMessCallback, gensym("t"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexCoord3f::rMessCallback, gensym("r"), A_NULL);
}


void GEMglTexCoord3f :: sMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->sMess ( (t_int)    arg0);
}
void GEMglTexCoord3f :: tMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->tMess ( (t_int)    arg0);
}
void GEMglTexCoord3f :: rMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->rMess ( (t_int)    arg0);
}
