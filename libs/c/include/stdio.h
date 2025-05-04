#ifndef STDIO_H
#define STDIO_H

#include <stddef.h>

#define FILENAME_MAX 256

typedef struct FILE {
    // TODO
} FILE;

int printf(const char* format, ...);
int fprintf(FILE* stream, const char* format, ...);
int sprintf(char* s, const char* format, ...);
int snprintf(char* s, size_t n, const char* format, ...);

int scanf(const char* format, ...);
int fscanf(FILE* stream, const char* format, ...);
int sscanf(const char* buffer, const char* format, ...);

#endif /* STDIO_H */
