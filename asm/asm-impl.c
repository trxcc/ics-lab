#include "asm.h"
#include <string.h>
#include <stdio.h>
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
  int s = 0, i = 0;
  /*for (int i = 0; i < 64; i++) {
    if ((x >> i) & 1) s++;
  }*/
  //printf("%lx\n", x);
  asm(
    "movl $0x0, %%eax;"
    ".loop:;"
    "cmpl $64, %%eax;"
    "jge .L2;"
    "addl $0x1, %%eax;"
    "shrq $0x1, %2;"
    "movq %3, %%rbx;"
    "andb $0x1, %%bl;"
    "cmpb $0x0, %%bl;"
    "jg .L1;"
    "jmp .loop;"
    ".L1:;"
    "addl $0x1, %2;"
    "jmp .loop;"
    ".L2:;"
    "movl %2, %%eax"
    : "=c"(s), "=a"(i)
    : "0"(s), "r"(x), "1"(i)
    : "%esi"
  );
  printf("i: %d\n", i);
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
