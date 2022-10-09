#include <stdint.h>
#include <stdio.h>

const uint64_t N = -1ULL;

uint64_t mod(uint64_t a, uint64_t b) {
  if(a < b) return a;
  return mod(mod(a >> 1, b) << 1 + mod(a & 1, b), b);
}

uint64_t Constant(uint64_t m) {
  int flag = 64;
  uint64_t t = 1;
  uint64_t ans = 0;
  while(flag--) {
    ans = mod(mod(ans, m) + mod(t, m), m);
    t <<= 1;
    t = mod(t, m);
  }
  return mod(mod(ans, m) + mod(1, m), m);
}

uint64_t plusmod(uint64_t x, uint64_t y, uint64_t m){
  uint64_t t = x + y;
  if (t < x || t < y) {
    t = plusmod(mod(t, m), Constant(m), m);
  }
  else return mod(t, m);
  return mod(t, m);
}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  uint64_t ans = 0, tmp = b;
  while(a != 0)  {
    uint64_t a0 = a & 1;
    if(a0) ans = plusmod(ans, tmp, m);
    tmp = plusmod(tmp, tmp, m);
    a >>= 1;
  }
  ans = mod(ans, m);
  return ans;
}
