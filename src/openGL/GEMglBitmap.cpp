////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//	zmoelnig@iem.kug.ac.at
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
//  this file has been generated...
////////////////////////////////////////////////////////

#include "GEMglBitmap.h"
#include "Base/GemPixUtil.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglBitmap , 
			  t_floatarg, A_DEFFLOAT, 
			  t_floatarg, A_DEFFLOAT, 
			  t_floatarg, A_DEFFLOAT, 
			  t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglBitmap :: GEMglBitmap	(t_float arg0=0, t_float arg1=0,
				 t_float arg2=1, t_float arg3=1) :
		xorig((GLfloat)arg0), 
		yorig((GLfloat)arg1), 
		xmove((GLfloat)arg2), 
		ymove((GLfloat)arg3)
{
  // img info: width, height, bitmap
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("xorig"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("yorig"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("xmove"));
	m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ymove"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglBitmap :: ~GEMglBitmap () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
inlet_free(m_inlet[3]);

}

/////////////////////////////////////////////////////////
// Render
//
void GEMglBitmap :: render(GemState *state) {
  if (!state || !state->image || !&state->image->image)return;
  
  glBitmap (state->image->image.xsize, state->image->image.ysize,
	    xorig, yorig, xmove, ymove, 
	    state->image->image.data);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglBitmap :: xorigMess (t_float arg1) {	// FUN
	xorig = (GLfloat)arg1;
	setModified();
}

void GEMglBitmap :: yorigMess (t_float arg1) {	// FUN
	yorig = (GLfloat)arg1;
	setModified();
}

void GEMglBitmap :: xmoveMess (t_float arg1) {	// FUN
	xmove = (GLfloat)arg1;
	setModified();
}

void GEMglBitmap :: ymoveMess (t_float arg1) {	// FUN
	ymove = (GLfloat)arg1;
	setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglBitmap :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglBitmap::xorigMessCallback,  	gensym("xorig"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglBitmap::yorigMessCallback,  	gensym("yorig"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglBitmap::xmoveMessCallback,  	gensym("xmove"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglBitmap::ymoveMessCallback,  	gensym("ymove"), A_DEFFLOAT, A_NULL);
};

void GEMglBitmap :: xorigMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->xorigMess ( (t_float)    arg0);
}
void GEMglBitmap :: yorigMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->yorigMess ( (t_float)    arg0);
}
void GEMglBitmap :: xmoveMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->xmoveMess ( (t_float)    arg0);
}
void GEMglBitmap :: ymoveMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->ymoveMess ( (t_float)    arg0);
}
