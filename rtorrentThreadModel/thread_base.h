

#ifndef UTILS_THREAD_BASE_H
#define UTILS_THREAD_BASE_H

#include <pthread.h>
#include <sys/types.h>
#include <torrent/utils/thread_base.h>

#include "rak/priority_queue_default.h"
#include "core/poll_manager.h"



class thread_queue_hack;

class ThreadBase : public torrent::thread_base {
public:
  typedef rak::priority_queue_default priority_queue;
  typedef void (*thread_base_func)(ThreadBase*);

  ThreadBase();
  virtual ~ThreadBase();

  priority_queue&     task_scheduler() { return m_taskScheduler; }

  // Throw torrent::shutdown_exception to stop the thread.
  static void         stop_thread(ThreadBase* thread);

  // ATM, only interaction with a thread's allowed by other threads is
  // through the queue_item call.

  void                queue_item(thread_base_func newFunc);

  // Only call this when global lock has been acquired, as it checks
  // ThreadBase::is_main_polling() which is only guaranteed to remain
  // 'false' if global lock keeps main thread from entering polling
  // again.
  //
  // Move to libtorrent some day.
  static void         interrupt_main_polling();

protected:
  int64_t             next_timeout_usec();

  void                call_queued_items();
  virtual void        call_events();

  // TODO: Add thread name.

  // The timer needs to be sync'ed when updated...

  rak::priority_queue_default m_taskScheduler;

  rak::priority_item  m_taskShutdown;

  // Temporary hack to pass messages to a thread. This really needs to
  // be cleaned up and/or integrated into the priority queue itself.
  thread_queue_hack*  m_threadQueue;
};

#endif