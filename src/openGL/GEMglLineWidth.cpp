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

#include "GEMglLineWidth.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglLineWidth , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglLineWidth
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglLineWidth :: GEMglLineWidth(t_floatarg arg1=0) :
             		width((GLfloat)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("width"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglLineWidth :: ~GEMglLineWidth(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglLineWidth :: render(GemState *state)
{ glLineWidth(width); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglLineWidth :: widthMess (t_float arg1) {
	width = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglLineWidth :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglLineWidth,gensym("glLineWidth"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglLineWidth::widthMessCallback, gensym("width"), A_NULL);
}


void GEMglLineWidth :: widthMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->widthMess ( (t_int)    arg0);
}
