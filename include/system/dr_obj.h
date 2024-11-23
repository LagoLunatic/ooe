#ifndef DR_OBJ_H
#define DR_OBJ_H

#include "types.h"

typedef struct DrObj {
    u8 _000[0x30 - 0x00];
    Vec pos;
    u8 _03C[0xD8 - 0x3C];
    u32 _0D8;
    u32 _0DC;
    u32 _0E0;
    u8 _0E4[0x13C - 0xE4];
    u16 _13C;
} DrObj;

#endif /* DR_OBJ_H */
