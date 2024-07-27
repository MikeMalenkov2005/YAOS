#ifndef GDT_H
#define GDT_H

#include "i386.h"

void _init_gdt();
void _set_gdt_entry(int index, size_t offset, uint32_t limit, uint8_t access, uint8_t flags);

#endif

