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

#include "GEMglPointSize.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglPointSize , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglPointSize
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglPointSize :: GEMglPointSize(t_floatarg arg1=0) :
             		size((GLfloat)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("size"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglPointSize :: ~GEMglPointSize(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglPointSize :: render(GemState *state)
{ glPointSize(size); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglPointSize :: sizeMess (t_float arg1) {
	size = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglPointSize :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglPointSize,gensym("glPointSize"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglPointSize::sizeMessCallback, gensym("size"), A_NULL);
}


void GEMglPointSize :: sizeMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->sizeMess ( (t_int)    arg0);
}
