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

#include "GEMglVertex4dv.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS (GEMglVertex4dv , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglVertex4dv
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglVertex4dv :: GEMglVertex4dv(t_floatarg arg1=0, t_floatarg arg2=0,
				 t_floatarg arg3=0, t_floatarg arg4=0)
{
  vMess(arg1, arg2, arg3, arg4);
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglVertex4dv :: ~GEMglVertex4dv(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglVertex4dv :: render(GemState *state)
{ glVertex4dv(m_v); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglVertex4dv :: vMess (t_float arg1, t_float arg2, t_float arg3, t_float arg4) {
  m_v[0]=(GLdouble)arg1;
  m_v[1]=(GLdouble)arg2;
  m_v[2]=(GLdouble)arg3;
  m_v[3]=(GLdouble)arg4;
  setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglVertex4dv :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglVertex4dv,gensym("glVertex4dv"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglVertex4dv::vMessCallback, gensym("v"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
}


void GEMglVertex4dv :: vMessCallback (void* data, t_floatarg f1, t_floatarg f2,
				      t_floatarg f3, t_floatarg f4) {
  GetMyClass(data)->vMess(f1, f2, f3, f4);
}
