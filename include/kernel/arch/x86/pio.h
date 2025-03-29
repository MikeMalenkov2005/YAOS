#ifndef KERNEL_ARCH_X86_PIO_H
#define KERNEL_ARCH_X86_PIO_H

#include <types.h>

inline static void WritePort8(UINT16 nPort, UINT8 nData)
{
  asm volatile ("outb %b0, %w1" : : "a"(nData), "Nd"(nPort) : "memory");
}

inline static void WritePort16(UINT16 nPort, UINT16 nData)
{
  asm volatile ("outw %w0, %w1" : : "a"(nData), "Nd"(nPort) : "memory");
}

inline static void WritePort32(UINT16 nPort, UINT32 nData)
{
  asm volatile ("outl %0, %w1" : : "a"(nData), "Nd"(nPort) : "memory");
}

inline static UINT8 ReadPort8(UINT16 nPort)
{
  UINT8 nData;
  asm volatile ("inb %w1, %b0" : "=a"(nData) : "Nd"(nPort) : "memory");
  return nData;
}

inline static UINT16 ReadPort16(UINT16 nPort)
{
  UINT16 nData;
  asm volatile ("inw %w1, %w0" : "=a"(nData) : "Nd"(nPort) : "memory");
  return nData;
}

inline static UINT32 ReadPort32(UINT16 nPort)
{
  UINT32 nData;
  asm volatile ("inl %w1, %0" : "=a"(nData) : "Nd"(nPort) : "memory");
  return nData;
}

#endif
