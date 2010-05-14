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
//    Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
  
#include "plugins/video.h"
#include "Gem/RTE.h"
using namespace gem;

class video :: PIMPL {
  friend class video;
public:
  /* interfaces */
  // the list of provided device-classes
  std::vector<const char*>m_providers;


  /* threading */
  bool threading;
  pthread_t thread;
  pthread_mutex_t*lock;

  bool cont;
  bool running;

  PIMPL(bool threading_) :
    threading(threading_),
    lock(NULL),
    cont(true),
    running(false)
  {
  }
  ~PIMPL(void) {
    cont=false;
    if(lock)pthread_mutex_destroy(lock); delete lock; lock=NULL;
  }


  static void*threadfun(void*you) {
    video*me=(video*)you;
    pixBlock*pix=NULL;
    me->m_pimpl->cont=true;
    me->m_pimpl->running=true;

    while(me->m_pimpl->cont) {
      me->grabFrame();
    }

    me->m_pimpl->running=false;

    return NULL;
  }
};


/////////////////////////////////////////////////////////
//
// pix_videoLinux
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
video :: video(bool threaded) :
  m_capturing(false), m_haveVideo(false), 
  m_width(64), m_height(64),
  m_channel(0), m_norm(0),
  m_reqFormat(GL_RGBA),
  m_devicename(NULL), m_devicenum(0), m_quality(0),
  m_pimpl(new PIMPL(threaded))
{
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
video :: ~video()
{
  if(m_pimpl)delete m_pimpl; m_pimpl=NULL;

  if(m_haveVideo)closeDevice();
}
/////////////////////////////////////////////////////////
// open/close
//
/////////////////////////////////////////////////////////
bool video :: open()
{
  //  post("open: %d -> %d", m_haveVideo, m_capturing);
  if(m_haveVideo)close();
  m_haveVideo=openDevice();
  return m_haveVideo;
}
void video :: close()
{
  //  post("close: %d -> %d", m_capturing, m_haveVideo);
  if(m_capturing)stop();
  if(m_haveVideo)closeDevice();
  m_haveVideo=false;
}
/////////////////////////////////////////////////////////
// start/stop
//
/////////////////////////////////////////////////////////
bool video :: start()
{
  //  post("start: %d -> %d", m_haveVideo, m_capturing);
  if(!m_haveVideo)return false;
  if(m_capturing)stop();
  m_capturing=startTransfer();
  startThread();
  return m_capturing;
}
bool video :: stop()
{
  //  post("stop: %d -> %d", m_capturing, m_haveVideo);
  bool running=m_capturing;
  if(!m_haveVideo)return false;
  stopThread();
  if(running)
    running=stopTransfer();

  m_capturing=false;
  return running;
}



/////////////////////////////////////////////////////////
// openDevice
//
/////////////////////////////////////////////////////////
bool video :: openDevice()
{
  return false;
}

/////////////////////////////////////////////////////////
// closeDevice
//
/////////////////////////////////////////////////////////
void video :: closeDevice()
{}

/////////////////////////////////////////////////////////
// resetDevice
//
/////////////////////////////////////////////////////////
bool video :: reset()
{
  return(false);
}
/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool video :: startTransfer()
{
  return false;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool video :: stopTransfer()
{
  return false;
}

/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool video :: restartTransfer()
{
  bool running=stopTransfer();
  if(running)return startTransfer();

  return false;
}


bool video::startThread() {
  if(!m_pimpl)return false;
  if(m_pimpl->lock){
    stopThread();
  }

  if(m_pimpl->threading) {
    m_pimpl->lock = new pthread_mutex_t;
    if ( pthread_mutex_init(m_pimpl->lock, NULL) < 0 ) {
      delete m_pimpl->lock;
      return false;
    }

    pthread_create(&m_pimpl->thread, 
                   0,
                   m_pimpl->threadfun, 
                   this);
  }
}
bool video::stopThread(unsigned int timeout) {
  if(!m_pimpl)return true;
  int i=0;

  m_pimpl->cont=false;
  if(timeout>0) {
    while(m_pimpl->running) {
      struct timeval sleep;
      sleep.tv_sec=0;  sleep.tv_usec=10; /* 10us */
      select(0,0,0,0,&sleep);
      i+=10;
      if(i>timeout) {
        return false;
      }
    }
  } else {
    while(m_pimpl->running) {
      struct timeval sleep;
      sleep.tv_sec=0;  sleep.tv_usec=10; /* 10us */
      select(0,0,0,0,&sleep);
      i+=10;
      if(i>1000000) {
        post("waiting for video grabbing thread to terminate...");
        i=0;
      }
    }
  }

  if(m_pimpl->lock){
    int i=0;
    while(pthread_mutex_destroy(m_pimpl->lock)) {
      struct timeval sleep;
      pthread_mutex_unlock(m_pimpl->lock);
      sleep.tv_sec=0;  sleep.tv_usec=10; /* 10us */
      select(0,0,0,0,&sleep);
      i++;
    }
    delete m_pimpl->lock;
    m_pimpl->lock=NULL;
  }

  return true;
}
void video::lock() {
  if(m_pimpl && m_pimpl->lock)
    pthread_mutex_lock(m_pimpl->lock);
}
void video::unlock() {
  if(m_pimpl && m_pimpl->lock)
    pthread_mutex_unlock(m_pimpl->lock);
}


pixBlock* video::getFrame(void) {
  if(m_pimpl && m_pimpl->running) {
    // get from thread
  } else {
    // no thread, grab it directly
    grabFrame();
  }

  return &m_image;
}


void video::releaseFrame(void) {
  m_image.newimage=false;
}

/////////////////////////////////////////////////////////
// set dimension
int video :: setDimen(int x, int y, int leftmargin, int rightmargin, int topmargin, int bottommargin){
  post("setting the dimension for video is not supported by this OS/device");
  return -1;
}
/////////////////////////////////////////////////////////
// set the displacment
int video :: setOffset(int x, int y){
  post("setting the channel is not supported by this OS/device");
  return -1;
}
/////////////////////////////////////////////////////////
// should the image be swapped ?
int video :: setSwap(int state){
  post("swapping the image is not supported by this OS/device");
  return -1;
}
/////////////////////////////////////////////////////////
// set the tv-norm
int video :: setNorm(char *n){
  post("setting the video-norm is not supported by this OS/device");
  return -1;
}
/////////////////////////////////////////////////////////
// set the channel of the current device
int video :: setChannel(int chan, t_float freq){
  post("setting the channel is not supported by this OS/device");
  return -1;
}
/////////////////////////////////////////////////////////
// set the device
int video :: setDevice(int d){
  post("setting the video-device is not supported by this OS/device");
  return -1;
}
int video :: setDevice(char*name){
  post("setting the video-device is not supported by this OS/device");
  return -1;
}
/////////////////////////////////////////////////////////
// set the color-space
int video :: setColor(int d){
  post("setting the color-space is not supported by this OS/device");
  return -1;
}
/////////////////////////////////////////////////////////
// set the quality for DV decoding
int video :: setQuality(int d){
  post("setting the quality is not supported by this OS/device");
  return -1;
}

/////////////////////////////////////////////////////////
// query whether this backend provides a certain type of video decoding, e.g. "dv"
bool video :: provides(const char*name) {
  if(!m_pimpl)return false;
  std::vector<const char*>::iterator it;
  for ( it=m_pimpl->m_providers.begin() ; it < m_pimpl->m_providers.end(); it++ ) {
    if(!strcmp(name, *it))return true;
  }

  return false;
}

/////////////////////////////////////////////////////////
// remember that this backend provides a certain type of video decoding, e.g. "dv"
void video :: provide(const char*name) {
  if(!m_pimpl)return;
  if(!provides(name)) {
    m_pimpl->m_providers.push_back(name);
    startpost("%s ", name);
  }
}


INIT_VIDEOFACTORY();
