////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2011 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "ThreadMutex.h"


#include <pthread.h>

#ifdef _WIN32
# include <winsock2.h>
#endif


class gem::thread::Mutex::PIMPL {
public:
  pthread_mutex_t*mutex;
  PIMPL(void) : mutex(NULL) {}

};


gem::thread::Mutex::Mutex(void) : m_pimpl(new PIMPL()) {
  m_pimpl->mutex=new pthread_mutex_t;
  pthread_mutex_init(m_pimpl->mutex, NULL); 
}
gem::thread::Mutex::~Mutex(void) {
  if(m_pimpl->mutex)
    pthread_mutex_destroy(m_pimpl->mutex); 

  delete(m_pimpl);
  m_pimpl=NULL;
}


void gem::thread::Mutex::lock(void) {
  pthread_mutex_lock(m_pimpl->mutex);
}

void gem::thread::Mutex::unlock(void) {
  pthread_mutex_unlock(m_pimpl->mutex);
}

bool gem::thread::Mutex::trylock(void) {
  return (pthread_mutex_trylock(m_pimpl->mutex));
}

