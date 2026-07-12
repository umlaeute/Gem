/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2012, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  simple platform independent thread abstraction

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
  // gets called from an alternative thread
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
