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

#include "GEMglStencilMask.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglStencilMask , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglStencilMask
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglStencilMask :: GEMglStencilMask(t_floatarg arg1=0) :
             		mask((GLuint)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mask"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglStencilMask :: ~GEMglStencilMask(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglStencilMask :: render(GemState *state)
{ glStencilMask(mask); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglStencilMask :: maskMess (int arg1) {
	mask = (GLuint)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglStencilMask :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglStencilMask,gensym("glStencilMask"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglStencilMask::maskMessCallback, gensym("mask"), A_NULL);
}


void GEMglStencilMask :: maskMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->maskMess ( (t_int)    arg0);
}
