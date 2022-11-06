#include "asm.h"
#include <assert.h>
#include <stdio.h>

int main() {
  char s[] = "whatever", l[] = "i", *ds = &l[0], *sc = &s[0];
  asm_jmp_buf buf;
  int r = asm_setjmp(buf);
  if (r == 0) {
    assert(asm_add(1234, 5678) == 6912);
    printf("%s\n", (char *)asm_memcpy(ds, sc, 6));
    assert(asm_popcnt(0x0123456789abcdefULL) == 32);
    assert(asm_popcnt(0xefULL) == 7);
    // TODO: add more tests here.
    asm_longjmp(buf, 123);
  } else {
    assert(r == 123);
    printf("PASSED.\n");
  }
}
