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

#include "GEMglClearDepth.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglClearDepth , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglClearDepth
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglClearDepth :: GEMglClearDepth(t_floatarg arg1=0) :
             		depth((GLclampd)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("depth"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglClearDepth :: ~GEMglClearDepth(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglClearDepth :: render(GemState *state)
{ glClearDepth(depth); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglClearDepth :: depthMess (double arg1) {
	depth = (GLclampd)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglClearDepth :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglClearDepth,gensym("glClearDepth"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglClearDepth::depthMessCallback, gensym("depth"), A_NULL);
}


void GEMglClearDepth :: depthMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->depthMess ( (t_int)    arg0);
}
