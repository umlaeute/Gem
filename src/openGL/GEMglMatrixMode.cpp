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

#include "GEMglMatrixMode.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglMatrixMode , t_symbol*, A_DEFSYMBOL)

/////////////////////////////////////////////////////////
//
// GEMglMatrixMode
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglMatrixMode :: GEMglMatrixMode(t_symbol* arg1=0) :
             		mode((GLenum)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mode"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglMatrixMode :: ~GEMglMatrixMode(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglMatrixMode :: render(GemState *state)
{ glMatrixMode(mode); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglMatrixMode :: modeMess (int arg1) {
	mode = (GLenum)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglMatrixMode :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglMatrixMode,gensym("glMatrixMode"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglMatrixMode::modeMessCallback, gensym("mode"), A_NULL);
}


void GEMglMatrixMode :: modeMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->modeMess (getGLdefine(arg0->s_name));
}
