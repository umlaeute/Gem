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

#include "GEMglTexCoord1sv.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglTexCoord1sv, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglTexCoord1sv
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglTexCoord1sv :: GEMglTexCoord1sv(t_floatarg arg1=0)
{
	vMess(arg1);
	m_inlet[0] = inlet_new (this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglTexCoord1sv :: ~GEMglTexCoord1sv(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglTexCoord1sv :: render(GemState *state)
{ glTexCoord1sv(m_v); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglTexCoord1sv :: vMess (t_float arg1) {
	m_v[0]=(GLshort)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglTexCoord1sv :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglTexCoord1sv,gensym("glTexCoord1sv"),A_FLOAT, A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglTexCoord1sv::vMessCallback, gensym("v"), A_FLOAT, A_NULL);
}


void GEMglTexCoord1sv :: vMessCallback (void* data, t_floatarg f1) {
	GetMyClass(data)->vMess (f1);
}
