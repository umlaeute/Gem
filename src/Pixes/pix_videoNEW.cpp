////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
    
#include "pix_videoNEW.h"
#include "Pixes/videoV4L.h"
CPPEXTERN_NEW(pix_videoNEW)

/////////////////////////////////////////////////////////
//
// pix_videoNEW
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_videoNEW :: pix_videoNEW(){
  m_videoHandle=new videoV4L();
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_videoNEW :: ~pix_videoNEW(){
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: render(GemState *state){
   if (m_videoHandle)m_videoHandle->getFrame();
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: startRendering(){
  if (!m_videoHandle) {
    post("GEM: pix_videoNEW: do video for this OS");
    return;
  }
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: stopRendering(){

}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: postrender(GemState *state){
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: offsetMess(int x, int y)
{
  error("offset not supported on this OS");
}

/////////////////////////////////////////////////////////
// swapMess
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: swapMess(int state)
{
  error("swap not supported on this OS");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_videoNEW::dimenMessCallback,
    	    gensym("dimen"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoNEW::offsetMessCallback,
    	    gensym("offset"), A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoNEW::swapMessCallback,
    	    gensym("swap"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoNEW::formatMessCallback,
    	    gensym("format"), A_SYMBOL, A_NULL);
}
void pix_videoNEW :: dimenMessCallback(void *data, t_symbol *s, int ac, t_atom *av)
{
    GetMyClass(data)->dimenMess((int)atom_getfloatarg(0, ac, av),
    	(int)atom_getfloatarg(1, ac, av),
    	(int)atom_getfloatarg(2, ac, av),
    	(int)atom_getfloatarg(3, ac, av),
    	(int)atom_getfloatarg(4, ac, av),
    	(int)atom_getfloatarg(5, ac, av) );
}
void pix_videoNEW :: offsetMessCallback(void *data, t_floatarg x, t_floatarg y)
{
    GetMyClass(data)->offsetMess((int)x, (int)y);
}
void pix_videoNEW :: swapMessCallback(void *data, t_floatarg state)
{
    GetMyClass(data)->swapMess((int)state);
}

void pix_videoNEW :: formatMessCallback(void *data, t_symbol*s)
{
  //    GetMyClass(data)->formatMess(s);
}
