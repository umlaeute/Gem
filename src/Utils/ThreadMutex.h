/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2011, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  locks a thread (wrapper around pthread_mutex)

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEM_THREADMUTEX_H_
#define _INCLUDE__GEM_GEM_THREADMUTEX_H_


#include "Gem/ExportDef.h"

namespace gem
{
namespace thread
{
class GEM_EXTERN Mutex
{
private:
  class PIMPL;
  PIMPL*m_pimpl;
public:
  Mutex(void);
  virtual ~Mutex(void);

  Mutex(const Mutex&);
  Mutex&operator=(const Mutex&);

  void lock   (void);
  void unlock (void);
  bool trylock(void);
};
};
};
#endif /* _INCLUDE__GEM_GEM_THREADMUTEX_H_ */
