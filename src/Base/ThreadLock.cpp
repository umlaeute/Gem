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

#include "ThreadLock.h"


#include <pthread.h>

#ifdef _WIN32
# include <winsock2.h>
#endif


class gem::threading::Lock::PIMPL {
public:
  pthread_mutex_t*lock;
  PIMPL(void) : lock(NULL) {}

};


gem::threading::Lock::Lock(void) : m_pimpl(new PIMPL()) {
  

}
gem::threading::Lock::~Lock(void) {
  if(m_pimpl->lock)
    pthread_mutex_destroy(m_pimpl->lock); 

  delete(m_pimpl);
  m_pimpl=NULL;
}


void gem::threading::Lock::lock(void) {
  pthread_mutex_lock(m_pimpl->lock);
}

void gem::threading::Lock::unlock(void) {
  pthread_mutex_unlock(m_pimpl->lock);
}

bool gem::threading::Lock::trylock(void) {
  return (pthread_mutex_trylock(m_pimpl->lock));
}

