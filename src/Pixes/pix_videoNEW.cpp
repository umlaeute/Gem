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
#include "Pixes/videoDV4L.h"
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
  m_videoHandle=new videoV4L(GL_RGBA);
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
   if (m_videoHandle)state->image=m_videoHandle->getFrame();
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: startRendering(){
  if (!m_videoHandle) {
    post("GEM: pix_video: do video for this OS");
    return;
  }
  m_videoHandle->startTransfer();
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: stopRendering(){
  if (m_videoHandle)m_videoHandle->stopTransfer();
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: postrender(GemState *state){
  state->image = NULL;
}
/////////////////////////////////////////////////////////
// dimenMess
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: dimenMess(int x, int y, int leftmargin, int rightmargin,
			       int topmargin, int bottommargin)
{
  if (m_videoHandle)m_videoHandle->setDimen(x,y,leftmargin,rightmargin,topmargin,bottommargin);
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: offsetMess(int x, int y)
{
  if (m_videoHandle)m_videoHandle->setOffset(x,y);
}
/////////////////////////////////////////////////////////
// swapMess
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: swapMess(int state)
{
  if (m_videoHandle)m_videoHandle->setSwap(state);
}
/////////////////////////////////////////////////////////
// channelMess
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: channelMess(int channel, t_float freq)
{
  if(m_videoHandle)m_videoHandle->setChannel(channel, freq);
}
/////////////////////////////////////////////////////////
// normMess
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: normMess(t_symbol *s)
{
  if(m_videoHandle)m_videoHandle->setNorm(s->s_name);
}
/////////////////////////////////////////////////////////
// colorMess
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: colorMess(t_atom*a)
{
  int format=0;
  if (a->a_type==A_SYMBOL){
      char c =tolower(*atom_getsymbol(a)->s_name);
      char c2=tolower(atom_getsymbol(a)->s_name[3]);

      switch (c){
      case 'g': format=GL_LUMINANCE; break;
      case 'y': format=GL_YCBCR_422_GEM; break;
      case 'r':
	if (c2=='a')format=GL_RGBA;
	else format=GL_RGB;
	break;
#if defined (GL_BGRA) & defined (GL_BGR)
      case 'b':
	if (c2=='a')format=GL_BGRA;	else 
		format=GL_BGR;
	break;
#endif
      default: format=GL_RGBA;
      }
  } else format=atom_getint(a);
  if(m_videoHandle)m_videoHandle->setColor(format);
}
/////////////////////////////////////////////////////////
// deviceMess
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: deviceMess(int dev)
{
  if (m_videoHandle)m_videoHandle->setDevice(dev);
}
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_videoNEW,gensym("pix_video"),A_NULL);

    class_addmethod(classPtr, (t_method)&pix_videoNEW::dimenMessCallback,
    	    gensym("dimen"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoNEW::offsetMessCallback,
    	    gensym("offset"), A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoNEW::swapMessCallback,
    	    gensym("swap"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoNEW::normMessCallback,
    	    gensym("norm"), A_SYMBOL, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoNEW::channelMessCallback,
    	    gensym("channel"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoNEW::modeMessCallback,
    	    gensym("mode"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoNEW::colorMessCallback,
    	    gensym("color"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoNEW::deviceMessCallback,
    	    gensym("device"), A_FLOAT, A_NULL);
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
void pix_videoNEW :: channelMessCallback(void *data, t_symbol*s, int argc, t_atom*argv)
{
  if (argc!=1&&argc!=2)return;
  int chan = atom_getint(argv);
  t_float freq = (argc==1)?0:atom_getfloat(argv+1);
  GetMyClass(data)->channelMess((int)chan, freq);

}
void pix_videoNEW :: normMessCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->normMess(s);
}
void pix_videoNEW :: modeMessCallback(void *data, t_symbol* nop, int argc, t_atom *argv)
{
  switch (argc){
  case 1:
    if (argv->a_type==A_FLOAT)GetMyClass(data)->channelMess(atom_getint(argv));
    else if (argv->a_type==A_FLOAT)GetMyClass(data)->normMess(atom_getsymbol(argv));
    else goto mode_error;
    break;
  case 2:
    if (argv->a_type==A_SYMBOL && (argv+1)->a_type==A_FLOAT){
      GetMyClass(data)->normMess(atom_getsymbol(argv));
      GetMyClass(data)->channelMess(atom_getint(argv+1));
    } else goto mode_error;  
    break;
  default:
  mode_error:
    post("invalid arguments for message \"mode [<norm>] [<channel>]\"");
  }
}
void pix_videoNEW :: colorMessCallback(void *data, t_symbol* nop, int argc, t_atom *argv){
  if (argc==1)GetMyClass(data)->colorMess(argv);
  else post("pix_video: invalid number of arguments (must be 1)");
}
void pix_videoNEW :: deviceMessCallback(void *data, t_floatarg state)
{
    GetMyClass(data)->deviceMess((int)state);
}
