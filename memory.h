#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

extern size_t _get_sys_mem_map();
extern void _set_mem_map(size_t map);
extern size_t _get_mem_map();

extern size_t _get_page_count();
extern size_t _get_page_size();

extern size_t _get_kernel_file_size();
extern size_t _get_kernel_memory_size();

#endif

