#include "uv_io_t.hpp"
#include <sys/eventfd.h>
#include <cerrno>
#include <stdexcept>
#include "uv_loop_t.hpp"
#include "uv_async_t.hpp"

using namespace std;
using namespace XX;

uv_io_t::uv_io_t(uv_loop_t* lp , uv_type_t tp) :
  loop(lp), type(tp), fd(init_eventfd())
{ 
  if(fd == -1)
    throw runtime_error("eventfd error");
}

int uv_io_t::init_eventfd() {
  int fd = -1;
  do {
    fd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
  } while(fd == -1 && errno == EINTR);
  return fd;
}

void uv_io_t::work() {
  if(type == UV_ASYNC) {
    std::queue<XX::uv_async_t *>& q = loop->getAsyncQueue();
    std::queue<XX::uv_async_t *> q2;
    while (!q.empty())
    {
      auto qf = q.front();
      q.pop();
      if(qf->compare_exchange(2, -1) == true) {
        qf->getCb()(qf);
      } else if(qf->compare_exchange(1, -1) == true) {
        //等待，具体做法汇编没看懂
      } else {
        q2.push(qf);
      }
    }
    q.swap(q2);
  }  //其他类型....
}