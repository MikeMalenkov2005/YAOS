#include <cpu.h>

__attribute__((naked))
void lcs(uint16_t cs) {
  asm volatile ("pop %ecx");
  asm volatile ("mov (%esp), %eax");
  asm volatile ("push %eax");
  asm volatile ("push %ecx");
  asm volatile ("retf");
}
