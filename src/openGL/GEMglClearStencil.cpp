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

#include "GEMglClearStencil.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglClearStencil , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglClearStencil
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglClearStencil :: GEMglClearStencil(t_floatarg arg1=0) :
             		s((GLint)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("s"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglClearStencil :: ~GEMglClearStencil(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglClearStencil :: render(GemState *state)
{ glClearStencil(s); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglClearStencil :: sMess (int arg1) {
	s = (GLint)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglClearStencil :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglClearStencil,gensym("glClearStencil"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglClearStencil::sMessCallback, gensym("s"), A_NULL);
}


void GEMglClearStencil :: sMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->sMess ( (t_int)    arg0);
}
