#include <kernel/arch/x86/pio.h>

void WritePort8(UINT16 nPort, UINT8 nData)
{
  asm volatile ("outb %b0, %w1" : : "a"(nData), "Nd"(nPort) : "memory");
}

void WritePort16(UINT16 nPort, UINT16 nData)
{
  asm volatile ("outw %w0, %w1" : : "a"(nData), "Nd"(nPort) : "memory");
}

void WritePort32(UINT16 nPort, UINT32 nData)
{
  asm volatile ("outl %0, %w1" : : "a"(nData), "Nd"(nPort) : "memory");
}

UINT8 ReadPort8(UINT16 nPort)
{
  UINT8 nData;
  asm volatile ("inb %w1, %b0" : "=a"(nData) : "Nd"(nPort) : "memory");
  return nData;
}

UINT16 ReadPort16(UINT16 nPort)
{
  UINT16 nData;
  asm volatile ("inw %w1, %w0" : "=a"(nData) : "Nd"(nPort) : "memory");
  return nData;
}

UINT32 ReadPort32(UINT16 nPort)
{
  UINT32 nData;
  asm volatile ("inl %w1, %0" : "=a"(nData) : "Nd"(nPort) : "memory");
  return nData;
}

