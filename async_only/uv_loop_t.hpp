#ifndef __LOOP__HPP
#define __LOOP__HPP
#include "uv_io_t.hpp"
#include <queue>

#define MAX_WATCHERS 100
namespace XX {
  class uv_async_t;
  class uv_io_t;
  class uv_loop_t {
    public:
      uv_loop_t();
      uv_loop_t(const uv_loop_t&) = delete;
      uv_loop_t& operator=(const uv_loop_t&) = delete;
      int get_event_fd() {return async_watcher.getfd();}
      void run();
      void push_async(uv_async_t*);
      std::queue<uv_async_t*>& getAsyncQueue();
    private:
      /*tool function*/
      int init_epoll();
      void uv_epoll();
      /*private members*/
      int epoll_fd;     
      std::queue<uv_async_t*> async_queue;
      uv_io_t async_watcher;
      std::queue<uv_io_t*> watcher_queue;
      uv_io_t* watchers[MAX_WATCHERS];
  };
}
#endif