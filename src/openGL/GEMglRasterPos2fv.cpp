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

#include "GEMglRasterPos2fv.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglRasterPos2fv, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglRasterPos2fv
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglRasterPos2fv :: GEMglRasterPos2fv(t_floatarg arg1=0, t_floatarg arg2=0)
{
	vMess(arg1, arg2);
	m_inlet[0] = inlet_new (this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglRasterPos2fv :: ~GEMglRasterPos2fv(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglRasterPos2fv :: render(GemState *state)
{ glRasterPos2fv(m_v); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglRasterPos2fv :: vMess (t_float arg1, t_float arg2) {
	m_v[0]=(GLfloat)arg1;
	m_v[1]=(GLfloat)arg2;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglRasterPos2fv :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglRasterPos2fv,gensym("glRasterPos2fv"),A_FLOAT, A_FLOAT, A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglRasterPos2fv::vMessCallback, gensym("v"), A_FLOAT, A_FLOAT, A_NULL);
}


void GEMglRasterPos2fv :: vMessCallback (void* data, t_floatarg f1, t_floatarg f2) {
	GetMyClass(data)->vMess (f1, f2);
}
