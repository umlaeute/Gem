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

#include "GEMglDrawBuffer.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglDrawBuffer , t_symbol*, A_DEFSYMBOL)

/////////////////////////////////////////////////////////
//
// GEMglDrawBuffer
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglDrawBuffer :: GEMglDrawBuffer(t_symbol* arg1=0)
{
  if (arg1)mode=(GLenum)getGLdefine(arg1->s_name);
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mode"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglDrawBuffer :: ~GEMglDrawBuffer(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglDrawBuffer :: render(GemState *state)
{ glDrawBuffer(mode); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglDrawBuffer :: modeMess (int arg1) {
	mode = (GLenum)arg1;
	setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglDrawBuffer :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglDrawBuffer,gensym("glDrawBuffer"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglDrawBuffer::modeMessCallback, gensym("mode"), A_NULL);
}


void GEMglDrawBuffer :: modeMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->modeMess (getGLdefine(arg0->s_name));
}
