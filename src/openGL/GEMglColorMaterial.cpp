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

#include "GEMglColorMaterial.h"

CPPEXTERN_NEW_WITH_TWO_ARGS (GEMglColorMaterial , t_symbol*, A_DEFSYMBOL, t_symbol*, A_DEFSYMBOL)

/////////////////////////////////////////////////////////
//
// GEMglColorMaterial
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglColorMaterial :: GEMglColorMaterial(t_symbol* arg1=0, t_symbol* arg2=0) :
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
GEMglColorMaterial :: ~GEMglColorMaterial(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglColorMaterial :: render(GemState *state)
{ glColorMaterial(face, mode); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglColorMaterial :: faceMess (int arg1) {
	face = (GLenum)arg1;
	setModified();
}


void GEMglColorMaterial :: modeMess (int arg1) {
	mode = (GLenum)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglColorMaterial :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglColorMaterial,gensym("glColorMaterial"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglColorMaterial::faceMessCallback, gensym("face"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglColorMaterial::modeMessCallback, gensym("mode"), A_NULL);
}


void GEMglColorMaterial :: faceMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->faceMess (getGLdefine(arg0->s_name));
}
void GEMglColorMaterial :: modeMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->modeMess (getGLdefine(arg0->s_name));
}
