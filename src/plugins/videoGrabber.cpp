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
  
#include "plugins/videoGrabber.h"
#include "plugins/PluginFactory.h"
#include "Gem/RTE.h"
#if 0
#include "Gem/Exception.h"

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

class videoGrabber :: PIMPL {
public:
  std::vector<gem::video*>m_grabbers;
  gem::video*m_grabber;

  std::vector<std::string > m_ids;

  bool isRunning, keepRunning;
  pthread_t p_thread;

  PIMPL() :
    m_grabber(NULL),
    isRunning(false), keepRunning(false)
  {
    gem::PluginFactory<gem::video>::loadPlugins("video");
    std::vector<std::string>ids=gem::PluginFactory<gem::video>::getIDs();

    addHandle(ids, "v4l2");
    addHandle(ids, "v4l");
    addHandle(ids, "dv4l");
    
    addHandle(ids);
  }
  ~PIMPL(void) {
  }

  /////////////////////////////////////////////////////////
  // add backends
  //
  /////////////////////////////////////////////////////////
  bool addHandle( std::vector<std::string>available, std::string ID=std::string("")) {
    unsigned int i=0;
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
      //      verbose(2, "trying to add '%s' as backend", key.c_str());
      if(std::find(m_ids.begin(), m_ids.end(), key)==m_ids.end()) {
        // not yet added, do so now!
        gem::video         *handle=NULL;
        //startpost("backend #%d='%s'\t", m_grabbers.size(), key.c_str());
        try {
          handle=gem::PluginFactory<gem::video>::getInstance(key); 
        } catch (GemException ex) {
        }
        if(NULL==handle) { 
          //post("<--- DISABLED");
          break;
        }
#if 0
        std::vector<std::string>devs=handle->provides();
        if(devs.size()>0) {
          startpost(": ");
          unsigned int i=0;
          for(i=0; i<devs.size(); i++) {
            startpost("%s ", devs[i].c_str());
          }
        }
#endif
        //  endpost();
        
        m_ids.push_back(key);
        m_grabbers.push_back(handle);
        count++;
        //verbose(2, "added backend#%d '%s' @ 0x%x", m_grabbers.size()-1, key.c_str(), handle);
      }
    }

  return (count>0);
}

  /* threading */
  void*process(void) {
    pixBlock*pix=NULL;
    post("starting capture thread");
    isRunning=true;
    
    while(keepRunning) {
      if(!grabFrame()) {
        break;
      }
    }
    isRunning=false;

    return NULL;
  }

  bool grabFrame(void) {
    return false;
  }

  static void*threadfun(void*you) {
    if(NULL==you)
      return NULL;
    PIMPL*me=(PIMPL*)you;
    return me->process();
  }

  void usleep(unsigned int usec) {
    struct timeval sleep;
    sleep.tv_sec=0;
    sleep.tv_usec=usec;
    select(0,0,0,0,&sleep);
  }

  bool startThread(void) {
    if(isRunning)return true;
    keepRunning=true;
    pthread_create(&p_thread, 0, threadfun, this);

    struct timeval sleep;
    while(!isRunning) {
      usleep(10);
    }
  }
  bool stopSthread(void) {
    if(!isRunning)return true;

    keepRunning=false;

    struct timeval sleep;
    while(isRunning) {
      usleep(10);
    }
    return true;
  }


  

};


/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
videoGrabber :: videoGrabber() :
  video("factory", 0, 0),
  m_pimpl(new PIMPL())
{
  
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoGrabber :: ~videoGrabber()
{
  if(m_haveVideo)closeDevice();
  if(m_pimpl)delete m_pimpl; m_pimpl=NULL;
}
/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoGrabber :: startTransfer()
{
  return false;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoGrabber :: stopTransfer()
{
  return false;
}


pixBlock* videoGrabber::getFrame(void) {
  pixBlock*pix=m_pimpl->getFrame();
  lock();
  return pix;
}


void videoGrabber::releaseFrame(void) {
  m_image.newimage=false;
  unlock();
  m_pimpl->thaw();
}

bool videoGrabber :: grabAsynchronous(bool fast) {
  return m_pimpl->setAsynchronous(fast);
}
#endif
INIT_VIDEOFACTORY();
