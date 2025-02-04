#ifndef FX_TYPES_H
#define FX_TYPES_H

#include "types.h"

typedef s32 fx32;

#define FX32_INT_SHIFT 0xC
#define FX32_INT(x) ((x) / (1 << FX32_INT_SHIFT))
#define TO_FX32(x) ((x) * (1 << FX32_INT_SHIFT))

// @fabricatedName
typedef struct Vec3x32 Vec3x32;
struct Vec3x32 {
    fx32 x;
    fx32 y;
    fx32 z;
};

// @fabricatedName
typedef struct Vec2x32 Vec2x32;
struct Vec2x32 {
    fx32 x;
    fx32 y;
};

extern s16 FX_SinCosTable_[0x2000];
inline s16 FX_SinIdx(u16 r0) {
    return FX_SinCosTable_[((r0 >> 4) * 2 + 0)];
}
inline s16 FX_CosIdx(u16 r0) {
    return FX_SinCosTable_[(r0 >> 4) * 2 + 1];
}

fx32 FX_Sqrt(int);
int FX_Atan2Idx(int, int);
// TODO: typedef for rotation? should probably have "Idx" in the name

#endif /* FX_TYPES_H */
