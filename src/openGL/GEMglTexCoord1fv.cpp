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

#include "GEMglTexCoord1fv.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglTexCoord1fv, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglTexCoord1fv
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglTexCoord1fv :: GEMglTexCoord1fv(t_floatarg arg1=0)
{
	vMess(arg1);
	m_inlet[0] = inlet_new (this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("v"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglTexCoord1fv :: ~GEMglTexCoord1fv(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglTexCoord1fv :: render(GemState *state)
{ glTexCoord1fv(m_v); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglTexCoord1fv :: vMess (t_float arg1) {
	m_v[0]=(GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglTexCoord1fv :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglTexCoord1fv,gensym("glTexCoord1fv"),A_FLOAT, A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglTexCoord1fv::vMessCallback, gensym("v"), A_FLOAT, A_NULL);
}


void GEMglTexCoord1fv :: vMessCallback (void* data, t_floatarg f1) {
	GetMyClass(data)->vMess (f1);
}
