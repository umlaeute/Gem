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

#include "GEMglPushAttrib.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglPushAttrib , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglPushAttrib
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglPushAttrib :: GEMglPushAttrib(t_floatarg arg1=0) :
             		mask((GLbitfield)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mask"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglPushAttrib :: ~GEMglPushAttrib(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglPushAttrib :: render(GemState *state)
{ glPushAttrib(mask); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglPushAttrib :: maskMess (t_float arg1) {
	mask = (GLbitfield)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglPushAttrib :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglPushAttrib,gensym("glPushAttrib"), A_FLOAT, A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglPushAttrib::maskMessCallback, gensym("mask"), A_FLOAT, A_NULL);
}


void GEMglPushAttrib :: maskMessCallback (   void* data, t_floatarg f0) {
	GetMyClass(data)->maskMess (f0);
}
