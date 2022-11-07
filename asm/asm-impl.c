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
    "leaq 8(%%rsp), %%rax;"
    "movq %%rbx, 0(%%rax);"
    //"movq %%rcx, 16(%%rdi);"
    //"movq %%rdx, 24(%%rdi);"
    "movq %%rsi, 8(%%rax);"
    "movq %%rdi, 16(%%rax);"
    "movq %%rbp, 24(%%rax);"
    "movq %%rsp, 32(%%rax);"
    "movq (%%rsp), %%rcx;"
    "movq %%rcx, 40(%%rax)"
    : 
    :   
    : "%rax", "%rcx" 
  ); 
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  asm(
    "cmpl $0x0, %%ecx;"
    "je .long_L1;"
    "leaq 8(%%rsp), %%rax;"
    //"movq 16(%%rsp), %%rcx"
    //"testl %%rcx, %%rcx;"
    //"jne .long_L1;"
    //"movq 0(%%rdi), %%rax;"
    "movq 0(%%rax), %%rbx;"
    //"movq 16(%%rdi), %%rcx;"
    //"movq 24(%%rdi), %%rdx;"
    "movq 8(%%rax), %%rsi;"
    "movq 16(%%rax), %%rdi;"
    "movq 24(%%rax), %%rbp;"
    "movq 32(%%rax), %%rsp;"
    //"movq 40(%%rax), %%rcx;"
    //"jmp *%%rcx;"
    ".long_L1:"
    : "=c"(val)
    : "c"(val)
    : 
  );
  return;
}
