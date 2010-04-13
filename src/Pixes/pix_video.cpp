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

#include "Base/GemConfig.h"

#ifndef GEM_VIDEOBACKEND

#include "pix_video.h"
#include "Base/GemState.h"
CPPEXTERN_NEW(pix_video)

/////////////////////////////////////////////////////////
//
// pix_video
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_video :: pix_video() : 
  m_videoHandle(NULL), m_driver(-1)
{
  gem::PluginFactory<gem::video, std::string>::loadPlugins("video");

  m_videoHandle=NULL;
  std::vector<std::string>ids=gem::PluginFactory<gem::video, std::string>::getIDs();

  addHandle(ids, "v4l2");
  addHandle(ids, "v4l");
  addHandle(ids, "dv4l");

  addHandle(ids);

  /*
   * calling driverMess() would immediately startTransfer(); 
   * we probably don't want this in initialization phase
   */
  if(m_videoHandles.size()>0) {
    m_driver=0;
    m_videoHandle=m_videoHandles[m_driver];
  } else {
    error("no video backends found!");
  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_video :: ~pix_video(){
  /* clean up all video handles;
   * the video-handles have to stop the transfer themselves
   */
  int i=0;
  for(i=0; i<m_videoHandles.size(); i++) {
    delete m_videoHandles[i];
    m_videoHandles[i]=NULL;
  }
}

/////////////////////////////////////////////////////////
// add backends
//
/////////////////////////////////////////////////////////
bool pix_video :: addHandle( std::vector<std::string>available, std::string ID)
{
  int i=0;
  int count=0;

  std::vector<std::string>id;
  if(!ID.empty()) {
    // if requested 'cid' is in 'available' add it to the list of 'id's
    if(std::find(available.begin(), available.end(), ID)!=available.end()) {
      id.push_back(ID);
    } else {
      // request for an unavailable ID
      verbose(2, "backend '%s' unavailable", ID.c_str());
      return false;
    }
  } else {
    // no 'ID' given: add all available IDs
    id=available;
  }

  for(i=0; i<id.size(); i++) {
    std::string key=id[i];
    verbose(2, "trying to add '%s' as backend", key.c_str());
    if(std::find(m_ids.begin(), m_ids.end(), key)==m_ids.end()) {
      post("%d: '%s' ", m_videoHandles.size(), key.c_str());
      // not yet added, do so now!
      gem::video         *handle=gem::PluginFactory<gem::video, std::string>::getInstance(key); 
      if(NULL==handle)break;
      m_ids.push_back(key);
      m_videoHandles.push_back(handle);
      count++;
      verbose(2, "added backend#%d '%s' @ 0x%x", m_videoHandles.size()-1, key.c_str(), handle);
    }
  }

  return (count>0);
}



/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_video :: render(GemState *state){
   if (m_videoHandle)state->image=m_videoHandle->getFrame();
#if 0
   /* DEBUG for missed frames */
   if(!state->image->newimage){
     static int i=0;
     post("video: missed frame %d", i);
     i++;
   }
   else
     post("video: got frame");
#endif
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_video :: startRendering(){
  if (!m_videoHandle) {
    error("do video for this OS");
    return;
  }
  verbose(1, "starting transfer");
  m_videoHandle->startTransfer();
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_video :: stopRendering(){
  if (m_videoHandle)m_videoHandle->stopTransfer();
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_video :: postrender(GemState *state){
  state->image = NULL;
}
/////////////////////////////////////////////////////////
// dimenMess
//
/////////////////////////////////////////////////////////
void pix_video :: dimenMess(int x, int y, int leftmargin, int rightmargin,
			       int topmargin, int bottommargin)
{
  if (m_videoHandle)m_videoHandle->setDimen(x,y,leftmargin,rightmargin,topmargin,bottommargin);
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void pix_video :: offsetMess(int x, int y)
{
  if (m_videoHandle)m_videoHandle->setOffset(x,y);
}
/////////////////////////////////////////////////////////
// swapMess
//
/////////////////////////////////////////////////////////
void pix_video :: swapMess(int state)
{
  if (m_videoHandle)m_videoHandle->setSwap(state);
}
/////////////////////////////////////////////////////////
// channelMess
//
/////////////////////////////////////////////////////////
void pix_video :: channelMess(int channel, t_float freq)
{
  if(m_videoHandle)m_videoHandle->setChannel(channel, freq);
}
/////////////////////////////////////////////////////////
// normMess
//
/////////////////////////////////////////////////////////
void pix_video :: normMess(t_symbol *s)
{
  if(m_videoHandle)m_videoHandle->setNorm(s->s_name);
}
/////////////////////////////////////////////////////////
// colorMess
//
/////////////////////////////////////////////////////////
void pix_video :: colorMess(t_atom*a)
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
void pix_video :: driverMess(t_symbol*s)
{
  int dev;
  for(dev=0; dev<m_videoHandles.size(); dev++) {
    if(m_videoHandles[dev]->provides(s->s_name)) {
      driverMess(dev);
      return;
    }
  }
  error("could not find a backend for driver '%s'", s->s_name);
}
void pix_video :: driverMess(int dev)
{
  //  post("driver: %d", dev);
  if(dev>=m_videoHandles.size()){
    error("driverID (%d) must not exceed %d", dev, m_videoHandles.size());
    return;
  }
  //  if((dev!=m_driver) && (m_videoHandle!=m_videoHandles[dev]))
    { 
      if(m_videoHandle)m_videoHandle->stopTransfer();
      m_videoHandle=m_videoHandles[dev];
      if(m_videoHandle)m_videoHandle->startTransfer();
      m_driver=dev;
    }
}
/////////////////////////////////////////////////////////
// deviceMess
//
/////////////////////////////////////////////////////////
void pix_video :: deviceMess(int dev)
{
  if (m_videoHandle)m_videoHandle->setDevice(dev);
}
void pix_video :: deviceMess(t_symbol*s)
{
  int err=0;
  if (m_videoHandle)err=m_videoHandle->setDevice(s->s_name);
  
  verbose(1, "device-err: %d", err);
  if(!err){
    int d=0;
    if(m_videoHandle)m_videoHandle->stopTransfer();
    for(d=0; d<m_videoHandles.size(); d++){
      if(m_videoHandles[d]->setDevice(s->s_name)){
        m_videoHandle=m_videoHandles[d];
        post("switched to driver #%d", d);
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
void pix_video :: enumerateMess()
{
  error("enumerate not supported on this OS");
}
/////////////////////////////////////////////////////////
// dialog
//
/////////////////////////////////////////////////////////
void pix_video :: dialogMess(int argc, t_atom*argv)
{
  error("dialog not supported on this OS");
}

/////////////////////////////////////////////////////////
// qualityMess
//
/////////////////////////////////////////////////////////
void pix_video :: qualityMess(int dev) {
  if (m_videoHandle)m_videoHandle->setQuality(dev);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_video :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_video::dimenMessCallback),
    	    gensym("dimen"), A_GIMME, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_video::offsetMessCallback),
    	    gensym("offset"), A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_video::swapMessCallback),
    	    gensym("swap"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_video::normMessCallback),
    	    gensym("norm"), A_SYMBOL, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_video::channelMessCallback),
    	    gensym("channel"), A_GIMME, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_video::modeMessCallback),
    	    gensym("mode"), A_GIMME, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_video::colorMessCallback),
    	    gensym("color"), A_GIMME, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_video::colorMessCallback),
    	    gensym("colorspace"), A_GIMME, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_video::deviceMessCallback),
    	    gensym("device"), A_GIMME, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_video::driverMessCallback),
    	    gensym("driver"), A_GIMME, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_video::driverMessCallback),
    	    gensym("open"), A_GIMME, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_video::enumerateMessCallback),
    	    gensym("enumerate"), A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_video::dialogMessCallback),
    	    gensym("dialog"), A_GIMME, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_video::qualityMessCallback),
	    gensym("quality"), A_FLOAT, A_NULL);
}
void pix_video :: dimenMessCallback(void *data, t_symbol *s, int ac, t_atom *av)
{
  GetMyClass(data)->dimenMess(static_cast<int>(atom_getfloatarg(0, ac, av)),
			      static_cast<int>(atom_getfloatarg(1, ac, av)),
			      static_cast<int>(atom_getfloatarg(2, ac, av)),
			      static_cast<int>(atom_getfloatarg(3, ac, av)),
			      static_cast<int>(atom_getfloatarg(4, ac, av)),
			      static_cast<int>(atom_getfloatarg(5, ac, av)) );
}
void pix_video :: offsetMessCallback(void *data, t_floatarg x, t_floatarg y)
{
    GetMyClass(data)->offsetMess(static_cast<int>(x), static_cast<int>(y));
}
void pix_video :: swapMessCallback(void *data, t_floatarg state)
{
    GetMyClass(data)->swapMess(static_cast<int>(state));
}
void pix_video :: channelMessCallback(void *data, t_symbol*s, int argc, t_atom*argv)
{
  if (argc!=1&&argc!=2)return;
  int chan = atom_getint(argv);
  t_float freq = (argc==1)?0:atom_getfloat(argv+1);
  GetMyClass(data)->channelMess(static_cast<int>(chan), freq);

}
void pix_video :: normMessCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->normMess(s);
}
void pix_video :: modeMessCallback(void *data, t_symbol* nop, int argc, t_atom *argv)
{
  switch (argc){
  case 1:
    if      (A_FLOAT ==argv->a_type)GetMyClass(data)->channelMess(atom_getint(argv));
    else if (A_SYMBOL==argv->a_type)GetMyClass(data)->normMess(atom_getsymbol(argv));
    else goto mode_error;
    break;
  case 2:
    if (A_SYMBOL==argv->a_type && A_FLOAT==(argv+1)->a_type){
      GetMyClass(data)->normMess(atom_getsymbol(argv));
      GetMyClass(data)->channelMess(atom_getint(argv+1));
    } else goto mode_error;  
    break;
  default:
  mode_error:
    ::post("invalid arguments for message \"mode [<norm>] [<channel>]\"");
  }
}
void pix_video :: colorMessCallback(void *data, t_symbol* nop, int argc, t_atom *argv){
  if (argc==1)GetMyClass(data)->colorMess(argv);
  else GetMyClass(data)->error("invalid number of arguments (must be 1)");
}
void pix_video :: deviceMessCallback(void *data, t_symbol*,int argc, t_atom*argv)
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
      GetMyClass(data)->error("device must be integer or symbol");
    }
  } else {
    GetMyClass(data)->error("can only set to 1 device at a time");
  }
}
void pix_video :: driverMessCallback(void *data, t_symbol*s, int argc, t_atom*argv)
{
  if(argc!=1) {
    GetMyClass(data)->error("'driver' takes a single numeric or symbolic driver ID");
  } else if (argv->a_type == A_FLOAT) {
    GetMyClass(data)->driverMess(atom_getint(argv));
  } else if (argv->a_type == A_SYMBOL) {
    GetMyClass(data)->driverMess(atom_getsymbol(argv));
  } else {
    GetMyClass(data)->error("'driver' takes a single numeric or symbolic driver ID");
  }
}
void pix_video :: enumerateMessCallback(void *data)
{
  GetMyClass(data)->enumerateMess();
}
void pix_video :: dialogMessCallback(void *data, t_symbol*s, int argc, t_atom*argv)
{
  GetMyClass(data)->dialogMess(argc, argv);
}
void pix_video :: qualityMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->qualityMess(static_cast<int>(state));
}
#endif /* no OS-specific GEM_VIDEOBACKEND */
