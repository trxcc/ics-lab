#include <stdint.h>

static const uint64_t N = (unsigned)9223372036854775808;

static uint64_t mod(uint64_t a, uint64_t b) {
  while (a >= b) {
    a -= b;
  }
  return a;
}

static inline uint64_t Constant(uint64_t m) {
  return mod((mod(N, m) + mod(N, m)), m);
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
