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

#include "GEMglTexCoord4f.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS (GEMglTexCoord4f , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglTexCoord4f
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglTexCoord4f :: GEMglTexCoord4f(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0, t_floatarg arg4=0) :
             		s((GLfloat)arg1),
		t((GLfloat)arg2),
		r((GLfloat)arg3),
		q((GLfloat)arg4)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("s"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("t"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("r"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("q"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglTexCoord4f :: ~GEMglTexCoord4f(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglTexCoord4f :: render(GemState *state)
{ glTexCoord4f(s, t, r, q); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglTexCoord4f :: sMess (t_float arg1) {
	s = (GLfloat)arg1;
	setModified();
}


void GEMglTexCoord4f :: tMess (t_float arg1) {
	t = (GLfloat)arg1;
	setModified();
}


void GEMglTexCoord4f :: rMess (t_float arg1) {
	r = (GLfloat)arg1;
	setModified();
}


void GEMglTexCoord4f :: qMess (t_float arg1) {
	q = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglTexCoord4f :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglTexCoord4f,gensym("glTexCoord4f"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglTexCoord4f::sMessCallback, gensym("s"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexCoord4f::tMessCallback, gensym("t"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexCoord4f::rMessCallback, gensym("r"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexCoord4f::qMessCallback, gensym("q"), A_NULL);
}


void GEMglTexCoord4f :: sMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->sMess ( (t_int)    arg0);
}
void GEMglTexCoord4f :: tMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->tMess ( (t_int)    arg0);
}
void GEMglTexCoord4f :: rMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->rMess ( (t_int)    arg0);
}
void GEMglTexCoord4f :: qMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->qMess ( (t_int)    arg0);
}
