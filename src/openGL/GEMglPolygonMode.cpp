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

#include "GEMglPolygonMode.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglPolygonMode , t_symbol*, A_DEFSYMBOL, t_symbol*, A_DEFSYMBOL)

/////////////////////////////////////////////////////////
//
// GEMglPolygonMode
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglPolygonMode :: GEMglPolygonMode(t_symbol* arg1=0, t_symbol* arg2=0) :
             		face((GLenum)arg1),
		mode((GLenum)arg2)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("face"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mode"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglPolygonMode :: ~GEMglPolygonMode(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglPolygonMode :: render(GemState *state)
{ glPolygonMode(face, mode); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglPolygonMode :: faceMess (int arg1) {
	face = (GLenum)arg1;
	setModified();
}


void GEMglPolygonMode :: modeMess (int arg1) {
	mode = (GLenum)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglPolygonMode :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglPolygonMode,gensym("glPolygonMode"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglPolygonMode::faceMessCallback, gensym("face"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglPolygonMode::modeMessCallback, gensym("mode"), A_NULL);
}


void GEMglPolygonMode :: faceMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->faceMess (getGLdefine(arg0->s_name));
}
void GEMglPolygonMode :: modeMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->modeMess (getGLdefine(arg0->s_name));
}
