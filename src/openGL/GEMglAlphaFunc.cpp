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

#include "GEMglAlphaFunc.h"

CPPEXTERN_NEW_WITH_GIMME (GEMglAlphaFunc)

/////////////////////////////////////////////////////////
//
// GEMglAlphaFunc
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglAlphaFunc :: GEMglAlphaFunc(int argc, t_atom*argv)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("func"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ref"));
	funcMess(argc, argv);
	if (argc>1)refMess(atom_getfloat(argv+1));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglAlphaFunc :: ~GEMglAlphaFunc(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglAlphaFunc :: render(GemState *state)
{ glAlphaFunc(func, ref); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglAlphaFunc :: funcMess (int argc, t_atom *argv) {
  if (argc>0 && argv!= 0){
    func = (GLenum)(argv->a_type == A_SYMBOL)?getGLdefine(argv->a_w.w_symbol->s_name):atom_getint(argv);
    setModified();
  }
}


void GEMglAlphaFunc :: refMess (t_float arg1) {
	ref = (GLclampf)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglAlphaFunc :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglAlphaFunc,gensym("glAlphaFunc"),A_GIMME,A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglAlphaFunc::funcMessCallback, gensym("func"), A_GIMME, A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglAlphaFunc::refMessCallback, gensym("ref"), A_DEFFLOAT, A_NULL);
}


void GEMglAlphaFunc :: funcMessCallback (   void* data, t_symbol* s, int argc, t_atom *argv) {
	GetMyClass(data)->funcMess (argc, argv);
}
void GEMglAlphaFunc :: refMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->refMess (arg0);
}
