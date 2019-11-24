#include "uv_async_t.hpp"
#include "uv_loop_t.hpp"
#include "uv_io_t.hpp"
#include <cerrno>
#include <cstdlib>

using namespace std;
using namespace XX;

uv_async_t::uv_async_t(uv_loop_t *l,async_cb f) : loop(l), cb(f)
{
  loop->push_async(this);
  atomic_init(&pending, 0);
}

void uv_async_t::uv_async_send()
{
  if (ACCESS_ONCE(pending) != 0)
  {
    return;
  }
  int val = 0;
  if (!atomic_compare_exchange_weak(&pending, &val, 1))
  {
    return;
  }
  uint64_t buf = 1;
  int len = sizeof(buf);
  int r = -1;
  do{
   r =::write(loop->get_event_fd(), &buf, len);
  } while(r == -1 && errno == EINTR);
  if(r == -1) 
    abort();
  val = 1;
  if (!atomic_compare_exchange_weak(&pending, &val, 2))
  {
    return;
  }
}