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

#include "GEMglTexCoord2f.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglTexCoord2f , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglTexCoord2f
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglTexCoord2f :: GEMglTexCoord2f(t_floatarg arg1=0, t_floatarg arg2=0) :
             		s((GLfloat)arg1),
		t((GLfloat)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("s"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("t"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglTexCoord2f :: ~GEMglTexCoord2f(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglTexCoord2f :: render(GemState *state)
{ glTexCoord2f(s, t); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglTexCoord2f :: sMess (t_float arg1) {
	s = (GLfloat)arg1;
	setModified();
}


void GEMglTexCoord2f :: tMess (t_float arg1) {
	t = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglTexCoord2f :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglTexCoord2f,gensym("glTexCoord2f"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglTexCoord2f::sMessCallback, gensym("s"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglTexCoord2f::tMessCallback, gensym("t"), A_NULL);
}


void GEMglTexCoord2f :: sMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->sMess ( (t_int)    arg0);
}
void GEMglTexCoord2f :: tMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->tMess ( (t_int)    arg0);
}
