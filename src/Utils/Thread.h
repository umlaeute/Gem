/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Thread.h
       - part of GEM
       - simple thread abstraction

    Copyright (c) 2011-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEM_THREAD_H_
#define _INCLUDE__GEM_GEM_THREAD_H_

#include "Gem/ExportDef.h"

namespace gem { namespace thread {
    GEM_EXTERN extern unsigned int getCPUCount(void);
#if 0
    class GEM_EXTERN Thread {
    private:
      class PIMPL;
      PIMPL*m_pimpl;
      friend class PIMPL;

    public:
    };
#endif

};}; // namespace




#endif /* _INCLUDE__GEM_GEM_THREAD_H_ */
