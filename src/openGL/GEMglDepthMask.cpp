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

#include "GEMglDepthMask.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglDepthMask , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglDepthMask
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglDepthMask :: GEMglDepthMask(t_floatarg arg1=0) :
             		flag((GLboolean)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("flag"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglDepthMask :: ~GEMglDepthMask(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglDepthMask :: render(GemState *state)
{ glDepthMask(flag); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglDepthMask :: flagMess (int arg1) {
	flag = (GLboolean)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglDepthMask :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglDepthMask,gensym("glDepthMask"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglDepthMask::flagMessCallback, gensym("flag"), A_NULL);
}


void GEMglDepthMask :: flagMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->flagMess ( (t_int)    arg0);
}
