#include <cpu.h>

#include <attributes.h>

void __naked lcs(uint16_t cs) {
  asm volatile ("pop %ecx");
  asm volatile ("mov (%esp), %eax");
  asm volatile ("push %eax");
  asm volatile ("push %ecx");
  asm volatile ("retf");
}
