#include "asm.h"
#include <string.h>
#include <stdio.h>
int64_t asm_add(int64_t a, int64_t b) {
  asm(
    "addq %1, %2"
    : "=a"(b)
    : "b"(a), "a"(b)
    : 
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
    "movl $0x0, %%ecx;"
    ".loop:;"
    "cmpl $64, %%eax;"
    "jae .L2;"
    "addl $0x1, %%eax;"
    "testq $0x1, %%rdx;"
    "jz .L1;"
    "addl $0x1, %%ecx;"
    //"movq %%rdx, %%rbx;"
    //"andb $0x1, %%bl;"
    //"cmpb $0x0, %%bl;"
    //"jne .L1;"
    //"jmp .loop;"
    ".L1:;"
    "shrq $0x1, %%rdx;"
    "jmp .loop;"
    ".L2:;"
    //"movl %1, %%eax"
    : "+c"(s)
    : "d"(x)
    : 
  );
  return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  //void *d0 = dest;
  asm(
    "movq $0x0, %%rax;"
    ".mem_loop:;"
    "cmpq %%rdx, %%rax;"
    "jae .mem_L2;"
    "movb (%%rsi, %%rax), %%cl;"
    "movb %%cl, (%%rdi, %%rax);"
    "addq $0x1, %%rax;"
    //"addq $0x1, %%rcx;"
    //"addq $0x1, %%rsi;"
    "jmp .mem_loop;"
    ".mem_L2:"
    : 
    : 
    : "%rax" 
  );
  return dest;
}

int asm_setjmp(asm_jmp_buf env) {
  int val;
  asm(
    //"leaq 8(%%rsp), %%rax;"
    "movq %%rdi, %%rax;"
    "movq %%rbx, 0(%%rax);"
    "movq %%rsi, 8(%%rax);"
    "movq %%rdi, 16(%%rax);"
    "movq %%rbp, 24(%%rax);"
    "movq %%rsp, 32(%%rax);"
    "movq (%%rsp), %%rcx;"
    "movq %%rcx, 40(%%rax);"
    "subl %%eax, %%eax"
    : "=a"(val)
    :   
    : "%rcx" 
  ); 
  return val;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  asm(
    "cmpl $0x0, %%ecx;"
    "je .long_L1;"
    //"leaq 8(%%rsp), %%rax;"
    "movq %%rdi, %%rax;"
    "movq 0(%%rax), %%rbx;"
    "movq 8(%%rax), %%rsi;"
    "movq 16(%%rax), %%rdi;"
    "movq 24(%%rax), %%rbp;"
    "movq 32(%%rax), %%rsp;"
    "movq 40(%%rax), %%rdx;"
    "movq %%rdx, (%%rsp);"
    "movl %%ecx, %%eax;"
    ".long_L1:"
    : 
    : "c"(val)
    : 
  );
  return;
}
