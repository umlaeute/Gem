////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Base/config.h"


#include "gemcontrol.h"
#include "Base/GemMan.h"


CPPEXTERN_NEW_WITH_ONE_ARG(gemcontrol, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// gemcontrol
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemcontrol :: gemcontrol(t_floatarg fps) {
  m_outlet = outlet_new(this->x_obj, 0);
  m_fpsOut = outlet_new(this->x_obj, 0);

  if (fps > 0.)GemMan::frameRate(fps);

  GemMan::addCtrl(this);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemcontrol :: ~gemcontrol(){
  GemMan::removeCtrl(this);
  if(m_outlet)outlet_free(m_outlet);
  if(m_fpsOut)outlet_free(m_fpsOut);
}
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void gemcontrol :: render()
{
  outlet_anything(m_outlet, gensym("gem_render"), 0, NULL);
}

/////////////////////////////////////////////////////////
// intMess
//
/////////////////////////////////////////////////////////
void gemcontrol :: intMess(int state)
{
  if (state)
    GemMan::startRendering();
  else
    GemMan::stopRendering();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemcontrol :: obj_setupCallback(t_class *classPtr){
  class_addfloat(classPtr, (t_method)&gemcontrol::floatMessCallback);

  class_addmethod(classPtr, (t_method)&gemcontrol::frameMessCallback,
		  gensym("frame"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&gemcontrol::frameMessCallback,
		  gensym("fps"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&gemcontrol::profileMessCallback,
		  gensym("profile"), A_FLOAT, A_NULL);


  // this should be the very method (aka: "fallback")
  class_addanything(classPtr, (t_method)&gemcontrol::anyMessCallback);
}


void gemcontrol :: floatMessCallback(void *data, t_float state)
{
  GetMyClass(data)->intMess((int)state);
}
void gemcontrol :: frameMessCallback(void*data, t_symbol*, int argc, t_atom*argv)
{
  GetMyClass(data)->frameMess(argc, argv);
}

void gemcontrol :: frameMess(int argc, t_atom*argv)
{
  switch(argc){
  default:
    error("GEM: gemcontrol: frame-message must have 0 or 1 arguments");
    return;
  case 1:
    GemMan::frameRate(atom_getfloat(argv));
  case 0:
    break;
  }
  outlet_float(m_fpsOut,(GemMan::fps));
}
void gemcontrol :: profileMessCallback(void *data, t_floatarg on)
{
  GemMan::m_profile = (int)on;
}

//////////////////////////////////////////////////////////////////////////////////
void gemcontrol :: anyMessCallback(void *data, t_symbol*s, int argc, t_atom*argv)
{
  GetMyClass(data)->thru(s, argc, argv);
}

void gemcontrol :: thru(t_symbol*s, int argc, t_atom*argv)
{
  outlet_anything(m_outlet, s, argc, argv);
}
