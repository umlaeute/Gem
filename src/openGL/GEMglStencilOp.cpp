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

#include "GEMglStencilOp.h"

CPPEXTERN_NEW_WITH_THREE_ARGS (GEMglStencilOp , t_symbol*, A_DEFSYMBOL, t_symbol*, A_DEFSYMBOL, t_symbol*, A_DEFSYMBOL)

/////////////////////////////////////////////////////////
//
// GEMglStencilOp
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglStencilOp :: GEMglStencilOp(t_symbol* arg1=0, t_symbol* arg2=0, t_symbol* arg3=0) :
             		fail((GLenum)arg1),
		zfail((GLenum)arg2),
		zpass((GLenum)arg3)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("fail"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("zfail"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("zpass"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglStencilOp :: ~GEMglStencilOp(){
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglStencilOp :: render(GemState *state)
{ glStencilOp(fail, zfail, zpass); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglStencilOp :: failMess (int arg1) {
	fail = (GLenum)arg1;
	setModified();
}


void GEMglStencilOp :: zfailMess (int arg1) {
	zfail = (GLenum)arg1;
	setModified();
}


void GEMglStencilOp :: zpassMess (int arg1) {
	zpass = (GLenum)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglStencilOp :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglStencilOp,gensym("glStencilOp"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglStencilOp::failMessCallback, gensym("fail"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglStencilOp::zfailMessCallback, gensym("zfail"), A_NULL);
	class_addmethod(classPtr, (t_method)&GEMglStencilOp::zpassMessCallback, gensym("zpass"), A_NULL);
}


void GEMglStencilOp :: failMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->failMess (getGLdefine(arg0->s_name));
}
void GEMglStencilOp :: zfailMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->zfailMess (getGLdefine(arg0->s_name));
}
void GEMglStencilOp :: zpassMessCallback (   void* data, t_symbol*    arg0) {
	GetMyClass(data)->zpassMess (getGLdefine(arg0->s_name));
}
