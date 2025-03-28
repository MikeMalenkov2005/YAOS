#include <kernel/arch/x86/task.h>
#include <kernel/arch/x86/pio.h>
#include <kernel/syscall.h>
#include <kernel/timer.h>
#include <kernel/panic.h>
#include <attrib.h>
#include <types.h>

extern void DebugPrint(UINT8 Prefix, UINTPTR Page);

inline static UINTPTR ReadCR2()
{
  UINTPTR CR2;
  asm volatile ("mov %%cr2, %0" : "=a"(CR2));
  return CR2;
}

void HandleInterrupt(INTERRUPT_FRAME Frame)
{
  /* TODO: Handle MORE Interrupts */
  SetTaskFrame(&Frame);
  if (Frame.ISRIndex < 32)
  {
    DebugPrint('E', Frame.ISRIndex);
    DebugPrint('C', Frame.ErrorCode);
    DebugPrint('R', ReadCR2());
    DebugPrint('I', Frame.EIP);
    KernelPanic("EXCEPTION");
  }
  if (Frame.ISRIndex == 32) HandleTimerTick();
  if (Frame.ISRIndex == 128) HandleSystemCall((void*)&Frame.EAX, Frame.EAX,
      (SYSCALL_ARGUMENTS){Frame.EBX, Frame.ECX, Frame.EDX});
  if (Frame.ISRIndex >= 32 && Frame.ISRIndex < 48)
  {
    if (Frame.ISRIndex >= 40)
    {
      WritePort8(0xA0, 0x20);
    }
    WritePort8(0x20, 0x20);
  }
}

void __naked isr()
{
  asm volatile ("pusha");
  asm volatile ("mov %ds, %ax");
  asm volatile ("push %eax");
  asm volatile ("mov $0x10, %ax");
  asm volatile ("mov %ax, %ds");
  asm volatile ("mov %ax, %es");
  asm volatile ("mov %ax, %fs");
  asm volatile ("mov %ax, %gs");
  asm volatile ("call %P0" : : "i"(HandleInterrupt));
  asm volatile ("pop %eax");
  asm volatile ("mov %ax, %ds");
  asm volatile ("mov %ax, %es");
  asm volatile ("mov %ax, %fs");
  asm volatile ("mov %ax, %gs");
  asm volatile ("popa");
  asm volatile ("add $8, %esp");
  asm volatile ("iret");
}

#define ISR_NOCODE(n)                     \
void __naked isr##n()                     \
{                                         \
  asm volatile ("push %0" : : "N"(0));    \
  asm volatile ("push %0" : : "N"(n));    \
  asm volatile ("jmp %P0" : : "i"(isr));  \
}

#define ISR_WITHCODE(n)                   \
void __naked isr##n()                     \
{                                         \
  asm volatile ("push %0" : : "N"(n));    \
  asm volatile ("jmp %P0" : : "i"(isr));  \
}

ISR_NOCODE(0);
ISR_NOCODE(1);
ISR_NOCODE(2);
ISR_NOCODE(3);
ISR_NOCODE(4);
ISR_NOCODE(5);
ISR_NOCODE(6);
ISR_NOCODE(7);
ISR_WITHCODE(8);
ISR_NOCODE(9);
ISR_WITHCODE(10);
ISR_WITHCODE(11);
ISR_WITHCODE(12);
ISR_WITHCODE(13);
ISR_WITHCODE(14);
ISR_NOCODE(15);
ISR_NOCODE(16);
ISR_WITHCODE(17);
ISR_NOCODE(18);
ISR_NOCODE(19);
ISR_NOCODE(20);
ISR_WITHCODE(21);
ISR_NOCODE(22);
ISR_NOCODE(23);
ISR_NOCODE(24);
ISR_NOCODE(25);
ISR_NOCODE(26);
ISR_NOCODE(27);
ISR_NOCODE(28);
ISR_WITHCODE(29);
ISR_WITHCODE(30);
ISR_NOCODE(31);
ISR_NOCODE(32);
ISR_NOCODE(33);
ISR_NOCODE(34);
ISR_NOCODE(35);
ISR_NOCODE(36);
ISR_NOCODE(37);
ISR_NOCODE(38);
ISR_NOCODE(39);
ISR_NOCODE(40);
ISR_NOCODE(41);
ISR_NOCODE(42);
ISR_NOCODE(43);
ISR_NOCODE(44);
ISR_NOCODE(45);
ISR_NOCODE(46);
ISR_NOCODE(47);
ISR_NOCODE(128);
