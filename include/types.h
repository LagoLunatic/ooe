#ifndef TYPES_H
#define TYPES_H

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef long long s64;
typedef int s32;
typedef short s16;
typedef char s8;

typedef int BOOL;
#define TRUE 1
#define FALSE 0

typedef struct Vec {
    int x;
    int y;
    int z;
} Vec;

#endif
