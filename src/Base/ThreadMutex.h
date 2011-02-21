/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	- locks a thread (wrapper around pthread_mutex)

    Copyright (c) 2011 IOhannes m zmoelnig. forum::f-bür::umläute. IEM. zmoelnig@iem.at-A
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_THREADMUTEX_H_
#define INCLUDE_GEM_THREADMUTEX_H_


#include "Base/GemExportDef.h"

namespace gem { 
  namespace thread {
    GEM_EXTERN class Mutex {
    private:
      class PIMPL;
      PIMPL*m_pimpl;
    public:
      Mutex(void);
      virtual ~Mutex(void);
      
      void lock   (void);
      void unlock (void);
      bool trylock(void);
    };
  };
};
#endif /* INCLUDE_GEM_THREADMUTEX_H_ */
