////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2010 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
  
#include "plugins/videoThreaded.h"
#include "Gem/RTE.h"
using namespace gem;

#if 0
# define debugPost post
#else
# define debugPost
#endif

#include <pthread.h>

#ifdef _WIN32
# include <winsock2.h>
#endif

/**
 * video capturing states
 *
 *  state                user-pov            system-pov
 * ----------------------------------------------------
 * is device open?       m_haveVideo         m_haveVideo
 * is device streaming?  m_pimpl->shouldrun  m_capturing
 * is thread running     (opaque)            m_pimpl->running
 *
 */

class videoThreaded :: PIMPL {
  friend class videoThreaded;
public:
  /* interfaces */
  // the list of provided device-classes
  std::vector<std::string>m_providers;

  /* threading */
  bool threading;
  pthread_t thread;
  pthread_mutex_t**locks;
  unsigned int numlocks;

  bool asynchronous;
  pthread_cond_t*condition_cond;
  pthread_mutex_t*condition_mutex;

  unsigned int timeout;

  bool cont;
  bool running;

  bool shouldrun; /* we should be capturing */

  const std::string name;

  PIMPL(const std::string name_, unsigned int locks_, unsigned int timeout_) :
    threading(locks_>0),
    locks(NULL),
    numlocks(0),
    asynchronous(true), condition_cond(NULL), condition_mutex(NULL),
    timeout(timeout_),
    cont(true),
    running(false),
    shouldrun(false),
    name(name_)
  {
    if(locks_>0) {
      numlocks=locks_;
      locks=new pthread_mutex_t*[numlocks];
      unsigned int i=0;
      for(i=0; i<locks_; i++)
        locks[i]=NULL;

      condition_mutex=new pthread_mutex_t;
      condition_cond =new pthread_cond_t;

      pthread_mutex_init(condition_mutex, NULL);
      pthread_cond_init(condition_cond, NULL);
    }
  }
  ~PIMPL(void) {
    cont=false;
    lock_delete();
    delete[]locks; 
    locks=NULL;

    doThaw();

    if(condition_mutex) {
      pthread_mutex_destroy(condition_mutex); 
      delete condition_mutex;
    }
    if(condition_cond) {
      pthread_cond_destroy(condition_cond); 
      delete condition_cond;
    }
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
      unsigned int i=0;
      for(i=0; i<numlocks; i++) {
        locks[i]=new pthread_mutex_t;
        if ( pthread_mutex_init(locks[i], NULL) < 0 ) {
          lock_delete();
          return false;
        }
      }
      return true;
    }
    return true;
  }
  void lock_delete(void) {
    if(locks) {
      unsigned int i=0;
      for(i=0; i<numlocks; i++) {
        if(locks[i]) {
          pthread_mutex_destroy(locks[i]); 
          delete locks[i];
          locks[i]=NULL;
        }
      }
    }
  }

  void doFreeze(void) {
    if(condition_mutex && condition_cond) {
      pthread_mutex_lock  ( condition_mutex );
       pthread_cond_wait  ( condition_cond, condition_mutex );
      pthread_mutex_unlock( condition_mutex );
    }
  }

  void freeze(void) {
    if(asynchronous)return;
    doFreeze();
  }

  void doThaw(void) {
    if(condition_mutex && condition_cond) {
      pthread_mutex_lock  (condition_mutex);
      pthread_cond_signal(condition_cond );
      pthread_mutex_unlock(condition_mutex);
    }
  }

  void thaw(void) {
    if(asynchronous)return;
    doThaw();
  }

  static void*threadfun(void*you) {
    videoThreaded*me=(videoThreaded*)you;
    pixBlock*pix=NULL;
    post("starting capture thread");
    me->m_pimpl->cont=true;
    me->m_pimpl->running=true;
    
    while(me->m_pimpl->cont) {
      if(!me->grabFrame()) {
        break;
      }
      me->m_pimpl->freeze();
    }
    me->m_pimpl->running=false;

    return NULL;
  }

  bool setAsynchronous(bool cont) {
    bool old=asynchronous;
    asynchronous=cont;
    if(asynchronous){
      doThaw();
    }
    return old;
  }
};


/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
videoThreaded :: videoThreaded(const std::string name, unsigned int locks, unsigned int timeout) :
  video(name, locks, timeout),
  m_pimpl(new PIMPL(name.empty()?std::string("<unknown>"):name, locks, timeout))
{
  if(!name.empty()) {
    provide(name);
  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoThreaded :: ~videoThreaded()
{
  if(m_pimpl)delete m_pimpl; m_pimpl=NULL;

  if(m_haveVideo)closeDevice();
}
/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoThreaded :: startTransfer()
{
  return false;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoThreaded :: stopTransfer()
{
  return false;
}

/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoThreaded :: restartTransfer()
{
  debugPost("restartTransfer");
  bool running=stop();
  if(running)return start();

  return false;
}


bool videoThreaded::startThread() {
  debugPost("startThread %d", m_pimpl->running);
  if(m_pimpl->running) {
    stopThread();
  }

  if(m_pimpl->threading) {
    if(!m_pimpl->lock_new())return false;

    pthread_create(&m_pimpl->thread, 
                   0,
                   m_pimpl->threadfun, 
                   this);
    while(!m_pimpl->running)
      usleep(10);

    return true;
  }
  return false;
}
bool videoThreaded::stopThread(int timeout) {
  int i=0;
  if(!m_pimpl->threading)return true;
  
  debugPost("stopThread: %d", timeout);

  m_pimpl->cont=false;

  m_pimpl->thaw();
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
      m_pimpl->thaw();
    }
    //pthread_join(m_pimpl->thread, NULL);
  }

  m_pimpl->lock_delete();
  return true;
}
void videoThreaded::lock(unsigned int id) {
  m_pimpl->lock(id);
}
void videoThreaded::unlock(unsigned int id) {
  m_pimpl->unlock(id);
}
void videoThreaded::usleep(unsigned long usec) {
  struct timeval sleep;
  long usec_ = usec%1000000;
  long sec_=0;
  //  long  sec_ = usec\1000000;
  sleep.tv_sec=sec_;
  sleep.tv_usec=usec_; 
  select(0,0,0,0,&sleep);
}

pixBlock* videoThreaded::getFrame(void) {
  pixBlock*pix=&m_image;
  if(!(m_haveVideo && m_capturing))return NULL;
  if(m_pimpl->threading) {
     // get from thread
    if(!m_pimpl->running){
      pix=NULL;
    }
  } else {
    // no thread, grab it directly
    if(!grabFrame()) {
      m_capturing=false;
      pix=NULL;
    }
  }
  lock();
  return pix;
}


void videoThreaded::releaseFrame(void) {
  m_image.newimage=false;
  unlock();
  m_pimpl->thaw();
}

bool videoThreaded :: grabAsynchronous(bool fast) {
  return m_pimpl->setAsynchronous(fast);
}

INIT_VIDEOFACTORY();
