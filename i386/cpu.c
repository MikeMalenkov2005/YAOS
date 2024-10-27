#include <cpu.h>

#include <attributes.h>

void __naked lcs(uint16_t cs) {
  asm volatile ("pop %ecx");
  asm volatile ("mov (%esp), %eax");
  asm volatile ("push %eax");
  asm volatile ("push %ecx");
  asm volatile ("retf");
}

size_t __naked rdflags() {
  asm volatile ("pushfl");
  asm volatile ("popl %eax");
  asm volatile ("ret");
}


void __naked wrflags(size_t flags) {
  asm volatile ("mov 4(%esp), %eax");
  asm volatile ("pushl %eax");
  asm volatile ("popfl");
  asm volatile ("ret");
}

