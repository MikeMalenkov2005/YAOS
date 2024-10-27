#include <tss.h>
#include <gdt.h>

#include <attributes.h>

struct tss_struct TSS __aligned(16);

struct tss_struct* init_tss(struct interrupt_frame* root_frame) {
  for (char* p = (void*)&TSS; p < (char*)(&TSS + 1); *p++ = 0);
  TSS.sp0 = (size_t)(root_frame + 1);
  return &TSS;
}

int is_root_frame(struct interrupt_frame* frame) {
  return TSS.sp0 - sizeof(*frame) <= (size_t)frame;
}

