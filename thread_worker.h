#ifndef RTORRENT_THREAD_WORKER_H
#define RTORRENT_THREAD_WORKER_H

#include "thread_base.h"

#include <rak/priority_queue_default.h>

namespace rpc {
class SCgi;
}

// Check if cacheline aligned with inheritance ends up taking two
// cachelines.

class lt_cacheline_aligned ThreadWorker : public ThreadBase {
public:
  ThreadWorker();
  ~ThreadWorker();

  const char*         name() const { return "worker_rtorrent"; }

  virtual void        init_thread();

  rpc::SCgi*          scgi() { return m_safe.scgi; }
  bool                set_scgi(rpc::SCgi* scgi);
  
  void                set_xmlrpc_log(const std::string& filename);

  static void         start_scgi(ThreadBase* thread);
  static void         msg_change_xmlrpc_log(ThreadBase* thread);

private:
  void                task_touch_log();

  void                change_xmlrpc_log();

  struct lt_cacheline_aligned safe_type {
    safe_type() : scgi(NULL) {}

    rpc::SCgi* scgi;
  };

  safe_type           m_safe;

  // The following types shall only be modified while holding the
  // global lock.
  std::string         m_xmlrpcLog;
};

#endif

