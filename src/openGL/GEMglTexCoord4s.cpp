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

#include "GEMglTexCoord4s.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS (GEMglTexCoord4s , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglTexCoord4s
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglTexCoord4s :: GEMglTexCoord4s(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0, t_floatarg arg4=0) :
             		s((GLshort)arg1),
		t((GLshort)arg2),
		r((GLshort)arg3),
		q((GLshort)arg4)
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
GEMglTexCoord4s :: ~GEMglTexCoord4s(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglTexCoord4s :: render(GemState *state)
{ glTexCoord4s(s, t, r, q); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglTexCoord4s :: sMess (int arg1) {
	s = (GLshort)arg1;
	setModified();
}


void GEMglTexCoord4s :: tMess (int arg1) {
	t = (GLshort)arg1;
	setModified();
}


void GEMglTexCoord4s :: rMess (int arg1) {
	r = (GLshort)arg1;
	setModified();
}


void GEMglTexCoord4s :: qMess (int arg1) {
	q = (GLshort)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglTexCoord4s :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglTexCoord4s,gensym("glTexCoord4s"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglTexCoord4s::sMessCallback, gensym("s"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexCoord4s::tMessCallback, gensym("t"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexCoord4s::rMessCallback, gensym("r"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexCoord4s::qMessCallback, gensym("q"), A_NULL);
}


void GEMglTexCoord4s :: sMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->sMess ( (t_int)    arg0);
}
void GEMglTexCoord4s :: tMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->tMess ( (t_int)    arg0);
}
void GEMglTexCoord4s :: rMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->rMess ( (t_int)    arg0);
}
void GEMglTexCoord4s :: qMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->qMess ( (t_int)    arg0);
}
