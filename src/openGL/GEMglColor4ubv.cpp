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

#include "GEMglColor4ubv.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS (GEMglColor4ubv , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglColor4ubv
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglColor4ubv :: GEMglColor4ubv(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0, t_floatarg arg4=0)
{
  vMess(arg1, arg2, arg3, arg4);
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglColor4ubv :: ~GEMglColor4ubv(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglColor4ubv :: render(GemState *state)
{ glColor4ubv(m_v); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglColor4ubv :: vMess (t_float arg1, t_float arg2, t_float arg3 ,t_float arg4) {
	m_v[0] = (GLubyte)arg1;
        m_v[1] = (GLubyte)arg2;
        m_v[2] = (GLubyte)arg3;
        m_v[3] = (GLubyte)arg4;

	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglColor4ubv :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglColor4ubv,gensym("glColor4ubv"),A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglColor4ubv::vMessCallback, gensym("v"),A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
}


void GEMglColor4ubv :: vMessCallback (   void* data, t_floatarg f1, t_floatarg f2, t_floatarg f3, t_floatarg f4) {
	GetMyClass(data)->vMess (f1, f2, f3, f4);
}
