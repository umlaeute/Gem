////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2011, IOhannes m zmölnig and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////

#include "ThreadMutex.h"

#include <pthread.h>

#ifdef _WIN32
# include <winsock2.h>
#endif

class gem::thread::Mutex::PIMPL
{
public:
  pthread_mutex_t*mutex;
  unsigned int*refcount;
  PIMPL(void) : mutex(new pthread_mutex_t), refcount(new unsigned int)
  {
    *refcount=1;
    pthread_mutex_init(mutex, NULL);
  }
  PIMPL(const PIMPL&org) : mutex(org.mutex), refcount(org.refcount)
  {
    ++*refcount;
  }
  ~PIMPL(void)
  {
    --*refcount;
    if(*refcount==0) {
      pthread_mutex_destroy(mutex);
      delete mutex;
      delete refcount;
    }
    mutex=NULL;
    refcount=NULL;
  }
};


gem::thread::Mutex::Mutex(void) : m_pimpl(new PIMPL())
{
}
gem::thread::Mutex::Mutex(const gem::thread::Mutex&org) : m_pimpl(
    new PIMPL(*org.m_pimpl))
{
}

gem::thread::Mutex::~Mutex(void)
{
  delete(m_pimpl);
  m_pimpl=NULL;
}


void gem::thread::Mutex::lock(void)
{
  pthread_mutex_lock(m_pimpl->mutex);
}

void gem::thread::Mutex::unlock(void)
{
  pthread_mutex_unlock(m_pimpl->mutex);
}

bool gem::thread::Mutex::trylock(void)
{
  return (0!=pthread_mutex_trylock(m_pimpl->mutex));
}

gem::thread::Mutex&gem::thread::Mutex::operator=(const
    gem::thread::Mutex&org)
{
  if(this!=&org && m_pimpl->mutex != org.m_pimpl->mutex) {
    PIMPL*pimpl=new PIMPL(*org.m_pimpl);
    delete m_pimpl;
    m_pimpl=pimpl;
  }

  return(*this);
}
