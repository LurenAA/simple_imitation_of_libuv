#ifndef __ASYNC__T_
#define __ASYNC__T_
#include <atomic>
#include <unistd.h>

#define ACCESS_ONCE(VAL) \
  (*((volatile decltype(VAL)*)(&VAL)))

namespace XX{
  class uv_loop_t;
  class uv_async_t;
  using async_cb = void(*)(uv_async_t*);
  class uv_async_t {
    public:
      uv_async_t(uv_loop_t*,async_cb);
      async_cb getCb() const {return cb;}
      bool compare_exchange(int exp, int der) {
        if(ACCESS_ONCE(pending) != exp) {
          return false;
        }
        return atomic_compare_exchange_weak(&pending, &exp, der);
      }
      void uv_async_send();
    private:  
      uv_loop_t* loop;
      std::atomic_int32_t pending; 
      async_cb cb;
  };
}

#endif