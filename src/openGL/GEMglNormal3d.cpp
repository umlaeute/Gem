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

#include "GEMglNormal3d.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglNormal3d , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglNormal3d
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglNormal3d :: GEMglNormal3d(t_floatarg arg1=0, t_floatarg arg2=0, t_floatarg arg3=0) :
             		nx((GLdouble)arg1),
		ny((GLdouble)arg2),
		nz((GLdouble)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("nx"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ny"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("nz"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglNormal3d :: ~GEMglNormal3d(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglNormal3d :: render(GemState *state)
{ glNormal3d(nx, ny, nz); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglNormal3d :: nxMess (double arg1) {
	nx = (GLdouble)arg1;
	setModified();
}


void GEMglNormal3d :: nyMess (double arg1) {
	ny = (GLdouble)arg1;
	setModified();
}


void GEMglNormal3d :: nzMess (double arg1) {
	nz = (GLdouble)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglNormal3d :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglNormal3d,gensym("glNormal3d"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglNormal3d::nxMessCallback, gensym("nx"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglNormal3d::nyMessCallback, gensym("ny"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglNormal3d::nzMessCallback, gensym("nz"), A_NULL);
}


void GEMglNormal3d :: nxMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->nxMess ( (t_int)    arg0);
}
void GEMglNormal3d :: nyMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->nyMess ( (t_int)    arg0);
}
void GEMglNormal3d :: nzMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->nzMess ( (t_int)    arg0);
}
