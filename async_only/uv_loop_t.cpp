#include "uv_loop_t.hpp"
#include <exception>
#include <stdexcept>
#include <queue>
#include <sys/epoll.h>
#include <cerrno>
#include <sys/ioctl.h>
#include <sys/eventfd.h>
#include <cassert>
#include <cstring>

using namespace XX;
using namespace std;

uv_loop_t::uv_loop_t() :
  epoll_fd(init_epoll()), async_watcher(this, UV_ASYNC){
  #ifndef __linux__
    throw runtime_error("not int linux")
  #endif
  if(epoll_fd == -1) 
    throw runtime_error("epoll error");
  watcher_queue.push(&async_watcher);
  watchers[async_watcher.getfd()] = &async_watcher;
}

void uv_loop_t::run() {
  int r = 0;
  //....省略其他部分的细节
  do {
    //...省略
    uv_epoll();
    //...省略
  } while(r != 0);
}

int uv_loop_t::init_epoll() {
  int fd = 0;
  fd = epoll_create1(EPOLL_CLOEXEC);
  if(fd == -1 && (errno == ENOSYS || errno == EINVAL)) {
    fd = epoll_create(244);
    if(fd != -1) {
      int status = -1;
      do {
        status = ioctl(fd, FIOCLEX);
      } while(status == -1 && errno == EINTR);
      if(status == -1) 
        return -1;
    }
  }
  return fd;
}

std::queue<uv_async_t*>& uv_loop_t::getAsyncQueue() {
  return async_queue;
}

void uv_loop_t::uv_epoll() {
  assert(epoll_fd > 0);
  struct epoll_event events[MAX_WATCHERS];

  while(!watcher_queue.empty()) {
    auto io = watcher_queue.front();
    watcher_queue.pop();
    epoll_event e;
    ::memset(&e, 0, sizeof e);
    e.events = EPOLLIN; //省略其他情况
    e.data.fd = io->getfd();
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, io->getfd(), &e);
  }

  int r = 0;
  do {
    r = epoll_wait(epoll_fd, events, MAX_WATCHERS, -1);
  } while (r == -1 && errno == EINTR);
  for(int i = 0; i < r; ++i) {
    auto watcher_1 = watchers[events[i].data.fd];
    watcher_1->work();
    //其他处理省略
  }
}

void uv_loop_t::push_async(uv_async_t* asy) {
  async_queue.push(asy);
}