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

#include "GEMglFrontFace.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglFrontFace , t_symbol*, A_DEFSYMBOL)

/////////////////////////////////////////////////////////
//
// GEMglFrontFace
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglFrontFace :: GEMglFrontFace(t_symbol* arg1=0) :
             		mode((GLenum)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mode"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglFrontFace :: ~GEMglFrontFace(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglFrontFace :: render(GemState *state)
{ glFrontFace(mode); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglFrontFace :: modeMess (int arg1) {
	mode = (GLenum)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglFrontFace :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglFrontFace,gensym("glFrontFace"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglFrontFace::modeMessCallback, gensym("mode"), A_NULL);
}


void GEMglFrontFace :: modeMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->modeMess (getGLdefine(arg0->s_name));
}
