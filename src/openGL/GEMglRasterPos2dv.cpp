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

#include "GEMglRasterPos2dv.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglRasterPos2dv, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglRasterPos2dv
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglRasterPos2dv :: GEMglRasterPos2dv(t_floatarg arg1=0, t_floatarg arg2=0)
{
	vMess(arg1, arg2);
	m_inlet[0] = inlet_new (this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglRasterPos2dv :: ~GEMglRasterPos2dv(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglRasterPos2dv :: render(GemState *state)
{ glRasterPos2dv(m_v); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglRasterPos2dv :: vMess (t_float arg1, t_float arg2) {
	m_v[0]=(GLdouble)arg1;
	m_v[1]=(GLdouble)arg2;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglRasterPos2dv :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglRasterPos2dv,gensym("glRasterPos2dv"),A_FLOAT, A_FLOAT, A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglRasterPos2dv::vMessCallback, gensym("v"), A_FLOAT, A_FLOAT, A_NULL);
}


void GEMglRasterPos2dv :: vMessCallback (void* data, t_floatarg f1, t_floatarg f2) {
	GetMyClass(data)->vMess (f1, f2);
}
