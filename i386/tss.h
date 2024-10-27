#ifndef TSS_H
#define TSS_H

#include <cpu.h>

inline static void ltr(uint16_t selector) {
  asm volatile ("ltr %w0" : : "r"(selector));
}

struct tss_struct {
  uint32_t link;
  uint32_t sp0;
  uint32_t ss0;
  uint32_t sp1;
  uint32_t ss1;
  uint32_t sp2;
  uint32_t ss2;
  uint32_t sp3;
  uint32_t ss3;
  uint32_t ip;
  uint32_t flags;
  uint32_t ax;
  uint32_t cx;
  uint32_t dx;
  uint32_t bx;
  uint32_t sp;
  uint32_t bp;
  uint32_t si;
  uint32_t di;
  uint32_t es;
  uint32_t cs;
  uint32_t ss;
  uint32_t ds;
  uint32_t fs;
  uint32_t gs;
  uint32_t ldtr;
  uint16_t tflag;
  uint16_t iopb;
};

struct tss_struct* init_tss(struct interrupt_frame* root_frame);

int is_root_frame(struct interrupt_frame* frame);

#endif
