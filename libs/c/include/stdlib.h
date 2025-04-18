#ifndef STDLIB_H
#define STDLIB_H

#include <stddef.h>

void qsort(void* ptr, size_t count, size_t size, int (*comp)(const void*, const void*));

#endif /* STDLIB_H */
