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

#include "GEMglRasterPos4sv.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS (GEMglRasterPos4sv, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglRasterPos4sv
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglRasterPos4sv :: GEMglRasterPos4sv(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0, t_floatarg arg4=0)
{
	vMess (arg1, arg2, arg3, arg4);
	m_inlet[0] = inlet_new (this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglRasterPos4sv :: ~GEMglRasterPos4sv(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglRasterPos4sv :: render(GemState *state)
{ glRasterPos4sv(m_v); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglRasterPos4sv :: vMess (t_float arg1, t_float arg2, t_float arg3, t_float arg4) {
	m_v[0]=(GLshort)arg1;
	m_v[1]=(GLshort)arg2;
	m_v[2]=(GLshort)arg3;
	m_v[3]=(GLshort)arg4;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglRasterPos4sv :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglRasterPos4sv,gensym("glRasterPos4sv"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglRasterPos4sv::vMessCallback, gensym("v"),  A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
}


void GEMglRasterPos4sv :: vMessCallback (void* data, t_floatarg f1, t_floatarg f2, t_floatarg f3, t_floatarg f4) {
	GetMyClass(data)->vMess (f1, f2, f3, f4);
}
