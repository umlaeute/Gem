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

#include "GEMglTexCoord3i.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglTexCoord3i , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglTexCoord3i
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglTexCoord3i :: GEMglTexCoord3i(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
             		s((GLint)arg1),
		t((GLint)arg2),
		r((GLint)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("s"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("t"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("r"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglTexCoord3i :: ~GEMglTexCoord3i(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglTexCoord3i :: render(GemState *state)
{ glTexCoord3i(s, t, r); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglTexCoord3i :: sMess (int arg1) {
	s = (GLint)arg1;
	setModified();
}


void GEMglTexCoord3i :: tMess (int arg1) {
	t = (GLint)arg1;
	setModified();
}


void GEMglTexCoord3i :: rMess (int arg1) {
	r = (GLint)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglTexCoord3i :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglTexCoord3i,gensym("glTexCoord3i"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglTexCoord3i::sMessCallback, gensym("s"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexCoord3i::tMessCallback, gensym("t"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexCoord3i::rMessCallback, gensym("r"), A_NULL);
}


void GEMglTexCoord3i :: sMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->sMess ( (t_int)    arg0);
}
void GEMglTexCoord3i :: tMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->tMess ( (t_int)    arg0);
}
void GEMglTexCoord3i :: rMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->rMess ( (t_int)    arg0);
}
