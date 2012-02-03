////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2012-2012 IOhannes m zmölnig. forum::fur::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "Thread.h"

#ifdef _WIN32
# include <windows.h>
#endif

#if defined HAVE_UNISTD_H
# include <unistd.h>
#endif

unsigned int  gem::thread::getCPUCount(void) {
  // http://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine

#ifdef _OPENMP
  /* OpenMP */
  return omp_get_num_procs();
#endif

#ifdef _WIN32
  SYSTEM_INFO sysinfo;
  GetSystemInfo( &sysinfo );

  return (sysinfo.dwNumberOfProcessors);
#endif

#ifdef __linux__
  /* Linux, Solaris, & AIX (per comments): */
  return sysconf( _SC_NPROCESSORS_ONLN );
#endif


#if defined (__APPLE__) || defined (__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
  /* FreeBSD, MacOS X, NetBSD, OpenBSD,  */
  uint32_t count;

  int mib[4];
  size_t len = sizeof(count); 

  /* set the mib for hw.ncpu */
  mib[0] = CTL_HW;
  mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

  /* get the number of CPUs from the system */
  sysctl(mib, 2, &count, &len, NULL, 0);

  if( count < 1 ) {
    mib[1] = HW_NCPU;
    sysctl( mib, 2, &count, &len, NULL, 0 );
    
    if(count < 1 ) count = 1;
  }

  return count;
#endif

#ifdef __hpux
/* HPUX */
 return mpctl(MPC_GETNUMSPUS, NULL, NULL);
#endif

#ifdef __irix__
 /* IRIX */
 return sysconf( _SC_NPROC_ONLN );
#endif

  return 1; // safe default
}


#include <pthread.h>
#if defined __linux__ || defined __APPLE__
# include <unistd.h>
# include <sys/time.h>
#endif
#ifdef _WIN32
# include <winsock2.h>
#endif

namespace gem { namespace thread {

class Thread::PIMPL { public:
  Thread*owner;
  volatile bool keeprunning;
  volatile bool isrunning;
  pthread_t p_thread;

  PIMPL(Thread*x):
    owner(x),
    keeprunning(true),
    isrunning(false)
  {}
  ~PIMPL(void) {
    stop(0);
  }
  static inline void*process(void*you) {
    PIMPL*me=reinterpret_cast<PIMPL*>(you);
    Thread*owner=me->owner;
    me->isrunning=true;

    while(me->keeprunning) {
      if(!owner->process())
	break;
    }
    me->isrunning=false;
    return 0;
  }
  bool start(void) {
    if(isrunning)return true;

    keeprunning=true;
    pthread_create(&p_thread, 0, process, this);

    struct timeval sleep;
    while(!isrunning) {
      sleep.tv_sec=0;
      sleep.tv_usec=10;
      select(0,0,0,0,&sleep);
    }

    return true;  
  }

  bool stop(unsigned int timeout) {
      if(!isrunning)return true;
      int timmy=(timeout/10); // we are sleeping for 10usec in each cycle
      bool checktimeout=(timeout>0);

      keeprunning=false;

      struct timeval sleep;
      while(isrunning) {
        sleep.tv_sec=0;
        sleep.tv_usec=10;
        select(0,0,0,0,&sleep);
	if(checktimeout && (timmy--<10))break;
      }
      return (!isrunning);
  }
};

Thread::Thread(void) :
  m_pimpl(new PIMPL(this)) {
}
Thread::~Thread(void) {
  stop(true);
  delete m_pimpl;
  m_pimpl=0;
}
bool Thread::start(void) {
  return m_pimpl->start();
}
bool Thread::stop(unsigned int timeout) {
  return m_pimpl->stop(timeout);
}
/* _private_ dummy implementations */
Thread&Thread::operator=(const Thread&org) {
  return (*this);
}
Thread::Thread(const Thread&org) : m_pimpl(new PIMPL(this)) {
}


};}; // namespace
