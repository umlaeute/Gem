////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2011 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "SynchedWorkerThread.h"
#include "Gem/RTE.h"
#include "ThreadMutex.h"

namespace gem { namespace thread {

  class SynchedWorkerThread::PIMPL {
  public:
    SynchedWorkerThread*owner;
    t_clock*clock;

    Mutex m_flag;
    bool flag;

    PIMPL(SynchedWorkerThread*x) : owner(x), clock(NULL), flag(false)
    {
      clock=clock_new(this, reinterpret_cast<t_method>(tickCb));
    }
    ~PIMPL(void) {
      if(clock)
	clock_free(clock);
    }

    static void tickCb(void*you) {
      PIMPL*me=reinterpret_cast<PIMPL*>(you);
      me->tick();
    }
    void tick(void) {
      //post("tick");
      id_t ID=WorkerThread::IMMEDIATE;
      void*data=0;
      do {
	if(!owner->dequeue(ID, data))
	  break;

	owner->done(ID, data);
      } while(true);

      m_flag.lock();
      flag=false;
      m_flag.unlock();
    }

    void tack(void) {
      //post("signal tack");
      m_flag.lock();
      bool done=flag;
      flag=true;
      m_flag.unlock();

      // already flagged
      if(done)return;

      sys_lock();
      clock_delay(clock, 0);
      sys_unlock();
    }

  };


  SynchedWorkerThread::SynchedWorkerThread(void) :
    m_pimpl(new PIMPL(this)) {
    start();
  }

  SynchedWorkerThread::~SynchedWorkerThread(void) {
    delete m_pimpl;
    m_pimpl=0;
  }



  void SynchedWorkerThread::signal(void) {
    m_pimpl->tack();
  }

};}; // } thread } gem
