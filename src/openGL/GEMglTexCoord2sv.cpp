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

#include "GEMglTexCoord2sv.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglTexCoord2sv, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglTexCoord2sv
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglTexCoord2sv :: GEMglTexCoord2sv(t_floatarg arg1=0, t_floatarg arg2=0)
{
	vMess(arg1, arg2);
	m_inlet[0] = inlet_new (this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglTexCoord2sv :: ~GEMglTexCoord2sv(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglTexCoord2sv :: render(GemState *state)
{ glTexCoord2sv(m_v); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglTexCoord2sv :: vMess (t_float arg1, t_float arg2) {
	m_v[0]=(GLshort)arg1;
	m_v[1]=(GLshort)arg2;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglTexCoord2sv :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglTexCoord2sv,gensym("glTexCoord2sv"),A_FLOAT, A_FLOAT, A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglTexCoord2sv::vMessCallback, gensym("v"), A_FLOAT, A_FLOAT, A_NULL);
}


void GEMglTexCoord2sv :: vMessCallback (void* data, t_floatarg f1, t_floatarg f2) {
	GetMyClass(data)->vMess (f1, f2);
}
