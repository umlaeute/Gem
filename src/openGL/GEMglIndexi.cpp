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

#include "GEMglIndexi.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglIndexi , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglIndexi
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglIndexi :: GEMglIndexi(t_floatarg arg1=0) :
             		c((GLint)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("c"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglIndexi :: ~GEMglIndexi(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglIndexi :: render(GemState *state)
{ glIndexi(c); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglIndexi :: cMess (int arg1) {
	c = (GLint)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglIndexi :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglIndexi,gensym("glIndexi"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglIndexi::cMessCallback, gensym("c"), A_NULL);
}


void GEMglIndexi :: cMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->cMess ( (t_int)    arg0);
}
