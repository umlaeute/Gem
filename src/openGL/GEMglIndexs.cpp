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

#include "GEMglIndexs.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglIndexs , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglIndexs
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglIndexs :: GEMglIndexs(t_floatarg arg1=0) :
             		c((GLshort)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("c"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglIndexs :: ~GEMglIndexs(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglIndexs :: render(GemState *state)
{ glIndexs(c); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglIndexs :: cMess (int arg1) {
	c = (GLshort)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglIndexs :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglIndexs,gensym("glIndexs"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglIndexs::cMessCallback, gensym("c"), A_NULL);
}


void GEMglIndexs :: cMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->cMess ( (t_int)    arg0);
}
