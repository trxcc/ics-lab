#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b) {
  asm(
    "addq %2, %1"
    : "=r"(b)
    : "0"(a), "r"(b)
    : "%eax"
  );
  return b;
}

int asm_popcnt(uint64_t x) {
  int s = 0;
  /*for (int i = 0; i < 64; i++) {
    if ((x >> i) & 1) s++;
  }*/
  asm(
    "movl $0x0, %%eax;"
    "movl $0x40, %%edi;"
    ".loop:;"
    "cmp %%edi, %%eax;"
    "jge .L2;"
    "shrq $0x1, %2;"
    "movq %2, %%rbx;"
    "andb $0x1, %%bl;"
    "cmpb $0x0, %%bl;"
    "jbe .L1;"
    "addl $0x1, %%eax;"
    "jmp .loop;"
    ".L1:;"
    "addl $0x1, %1;"
    ".L2:;"
    "movl %1, %%eax"
    : "=c"(s)
    : "0"(s), "r"(x)
    : "%esi"
  );
  return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  return memcpy(dest, src, n);
}

int asm_setjmp(asm_jmp_buf env) {
  return setjmp(env);
}

void asm_longjmp(asm_jmp_buf env, int val) {
  longjmp(env, val);
}
