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

#include "GEMglNormal3i.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglNormal3i , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglNormal3i
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglNormal3i :: GEMglNormal3i(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
             		nx((GLint)arg1),
		ny((GLint)arg2),
		nz((GLint)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("nx"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ny"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("nz"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglNormal3i :: ~GEMglNormal3i(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglNormal3i :: render(GemState *state)
{ glNormal3i(nx, ny, nz); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglNormal3i :: nxMess (int arg1) {
	nx = (GLint)arg1;
	setModified();
}


void GEMglNormal3i :: nyMess (int arg1) {
	ny = (GLint)arg1;
	setModified();
}


void GEMglNormal3i :: nzMess (int arg1) {
	nz = (GLint)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglNormal3i :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglNormal3i,gensym("glNormal3i"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglNormal3i::nxMessCallback, gensym("nx"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglNormal3i::nyMessCallback, gensym("ny"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglNormal3i::nzMessCallback, gensym("nz"), A_NULL);
}


void GEMglNormal3i :: nxMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->nxMess ( (t_int)    arg0);
}
void GEMglNormal3i :: nyMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->nyMess ( (t_int)    arg0);
}
void GEMglNormal3i :: nzMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->nzMess ( (t_int)    arg0);
}
