////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//	zmoelnig@iem.kug.ac.at
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
//  this file has been generated...
////////////////////////////////////////////////////////

#include "GEMglBlendEquation.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglBlendEquation , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglBlendEquation :: GEMglBlendEquation	(t_floatarg arg0=0) :
  mode((GLenum)arg0)
{
#ifdef DONT_HAVE_GLBLENDEQUATION
  error("Gem has been compiled without glBlendEquation");
#endif /* DONT_HAVE_GLBLENDEQUATION */
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mode"));
}
////////////////////////////////////////////////////////
// Destructor
//
GEMglBlendEquation :: ~GEMglBlendEquation () {
  inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglBlendEquation :: render(GemState *state) {
#if defined GL_VERSION_1_2 && !defined DONT_HAVE_GLBLENDEQUATION
  glBlendEquation (mode);
#endif /* DONT_HAVE_GLBLENDEQUATION */
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglBlendEquation :: modeMess (t_float arg1) {	// FUN
  mode = (GLenum)arg1;
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglBlendEquation :: obj_setupCallback(t_class *classPtr) {
  class_addmethod(classPtr, (t_method)&GEMglBlendEquation::modeMessCallback,  	gensym("mode"), A_DEFFLOAT, A_NULL);
};

void GEMglBlendEquation :: modeMessCallback (void* data, t_floatarg arg0){
  GetMyClass(data)->modeMess ( (t_float)    arg0);
}
