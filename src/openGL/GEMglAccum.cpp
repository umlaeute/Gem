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

#include "GEMglAccum.h"

CPPEXTERN_NEW_WITH_GIMME (GEMglAccum)

/////////////////////////////////////////////////////////
//
// GEMglAccum
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglAccum :: GEMglAccum(int argc, t_atom *argv)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("op"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("value"));
	opMess(argc, argv);
	if (argc>1)valueMess(atom_getfloat(argv+1));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglAccum :: ~GEMglAccum(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglAccum :: render(GemState *state)
{ glAccum(op, value); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglAccum :: opMess (int argc, t_atom *argv) {
  if (argc>0 && argv!= 0){
    if (argv->a_type == A_SYMBOL)
      op = (GLenum)getGLdefine(argv->a_w.w_symbol->s_name);
    else op = atom_getint(argv);
    setModified();
  }
}


void GEMglAccum :: valueMess (t_float arg1) {
	value = (GLfloat)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglAccum :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglAccum,gensym("glAccum"), A_GIMME, A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglAccum::opMessCallback, gensym("op"), A_GIMME, A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglAccum::valueMessCallback, gensym("value"), A_DEFFLOAT, A_NULL);
}


void GEMglAccum :: opMessCallback (   void* data, t_symbol *s, int argc, t_atom *argv) {
	GetMyClass(data)->opMess (argc, argv);
}
void GEMglAccum :: valueMessCallback (   void* data, t_float arg0) {
	GetMyClass(data)->valueMess (arg0);
}
