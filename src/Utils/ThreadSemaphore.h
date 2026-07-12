/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2011, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  locks a thread (wrapper around pthread's cond_t)

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEM_THREADSEMAPHORE_H_
#define _INCLUDE__GEM_GEM_THREADSEMAPHORE_H_


#include "Gem/ExportDef.h"

namespace gem
{
namespace thread
{
class GEM_EXTERN Semaphore
{
private:
  class PIMPL;
  PIMPL*m_pimpl;
public:
  Semaphore(void);
  virtual ~Semaphore(void);
  Semaphore(const Semaphore&);

  /** block the current thread until the Semaphore is thaw()ed again */
  void freeze (void);
  /** unblock any waiting threads */
  void thaw   (void);

  virtual Semaphore&operator=(const Semaphore&);
};
};
};
#endif /* _INCLUDE__GEM_GEM_THREADSEMAPHORE_H_ */
