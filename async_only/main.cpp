#include "uv_loop_t.hpp"
#include "uv_async_t.hpp"
#include "cstddef"
#include <iostream>
using namespace std;
using namespace XX;
void uv_async_cb_func(uv_async_t *handle) {
  cout << "123456" << endl;
}

int main() {
  uv_loop_t loop;
  uv_async_t asy(&loop, uv_async_cb_func);
  asy.uv_async_send();
  loop.run();
  
  return 0;
}