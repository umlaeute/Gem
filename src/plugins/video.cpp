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

#if 1
#define debugPost post
#else
#define debugPost
#endif


class video :: PIMPL {
  friend class video;
public:
  /* interfaces */
  // the list of provided device-classes
  std::vector<std::string>m_providers;


  /* threading */
  bool threading;
  pthread_t thread;
  pthread_mutex_t**locks;
  unsigned int numlocks;

  unsigned int timeout;

  bool cont;
  bool running;

  bool shouldrun; /* we should be capturing */

  PIMPL(unsigned int locks_, unsigned int timeout_) :
    threading(locks_>0),
    locks(NULL),
    numlocks(0),
    timeout(timeout_),
    cont(true),
    running(false),
    shouldrun(false)
  {
    if(locks_>0) {
      numlocks=locks_;
      locks=new pthread_mutex_t*[numlocks];
      int i=0;
      for(i=0; i<locks_; i++)
        locks[i]=NULL;
    }
  }
  ~PIMPL(void) {
    cont=false;
    lock_delete();
    delete[]locks; 
    locks=NULL;
  }

  void lock(unsigned int i) {
    //    post("lock %d?\t%d", i, numlocks);

    if(i<numlocks && locks[i]) {
      pthread_mutex_lock(locks[i]);
    }
  }
  void unlock(unsigned int i) {
    //      post("unlock %d? %d", i,numlocks);

    if(i<numlocks && locks[i]) {
      pthread_mutex_unlock(locks[i]);
    }
  }
  bool lock_new(void) {
    if(locks) {
      int i=0;
      for(i=0; i<numlocks; i++) {
        locks[i]=new pthread_mutex_t;
        if ( pthread_mutex_init(locks[i], NULL) < 0 ) {
          lock_delete();
          return false;
        }
      }
      return true;
    }
  }
  void lock_delete(void) {
    if(locks) {
      int i=0;
      for(i=0; i<numlocks; i++) {
        if(locks[i]) {
          pthread_mutex_destroy(locks[i]); 
          delete locks[i];
          locks[i]=NULL;
        }
      }
    }
  }

