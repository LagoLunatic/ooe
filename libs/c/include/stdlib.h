#ifndef STDLIB_H
#define STDLIB_H

#include <cmath.h>
#include <stddef.h>

void* malloc(size_t size);
void free(void* ptr);
void* realloc(void* ptr, size_t new_size);
void* calloc(size_t num, size_t size);

int rand();
void srand(unsigned seed);

int atoi(const char* str);
long atol(const char* str);
long long atoll(const char* str);
double atof(const char* str);

void qsort(void* ptr, size_t count, size_t size, int (*comp)(const void*, const void*));

#endif /* STDLIB_H */
