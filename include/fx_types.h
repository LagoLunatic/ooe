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

#endif /* FX_TYPES_H */
