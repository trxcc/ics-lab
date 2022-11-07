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
    "movq $0x0, %%rax;"
    ".mem_loop:;"
    "cmpq %%rdx, %%rax;"
    "jge .mem_L2;"
    "movb (%%rsi), %%bl;"
    "movb %%bl, (%%rcx);"
    "addq $0x1, %%rax;"
    "addq $0x1, %0;"
    "addq $0x1, %%rsi;"
    "jmp .mem_loop;"
    ".mem_L2:"
    : 
    : "c"(d0)
    : "%rax" 
  );
  return dest;
}

int asm_setjmp(asm_jmp_buf env) {
  asm(
    //"movq %%rax, 0(%%rdi);"
    "movq %%rbx, 8(%%rdi);"
    //"movq %%rcx, 16(%%rdi);"
    //"movq %%rdx, 24(%%rdi);"
    "movq %%rsi, 32(%%rdi);"
    "movq %%rbp, 40(%%rdi);"
    "leaq 8(%%rsp), %%rax;"
    "movq %%rax, 48(%%rdi);"
    "movq (%%rsp), %%rax;"
    "movq %%rax, 56(%%rdi)"
    : 
    :
    : 
  ); 
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  asm(
    "testl %%esi, %%esi;"
    "jne .long_L1;"
    //"movq 0(%%rdi), %%rax;"
    "movq 8(%%rdi), %%rbx;"
    //"movq 16(%%rdi), %%rcx;"
    //"movq 24(%%rdi), %%rdx;"
    "movq 32(%%rdi), %%rsi;"
    "movq 40(%%rdi), %%rbp;"
    "movq 48(%%rdi), %%rsp;"
    "movq 56(%%rdi), %%rdi;"
    "jmp *(%%rdi);"
    ".long_L1:"
    :
    :
    :
  );
  return;
}
