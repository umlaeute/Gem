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

#include "GEMglVertex2dv.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglVertex2dv, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglVertex2dv
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglVertex2dv :: GEMglVertex2dv (t_floatarg arg1, t_floatarg arg2)
{
	vMess(arg1, arg2);
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglVertex2dv :: ~GEMglVertex2dv(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglVertex2dv :: render(GemState *state)
{ glVertex2dv(m_v); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglVertex2dv :: vMess (t_float arg1, t_float arg2) {
	m_v[0] = (GLdouble)arg1;
	m_v[1] = (GLdouble)arg2;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglVertex2dv :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglVertex2dv,gensym("glVertex2dv"),A_FLOAT, A_FLOAT, A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglVertex2dv::vMessCallback, gensym("v"), A_FLOAT, A_FLOAT, A_NULL);
}


void GEMglVertex2dv :: vMessCallback (void *data, t_floatarg f1, t_floatarg f2){
	GetMyClass(data)->vMess (f1, f2);
}
