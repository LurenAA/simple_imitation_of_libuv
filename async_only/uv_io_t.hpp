#ifndef __UV_ASYNC_IO_T
#define __UV_ASYNC_IO_T

namespace XX{
  enum uv_type_t {
    UV_ASYNC,
  };
  class uv_loop_t;
  class uv_io_t {
    public: 
      int getfd() {return fd;}
      void work();
      uv_io_t(uv_loop_t* lp, uv_type_t);
      uv_io_t(const uv_io_t&) = delete;
      uv_io_t& operator=(const uv_io_t&) = delete;
    private:
      /*private members*/
      uv_loop_t* loop;
      uv_type_t type; 
      int fd; //eventfd
      /*tool functions*/
      int init_eventfd();
  };
}
#endif