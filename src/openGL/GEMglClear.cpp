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

#include "GEMglClear.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglClear, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglClear
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglClear :: GEMglClear(t_floatarg arg1=0) :
  mask((GLbitfield)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mask"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglClear :: ~GEMglClear(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglClear :: render(GemState *state)
{ glClear(mask); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglClear :: maskMess (t_float arg1) {
  mask = (GLbitfield)arg1;
  setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglClear :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglClear,gensym("glClear"), A_FLOAT, A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglClear::maskMessCallback, gensym("mask"), A_FLOAT, A_NULL);
}


void GEMglClear :: maskMessCallback (void* data, t_floatarg f0) {
	GetMyClass(data)->maskMess(f0);
}
