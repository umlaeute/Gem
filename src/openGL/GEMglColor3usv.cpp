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

#include "GEMglColor3usv.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglColor3usv , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)


/////////////////////////////////////////////////////////
//
// GEMglColor3usv
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglColor3usv :: GEMglColor3usv(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0)
{
  vMess(arg1, arg2, arg3);
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglColor3usv :: ~GEMglColor3usv(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglColor3usv :: render(GemState *state)
{ glColor3usv(m_v); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglColor3usv :: vMess (t_float arg1, t_float arg2, t_float arg3) {
  /*
  m_v[0] = (GLushort)arg1;
  m_v[1] = (GLushort)arg2;
  m_v[2] = (GLushort)arg3;
  */
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglColor3usv :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglColor3usv,gensym("glColor3usv"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglColor3usv::vMessCallback, gensym("v"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
}


void GEMglColor3usv :: vMessCallback (void* data, t_floatarg f1, t_floatarg f2, t_floatarg f3){
  GetMyClass(data)->vMess (f1, f2, f3);
}
