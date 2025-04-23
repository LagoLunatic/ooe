#ifndef NITRO_H
#define NITRO_H

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

typedef float f32;
typedef double f64;
typedef volatile f32 vf32;
typedef volatile f64 vf64;

typedef int BOOL;

#define TRUE 1
#define FALSE 0

#include <time.h>

typedef u64 OSTime;

OSTime OS_GetTick();
OSTime func_02023e00(OSTime, int, int);
inline OSTime OS_TicksToMilliSeconds(OSTime ticks) {
    return func_02023e00(ticks * 0x40, 0x82ea, 0);
}

// === TODO (below here are placeholders)

typedef struct OSMutex {
} OSMutex;
typedef struct OSThread {
} OSThread;

typedef int OSIntrMode;

typedef struct FILE {
} FILE;

#define MAC_ALEN 1

#endif /* NITRO_H */
