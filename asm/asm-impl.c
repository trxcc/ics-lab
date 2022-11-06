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
  int s = 0;
  /*for (int i = 0; i < 64; i++) {
    if ((x >> i) & 1) s++;
  }*/
  asm(
    "movl $0x0, %%eax;"
    ".loop:;"
    "cmpl $64, %%eax;"
    "jge .L2;"
    "addl $0x1, %%eax;"
    "movq %2, %%rbx;"
    "shrq $0x1, %2;"
    "andb $0x1, %%bl;"
    "cmpb $0x0, %%bl;"
    "jne .L1;"
    "jmp .loop;"
    ".L1:;"
    "addl $0x1, %1;"
    "jmp .loop;"
    ".L2:;"
    "movl %1, %%eax"
    : "=c"(s)
    : "0"(s), "r"(x)
    : "%esi"
  );
  return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  void *d0 = dest;
  asm(
    //"movq $0x0, %%rax;"
    ".mem_loop:;"
    //"cmpq %%rdx, %%rax;"
    "jge .mem_L2;"
    //"movl (%1, %%rax), %%ebx;"
    //"movl %%ebx, (%0, %%rax);"
    //"addq $0x1, %%rax;"
    //"addq $0x1, %0;"
    //"addq $0x1, %1;"
    //"jmp .mem_loop;"
    ".mem_L2:"
    : 
    : "c"(d0)
  );
  return dest;
}

int asm_setjmp(asm_jmp_buf env) {
  return setjmp(env);
}

void asm_longjmp(asm_jmp_buf env, int val) {
  longjmp(env, val);
}
