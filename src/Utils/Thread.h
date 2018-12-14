/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Thread.h
       - part of GEM
       - simple platform independent thread abstraction

    Copyright (c) 2011-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEM_THREAD_H_
#define _INCLUDE__GEM_GEM_THREAD_H_

#include "Gem/ExportDef.h"

namespace gem
{
namespace thread
{
/**
 * get the number of available CPUs on the system
 */
GEM_EXTERN unsigned int getCPUCount(void);

class GEM_EXTERN Thread
{
private:
  class PIMPL;
  PIMPL*m_pimpl;
  friend class PIMPL;

  Thread(const Thread&);
  Thread&operator=(const Thread&);
public:
  Thread(void);
  virtual ~Thread(void);

  ////
  // start thread
  virtual bool start(void);
  ////
  // stop thread
  //  waits for at most wait4usec microseconds
  //  is wait4usec==0, waits until process terminates (e.g. forever)
  virtual bool stop(unsigned int wait4usec=0);

protected:
  ////
  // the worker!
  // get's called from an alternative thread
  // if TRUE is returned, process() will be called again
  //  until stop() is called
  // if FALSE is returned, the thread may exit
  virtual bool process(void) = 0;
};


////////
// wrapper around select() or whatever
GEM_EXTERN void usleep(unsigned long usec);
};
}; // namespace




#endif /* _INCLUDE__GEM_GEM_THREAD_H_ */
