/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Synchedworkerthread.h
       - part of GEM
       - a worker thread that automatically dequeues in the main thread

    Copyright (c) 2011 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_SYNCHEDWORKERTHREAD_H_
#define INCLUDE_GEM_SYNCHEDWORKERTHREAD_H_

#include "Gem/WorkerThread.h"

namespace gem { namespace thread {
    GEM_EXTERN class SynchedWorkerThread : public WorkerThread {
		  private:
      class PIMPL;
      PIMPL*m_pimpl;
      friend class PIMPL;
		  public:
      SynchedWorkerThread(void);
      virtual ~SynchedWorkerThread(void);

		  protected:
      // this get's called from the main thread(!) with each
      // finished data chunk
      virtual void done(id_t ID, void*data) = 0;

      //////
      // tell RTE to call back asap
      virtual void signal(void);

    };};};


#endif /* INCLUDE_GEMSYNCHEDWORKERTHREAD_H_ */
