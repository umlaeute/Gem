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

#include "GEMglIndexd.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglIndexd , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglIndexd
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglIndexd :: GEMglIndexd(t_floatarg arg1=0) :
             		c((GLdouble)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("c"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglIndexd :: ~GEMglIndexd(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglIndexd :: render(GemState *state)
{ glIndexd(c); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglIndexd :: cMess (double arg1) {
	c = (GLdouble)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglIndexd :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglIndexd,gensym("glIndexd"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglIndexd::cMessCallback, gensym("c"), A_NULL);
}


void GEMglIndexd :: cMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->cMess ( (t_int)    arg0);
}
