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
// checked
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GEMglBegin.h"

CPPEXTERN_NEW_WITH_GIMME (GEMglBegin)

/////////////////////////////////////////////////////////
//
// GEMglBegin
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglBegin :: GEMglBegin(int argc, t_atom *argv)
{
	post("argc=%d", argc);
	post("argv(typ)=%d", argv->a_type);
	modeMess(argc, argv);
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mode"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglBegin :: ~GEMglBegin(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglBegin :: render(GemState *state)
{ glBegin(mode); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////
void GEMglBegin :: modeMess (int argc, t_atom *argv) {
  if (argc>0 && argv!= 0){
    if (argv->a_type == A_SYMBOL)
      mode = (GLenum)getGLdefine(argv->a_w.w_symbol->s_name);
    else mode = atom_getint(argv);
    setModified();
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglBegin :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglBegin,gensym("glBegin"),A_GIMME,A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglBegin::modeMessCallback, gensym("mode"), A_GIMME, A_NULL);
}


void GEMglBegin :: modeMessCallback (   void* data, t_symbol*    arg0, int argc, t_atom *argv) {
    GetMyClass(data)->modeMess (argc, argv);
}
