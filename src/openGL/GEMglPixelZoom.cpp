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

#include "GEMglPixelZoom.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglPixelZoom , t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglPixelZoom
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglPixelZoom :: GEMglPixelZoom(t_floatarg arg1=0, t_floatarg arg2=0) :
             		xfactor((GLfloat)arg1),
		yfactor((GLfloat)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("xfactor"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("yfactor"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglPixelZoom :: ~GEMglPixelZoom(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglPixelZoom :: render(GemState *state)
{ glPixelZoom(xfactor, yfactor); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglPixelZoom :: xfactorMess (t_float arg1) {
	xfactor = (GLfloat)arg1;
	setModified();
}


void GEMglPixelZoom :: yfactorMess (t_float arg1) {
	yfactor = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglPixelZoom :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglPixelZoom,gensym("glPixelZoom"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglPixelZoom::xfactorMessCallback, gensym("xfactor"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglPixelZoom::yfactorMessCallback, gensym("yfactor"), A_NULL);
}


void GEMglPixelZoom :: xfactorMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->xfactorMess ( (t_int)    arg0);
}
void GEMglPixelZoom :: yfactorMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->yfactorMess ( (t_int)    arg0);
}
