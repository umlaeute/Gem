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

#include "GEMglNormal3b.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglNormal3b , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglNormal3b
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglNormal3b :: GEMglNormal3b(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
             		nx((GLbyte)arg1),
		ny((GLbyte)arg2),
		nz((GLbyte)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("nx"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ny"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("nz"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglNormal3b :: ~GEMglNormal3b(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglNormal3b :: render(GemState *state)
{ glNormal3b(nx, ny, nz); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglNormal3b :: nxMess (int arg1) {
	nx = (GLbyte)arg1;
	setModified();
}


void GEMglNormal3b :: nyMess (int arg1) {
	ny = (GLbyte)arg1;
	setModified();
}


void GEMglNormal3b :: nzMess (int arg1) {
	nz = (GLbyte)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglNormal3b :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglNormal3b,gensym("glNormal3b"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglNormal3b::nxMessCallback, gensym("nx"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglNormal3b::nyMessCallback, gensym("ny"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglNormal3b::nzMessCallback, gensym("nz"), A_NULL);
}


void GEMglNormal3b :: nxMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->nxMess ( (t_int)    arg0);
}
void GEMglNormal3b :: nyMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->nyMess ( (t_int)    arg0);
}
void GEMglNormal3b :: nzMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->nzMess ( (t_int)    arg0);
}
