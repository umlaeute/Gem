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

#include "GEMglPolygonOffset.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglPolygonOffset , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglPolygonOffset
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglPolygonOffset :: GEMglPolygonOffset(t_floatarg arg1=0, t_floatarg arg2=0) :
             		factor((GLfloat)arg1),
		units((GLfloat)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("factor"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("units"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglPolygonOffset :: ~GEMglPolygonOffset(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglPolygonOffset :: render(GemState *state)
{ glPolygonOffset(factor, units); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglPolygonOffset :: factorMess (t_float arg1) {
	factor = (GLfloat)arg1;
	setModified();
}


void GEMglPolygonOffset :: unitsMess (t_float arg1) {
	units = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglPolygonOffset :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglPolygonOffset,gensym("glPolygonOffset"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglPolygonOffset::factorMessCallback, gensym("factor"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglPolygonOffset::unitsMessCallback, gensym("units"), A_NULL);
}


void GEMglPolygonOffset :: factorMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->factorMess ( (t_int)    arg0);
}
void GEMglPolygonOffset :: unitsMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->unitsMess ( (t_int)    arg0);
}
