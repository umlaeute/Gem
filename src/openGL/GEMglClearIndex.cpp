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

#include "GEMglClearIndex.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglClearIndex , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglClearIndex
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglClearIndex :: GEMglClearIndex(t_floatarg arg1=0) :
             		c((GLfloat)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("c"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglClearIndex :: ~GEMglClearIndex(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglClearIndex :: render(GemState *state)
{ glClearIndex(c); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglClearIndex :: cMess (t_float arg1) {
	c = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglClearIndex :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglClearIndex,gensym("glClearIndex"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglClearIndex::cMessCallback, gensym("c"), A_NULL);
}


void GEMglClearIndex :: cMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->cMess ( (t_int)    arg0);
}