  static void*threadfun(void*you) {
    video*me=(video*)you;
    pixBlock*pix=NULL;
    post("starting capture thread");
    me->m_pimpl->cont=true;
    me->m_pimpl->running=true;

    while(me->m_pimpl->cont) {
      if(!me->grabFrame()) {
	break;
      }
    }
    me->m_pimpl->running=false;
    post("exiting capture thread");
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
video :: video(unsigned int locks, unsigned int timeout) :
  m_capturing(false), m_haveVideo(false), 
  m_width(64), m_height(64),
  m_channel(0), m_norm(0),
  m_reqFormat(GL_RGBA),
  m_devicename(std::string("")), m_devicenum(0), m_quality(0),
  m_pimpl(new PIMPL(locks, timeout))
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
  debugPost("open: %d -> %d", m_haveVideo, m_capturing);
  if(m_haveVideo)close();
  m_haveVideo=openDevice();
  return m_haveVideo;
}
void video :: close()
{
  debugPost("close: %d -> %d", m_capturing, m_haveVideo);
  if(m_capturing)stop();
  if(m_haveVideo)closeDevice();
  m_pimpl->shouldrun=false;
  m_haveVideo=false;
}
/////////////////////////////////////////////////////////
// start/stop
//
/////////////////////////////////////////////////////////
bool video :: start()
{
  debugPost("start: %d -> %d", m_haveVideo, m_capturing);
  if(!m_haveVideo)return false;
  if(m_capturing)stop();
  m_capturing=startTransfer();
  if(m_capturing)
    startThread();

  m_pimpl->shouldrun=true;
  return m_capturing;
}
bool video :: stop()
{
  debugPost("stop: %d -> %d", m_capturing, m_haveVideo);
  bool running=m_pimpl->shouldrun;
  m_pimpl->shouldrun=false;
  if(!m_haveVideo)return false;
  if(m_capturing) {
    stopThread();
    stopTransfer();
  }

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
  bool running=stop();
  if(running)return start();

  return false;
}


bool video::startThread() {
  post("startThread %x", m_pimpl);
  if(m_pimpl->running) {
    stopThread();
  }

  if(m_pimpl->threading) {
    if(!m_pimpl->lock_new())return false;

    pthread_create(&m_pimpl->thread, 
                   0,
                   m_pimpl->threadfun, 
                   this);
    return true;
  }
  return false;
}
bool video::stopThread(int timeout) {
  int i=0;
  post("stopThread: %d", timeout);
  m_pimpl->cont=false;
  if(timeout<0)timeout=m_pimpl->timeout;
  if(timeout>0) {
    while(m_pimpl->running) {
      usleep(10);
      i+=10;
      if(i>timeout) {
        return false;
      }
    }
  } else {
    while(m_pimpl->running) {
      usleep(10);
      i+=10;
      if(i>1000000) {
        post("waiting for video grabbing thread to terminate...");
        i=0;
      }
    }
    //pthread_join(m_pimpl->thread, NULL);
  }

  m_pimpl->lock_delete();

  return true;
}
void video::lock(unsigned int id) {
  m_pimpl->lock(id);
}
void video::unlock(unsigned int id) {
  m_pimpl->unlock(id);
}
void video::usleep(unsigned long usec) {
  struct timeval sleep;
  long usec_ = usec%1000000;
  long sec_=0;
  //  long  sec_ = usec\1000000;
  sleep.tv_sec=sec_;
  sleep.tv_usec=usec_; 
  select(0,0,0,0,&sleep);
}

pixBlock* video::getFrame(void) {
  if(!(m_haveVideo && m_capturing))return NULL;
  if(m_pimpl && m_pimpl->running) {
    // get from thread
  } else {
    // no thread, grab it directly
    grabFrame();
  }
  lock();
  return &m_image;
}


void video::releaseFrame(void) {
  m_image.newimage=false;
  unlock();
}

/////////////////////////////////////////////////////////
// set dimension
bool video :: setDimen(int x, int y, int leftmargin, int rightmargin, int topmargin, int bottommargin){
  //  post("setting the dimension for video is not supported by this OS/device");

  m_width=x;
  m_height=y;
  return false;
}

/////////////////////////////////////////////////////////
// set the tv-norm
bool video :: setNorm(const std::string norm){
  post("setting the video-norm is not supported by this OS/device");
  return false;
}
/////////////////////////////////////////////////////////
// set the channel of the current device
bool video :: setChannel(int chan, t_float freq){
  m_channel=chan;
  m_frequency=freq;
  return false;
}
/////////////////////////////////////////////////////////
// set the device
bool video :: setDevice(int d){
  post("setting the video-device is not supported by this OS/device");
  return false;
}
bool video :: setDevice(const std::string name){
  post("setting the video-device is not supported by this OS/device");
  return false;
}
/////////////////////////////////////////////////////////
// set the color-space
bool video :: setColor(int d){
  post("setting the color-space is not supported by this OS/device");
  return false;
}
/////////////////////////////////////////////////////////
// set the quality for DV decoding
bool video :: setQuality(int d){
  post("setting the quality is not supported by this OS/device");
  return false;
}
/////////////////////////////////////////////////////////
// open a dialog for the settings
bool video :: dialog(std::vector<std::string>dialognames){
  return false;
}

std::vector<std::string>video::enumerate(void) {
  std::vector<std::string>result;
  return result;
}





/////////////////////////////////////////////////////////
// query whether this backend provides a certain type of video decoding, e.g. "dv"
bool video :: provides(const std::string name) {
  if(!m_pimpl)return false;
  int i;
  for(i=0; i<m_pimpl->m_providers.size(); i++)
    if(name == m_pimpl->m_providers[i])return true;

  return false;
}
std::vector<std::string>video :: provides() {
  std::vector<std::string>result;
  if(m_pimpl) {
    int i;
    for(i=0; i<m_pimpl->m_providers.size(); i++)
      result.push_back(m_pimpl->m_providers[i]);
  }
  return result;
}


/////////////////////////////////////////////////////////
// remember that this backend provides a certain type of video decoding, e.g. "dv"
void video :: provide(const std::string name) {
  if(!m_pimpl)return;
  if(!provides(name)) {
    m_pimpl->m_providers.push_back(name);
  }
}


INIT_VIDEOFACTORY();
