//tcgs_memory.h

#ifndef _TCGS_MEMORY_H
#define _TCGS_MEMORY_H

#include <stddef.h>

extern inline void *allocate(size_t size);
extern inline void *callocate(size_t size);

#endif //_TCGS_MEMORY_H
