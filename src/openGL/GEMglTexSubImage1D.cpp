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

#include "GEMglTexSubImage1D.h"
#include "Base/GemPixUtil.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglTexSubImage1D, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT )

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglTexSubImage1D :: GEMglTexSubImage1D(t_floatarg arg0=0,
					 t_floatarg arg1=0,
					 t_floatarg arg2=0) :
  level((GLint)arg0), 
  xoffset((GLint)arg1), 
  width((GLsizei)arg2) 
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("level"));
  m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("xoffset"));
  m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("width"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglTexSubImage1D :: ~GEMglTexSubImage1D () {
  inlet_free(m_inlet[0]);
  inlet_free(m_inlet[1]);
  inlet_free(m_inlet[2]);
}

//////////////////
// extension check
bool GEMglTexSubImage1D :: isRunnable(void) {
  if(GLEW_VERSION_1_1)return true;
  error("your system does not support OpenGL-1.1");
  return false;
}


/////////////////////////////////////////////////////////
// Render
//
void GEMglTexSubImage1D :: render(GemState *state) {
  if(!state||!state->image||!&state->image->image)return;
  target=GL_TEXTURE_1D;
  glTexSubImage1D (target, level, xoffset, width,
		   state->image->image.format, 
		   state->image->image.type, 
		   state->image->image.data);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglTexSubImage1D :: targetMess (t_float arg1) {	// FUN
  error("target has to be GL_TEXTURE_1D");
}

void GEMglTexSubImage1D :: levelMess (t_float arg1) {	// FUN
	level = (GLint)arg1;
	setModified();
}

void GEMglTexSubImage1D :: xoffsetMess (t_float arg1) {	// FUN
	xoffset = (GLint)arg1;
	setModified();
}

void GEMglTexSubImage1D :: widthMess (t_float arg1) {	// FUN
	width = (GLsizei)arg1;
	setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglTexSubImage1D :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglTexSubImage1D::targetMessCallback,  	gensym("target"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexSubImage1D::levelMessCallback,  	gensym("level"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexSubImage1D::xoffsetMessCallback,  	gensym("xoffset"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglTexSubImage1D::widthMessCallback,  	gensym("width"), A_DEFFLOAT, A_NULL);
}

void GEMglTexSubImage1D :: targetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->targetMess ( (t_float)    arg0);
}
void GEMglTexSubImage1D :: levelMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->levelMess ( (t_float)    arg0);
}
void GEMglTexSubImage1D :: xoffsetMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->xoffsetMess ( (t_float)    arg0);
}
void GEMglTexSubImage1D :: widthMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->widthMess ( (t_float)    arg0);
}
