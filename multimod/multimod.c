#include <stdint.h>
#include <stdio.h>
//static const uint64_t N = 9223372036854775808;

uint64_t mod(uint64_t a, uint64_t b) {
  while (a >= b) {
    a -= b;
  }
  return a;
}

uint64_t Constant(uint64_t m) {
  int flag = 64;
  uint64_t t = 1;
  uint64_t ans = 0;
  while(flag--) {
    printf("%d\n", flag);
    ans = mod(mod(ans, m) + mod(t, m), m);
    t <<= 1;
    //t = mod(mod(t, m) + mod(1, m), m);
  }
  return mod(mod(ans, m) + mod(1, m), m);
}

static uint64_t plusmod(uint64_t x, uint64_t y, uint64_t m){
  uint64_t t = x + y;
  return mod((mod(t, m)+Constant(m)), m);
}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  uint64_t t = a + b;
  uint64_t ans = 0, tmp = b;
  while(a != 0)  {
    uint64_t a0 = mod(a, 2);
    if(a0) ans = plusmod(ans, tmp, m);
    tmp <<= 1;
    tmp = mod(tmp, m);
    a >>= 1;
  }
  ans = mod(ans, m);
  return ans;
}
