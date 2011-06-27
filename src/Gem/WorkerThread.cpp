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

#include "WorkerThread.h"
#include <queue>

#include "ThreadMutex.h"
#include "ThreadSemaphore.h"

#include <pthread.h>
#include <unistd.h>


namespace gem { namespace thread {

  const WorkerThread::id_t WorkerThread::IMMEDIATE =  0;
  const WorkerThread::id_t WorkerThread::INVALID   = ~0;

  class WorkerThread::PIMPL {
  public:
    WorkerThread*owner;
    WorkerThread::id_t ID;

    bool keeprunning;
    bool isrunning;

    std::queue< std::pair<WorkerThread::id_t, void*> > q_todo;
    std::queue< std::pair<WorkerThread::id_t, void*> > q_done;
    Mutex m_todo;
    Mutex m_done;
    Semaphore s_newdata;

    WorkerThread::id_t cancelledID;

    pthread_t p_thread;

    PIMPL(WorkerThread*x) : owner(x), ID(1),
			    keeprunning(true), isrunning(false),
			    cancelledID(WorkerThread::INVALID)
    {

    }
    ~PIMPL(void) {

    }

    inline WorkerThread::id_t nextID(void) {
      ID++;
      while(ID == WorkerThread::IMMEDIATE || ID == WorkerThread::INVALID)
	ID++;
      return ID;
    }

    static inline void*process(void*you) {
      PIMPL*me=reinterpret_cast<PIMPL*>(you);
      WorkerThread*wt=me->owner;
      me->isrunning=true;
      std::pair <id_t, void*> in, out;

      while(me->keeprunning) {
	// wait till we are signalled new data

	me->m_todo.lock();
	if(me->q_todo.empty()) {
	  me->m_todo.unlock();
	  me->s_newdata.freeze();

	  // either new data has arrived or we are told to stop
	  if(!me->keeprunning)
	    break;

	  me->m_todo.lock();
	}
	in=me->q_todo.front();
	me->q_todo.pop();
	me->m_todo.unlock();

	out.first = in.first;
	out.second=wt->process(in.first, in.second);

	me->m_done.lock();
	if(me->cancelledID != out.first) {
	  me->q_done.push(out);
	}
	me->m_done.unlock();
      }
      me->isrunning=false;
      return 0;
    }

    bool start(void) {
      if(isrunning)return true;
      
      keeprunning=true;
      pthread_create(&p_thread, 0, process, this);

      while(!isrunning)
	usleep(10);

      return true;

    }
    bool stop(void) {
      if(!isrunning)return true;

      keeprunning=false;
      s_newdata.thaw();
    }

  };


  WorkerThread::WorkerThread(void) :
    m_pimpl(new PIMPL(this)) {

  }

  WorkerThread::~WorkerThread(void) {
    delete m_pimpl;
    m_pimpl=0;
  }
  bool WorkerThread::start(void) {
    return m_pimpl->start();
  }
  bool WorkerThread::stop(void) {
    return m_pimpl->stop();
  }



  WorkerThread::id_t WorkerThread::queue(void*data) {
    std::pair <id_t, void*> DATA;
    DATA.second = data;

    m_pimpl->m_todo.lock();
    id_t ID=m_pimpl->nextID();
    DATA.first = ID;
    m_pimpl->q_todo.push(DATA);
    m_pimpl->m_todo.unlock();

    m_pimpl->s_newdata.thaw();
    return ID;
  }
  bool WorkerThread::cancel(WorkerThread::id_t ID) {
    return false;
  }
  WorkerThread::id_t WorkerThread::dequeue(void**data) {
    std::pair <id_t, void*> DATA;
    DATA.first=WorkerThread::INVALID;
    DATA.second=0;

    m_pimpl->m_done.lock();
    if(!m_pimpl->q_done.empty()) {
      m_pimpl->q_done.front();
      m_pimpl->q_done.pop();
    }
    m_pimpl->m_done.unlock();

    id_t ID=DATA.first;
    *data=DATA.second;

    return ID;
  }

  void WorkerThread::signal(void) {
    // nada
  }

};}; // } thread } gem
