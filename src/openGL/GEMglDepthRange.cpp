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

#include "GEMglDepthRange.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglDepthRange , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglDepthRange
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglDepthRange :: GEMglDepthRange(t_floatarg arg1=0, t_floatarg arg2=0) :
             		near_val((GLclampd)arg1),
		far_val((GLclampd)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("near_val"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("far_val"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglDepthRange :: ~GEMglDepthRange(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglDepthRange :: render(GemState *state)
{ glDepthRange(near_val, far_val); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglDepthRange :: near_valMess (double arg1) {
	near_val = (GLclampd)arg1;
	setModified();
}


void GEMglDepthRange :: far_valMess (double arg1) {
	far_val = (GLclampd)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglDepthRange :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglDepthRange,gensym("glDepthRange"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglDepthRange::near_valMessCallback, gensym("near_val"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglDepthRange::far_valMessCallback, gensym("far_val"), A_NULL);
}


void GEMglDepthRange :: near_valMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->near_valMess ( (t_int)    arg0);
}
void GEMglDepthRange :: far_valMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->far_valMess ( (t_int)    arg0);
}
