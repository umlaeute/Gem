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

// we set the help-symbol ourselfs
#define HELPSYMBOL "pix_video"

#include "pix_videoNEW.h"
#include "Pixes/videoV4L.h"
#include "Pixes/videoV4L2.h"
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
  m_videoHandle=NULL;
  int i = MAX_VIDEO_HANDLES;
  while(i--)m_videoHandles[i]=NULL;
  i=0;
#ifdef HAVE_VIDEO4LINUX2
  startpost("video driver %d: ", i); m_videoHandles[i]=new videoV4L2(GL_RGBA);  i++;
#elif defined HAVE_VIDEO4LINUX
  startpost("video driver %d: ", i); m_videoHandles[i]=new videoV4L(GL_RGBA);  i++;
#endif /* V4L */
#ifdef HAVE_LIBDV
  startpost("video driver %d: ", i); m_videoHandles[i]=new videoDV4L(GL_RGBA);  i++;
#endif /* DV4L */

  m_numVideoHandles=i;
  driverMess(0);
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
  post("pix_videoNEW: starting transfer");
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
      char c =*atom_getsymbol(a)->s_name;
      // we only have 3 colour-spaces: monochrome (GL_LUMINANCE), yuv (GL_YCBCR_422_GEM), and rgba (GL_RGBA)
      // if you don't need colour, i suggest, take monochrome
      // if you don't need alpha,  i suggest, take yuv
      // else take rgba
      switch (c){
      case 'g': case 'G': format=GL_LUMINANCE; break;
      case 'y': case 'Y': format=GL_YCBCR_422_GEM; break;
      case 'r': case 'R':
      default: format=GL_RGBA;
      }
  } else format=atom_getint(a);
  if(m_videoHandle)m_videoHandle->setColor(format);
}
/////////////////////////////////////////////////////////
// driverMess
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: driverMess(int dev)
{
//  post("driver: %d", dev);
  if(dev>=m_numVideoHandles){
    post("driverID (%d) must not exceed %d", dev, m_numVideoHandles);
    return;
  }
  if(m_videoHandle==m_videoHandles[dev])return;
  //  if(m_videoHandle)m_videoHandle->stopTransfer();
  m_videoHandle=m_videoHandles[dev];
  //  if(m_videoHandle)m_videoHandle->startTransfer();
}
/////////////////////////////////////////////////////////
// deviceMess
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: deviceMess(int dev)
{
  if (m_videoHandle)m_videoHandle->setDevice(dev);
}
void pix_videoNEW :: deviceMess(t_symbol*s)
{
  int err=0;
  if (m_videoHandle)err=m_videoHandle->setDevice(s->s_name);
  if(!err){
    int d=0;
    if(m_videoHandle)m_videoHandle->stopTransfer();
    for(d=0; d<m_numVideoHandles; d++){
      if(m_videoHandles[d]->setDevice(s->s_name)){
        m_videoHandle=m_videoHandles[d];
        break;
      }
    }
  }
  if(m_videoHandle)m_videoHandle->startTransfer();  
}
/////////////////////////////////////////////////////////
// enumerate devices
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: enumerateMess()
{
  error("enumerate not supported on this OS");
}
/////////////////////////////////////////////////////////
// dialog
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: dialogMess(int argc, t_atom*argv)
{
  error("dialog not supported on this OS");
}

/////////////////////////////////////////////////////////
// qualityMess
//
/////////////////////////////////////////////////////////
void pix_videoNEW :: qualityMess(int dev) {
  if (m_videoHandle)m_videoHandle->setQuality(dev);
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
    class_addmethod(classPtr, (t_method)&pix_videoNEW::colorMessCallback,
    	    gensym("colorspace"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoNEW::deviceMessCallback,
    	    gensym("device"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoNEW::driverMessCallback,
    	    gensym("driver"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoNEW::driverMessCallback,
    	    gensym("open"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoNEW::enumerateMessCallback,
    	    gensym("enumerate"), A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoNEW::dialogMessCallback,
    	    gensym("dialog"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoNEW::qualityMessCallback,
	    gensym("quality"), A_FLOAT, A_NULL);
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
void pix_videoNEW :: deviceMessCallback(void *data, t_symbol*,int argc, t_atom*argv)
{
  if(argc==1){
    switch(argv->a_type){
    case A_FLOAT:
      GetMyClass(data)->deviceMess(atom_getint(argv));
      break;
    case A_SYMBOL:
      GetMyClass(data)->deviceMess(atom_getsymbol(argv));
      break;
    default:
      error("pix_video: device must be integer or symbol");
    }
  } else {
    error("pix_video: can only set to 1 device at a time");
  }
}
void pix_videoNEW :: driverMessCallback(void *data, t_floatarg state)
{
    GetMyClass(data)->driverMess((int)state);
}
void pix_videoNEW :: enumerateMessCallback(void *data)
{
  GetMyClass(data)->enumerateMess();
}
void pix_videoNEW :: dialogMessCallback(void *data, t_symbol*s, int argc, t_atom*argv)
{
  GetMyClass(data)->dialogMess(argc, argv);
}
void pix_videoNEW :: qualityMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->qualityMess((int)state);
}
