#ifndef DR_GRAPH_H
#define DR_GRAPH_H

#include "types.h"

typedef struct _DrGraphChara DrGraphChara;
struct _DrGraphChara {
    /* 0x0 */ u8 m0[0x1 - 0x0];
    /* 0x1 */ u8 m1;
    /* 0x2 */ s16 m2;
    /* 0x4 */ u32 m4;
    /* 0x8 */ void* charaData;
};  // Size: 0xC

typedef struct _DrGraphPalette DrGraphPalette;
struct _DrGraphPalette {
    /* 0x0 */ u8 m0[0x1 - 0x0];
    /* 0x1 */ u8 m1;
    /* 0x2 */ u8 m2;
    /* 0x3 */ u8 m3[0x4 - 0x3];
};  // Size: 0x4

typedef struct _DrGraphRect DrGraphRect;
struct _DrGraphRect {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 w;
    /* 0x6 */ s16 h;
};  // Size: 0x8

typedef struct _DrGraphOpdPatHeader DrGraphOpdPatHeader;
struct _DrGraphOpdPatHeader {
    /* 0x0 */ u8 m0[0x2 - 0x0];
    /* 0x2 */ u8 rectCount;
    /* 0x3 */ u8 partCount;
    /* 0x4 */ DrGraphRect* rects;
    /* 0x8 */ void* parts;
};  // Size: 0xC

typedef struct _DrGraphOpdAnimHeader DrGraphOpdAnimHeader;
struct _DrGraphOpdAnimHeader {
    /* 0x0 */ u16 m0;
    /* 0x2 */ u8 m2[0x4 - 0x2];
    /* 0x4 */ void* m4;
};  // Size: 0x8

typedef struct _DrGraphOpd DrGraphOpd;
struct _DrGraphOpd {
    /* 0x0 */ s16 patternCount;
    /* 0x2 */ s16 animCount;
    /* 0x4 */ DrGraphOpdPatHeader* patterns;
    /* 0x8 */ DrGraphOpdAnimHeader* anims;
};  // Size: 0xC

#endif /* DR_GRAPH_H */
