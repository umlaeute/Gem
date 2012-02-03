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

using namespace gem::thread;

unsigned int Thread::getCPUCount(void) {
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
