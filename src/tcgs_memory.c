//tcgs_memory.c

#include "tcgs_memory.h"
#include <stdlib.h>

inline void *allocate(size_t size)
{
	return malloc(size);
}

inline void *callocate(size_t size)
{
	return calloc(1, size);
}
