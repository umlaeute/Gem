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

#include "GEMglIndexf.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglIndexf , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglIndexf
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglIndexf :: GEMglIndexf(t_floatarg arg1=0) :
             		c((GLfloat)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("c"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglIndexf :: ~GEMglIndexf(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglIndexf :: render(GemState *state)
{ glIndexf(c); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglIndexf :: cMess (t_float arg1) {
	c = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglIndexf :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglIndexf,gensym("glIndexf"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglIndexf::cMessCallback, gensym("c"), A_NULL);
}


void GEMglIndexf :: cMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->cMess ( (t_int)    arg0);
}
