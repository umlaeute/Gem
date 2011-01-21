/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	- locks a thread

    Copyright (c) 2011 IOhannes m zmoelnig. forum::f-bür::umläute. IEM. zmoelnig@iem.at-A
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_THREADLOCK_H_
#define INCLUDE_GEM_THREADLOCK_H_


#include "Base/GemExportDef.h"

namespace gem { 
  namespace threading {
    GEM_EXTERN class Lock {
    private:
      class PIMPL;
      PIMPL*m_pimpl;
    public:
      Lock(void);
      virtual ~Lock(void);
      
      void lock   (void);
      void unlock (void);
      bool trylock(void);
    };
  };
};
#endif /* INCLUDE_GEM_THREADLOCK_H_ */
