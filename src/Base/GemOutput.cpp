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

#include "GemOutput.h"

/////////////////////////////////////////////////////////
//
// GemOutput
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemOutput :: GemOutput()
{}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemOutput :: ~GemOutput()
{
  destroyMess();
}

/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void GemOutput :: renderMess(gem_control cb)
{
  preRender(cb);
  if(cb!=NULL)(cb);
  postRender(cb);
}
/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
void GemOutput :: createMess()
{
}
/////////////////////////////////////////////////////////
// destroyMess
//
/////////////////////////////////////////////////////////
void GemOutput :: destroyMess()
{
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void GemOutput :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&GemOutput::renderMessCallback,
		  gensym("gem_render"), A_POINTER, A_NULL);
  class_addmethod(classPtr, (t_method)&GemOutput::createMessCallback,
		  gensym("create"), A_NULL);
  class_addmethod(classPtr, (t_method)&GemOutput::destroyMessCallback,
		  gensym("destroy"), A_NULL);
}
void GemOutput :: renderMessCallback(void *data, t_gpointer*cb)
{
  GetMyClass(data)->renderMess((gem_control)cb);
}
void GemOutput :: createMessCallback(void *data)
{
  GetMyClass(data)->createMess();
}
void GemOutput :: destroyMessCallback(void *data)
{
  GetMyClass(data)->destroyMess();
}
