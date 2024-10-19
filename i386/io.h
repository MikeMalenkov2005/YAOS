#ifndef IO_H
#define IO_H

#include <stdint.h>

inline static uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile ("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}

inline static void outb(uint16_t port, uint8_t val) {
  asm volatile ("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

inline static uint16_t inw(uint16_t port) {
  uint16_t ret;
  asm volatile ("inw %w1, %w0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}

inline static void outw(uint16_t port, uint16_t val) {
  asm volatile ("outw %w0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

inline static uint32_t inl(uint16_t port) {
  uint32_t ret;
  asm volatile ("inl %w1, %0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}

inline static void outl(uint16_t port, uint32_t val) {
  asm volatile ("outl %0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

#endif
