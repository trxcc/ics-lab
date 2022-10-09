#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
uint64_t multimod(uint64_t, uint64_t, uint64_t);
extern uint64_t mod(uint64_t, uint64_t);
extern uint64_t Constant(uint64_t);
void test(uint64_t a, uint64_t b, uint64_t m) {
  #define U64 "%" PRIu64
  printf(U64 "\n", Constant(7));
  assert(0);
  printf(U64 " * " U64 " mod " U64 " = " U64 "\n", a, b, m, multimod(a, b, m));
}

int main() {
  test(10, 10, 32);
  test(123, 456, -1ULL);
  //test(-2ULL, -2ULL, -1ULL); // should be 1
}
