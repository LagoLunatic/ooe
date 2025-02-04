#ifndef DR_3DBG_H
#define DR_3DBG_H

#include "global.h"

typedef struct ScrollWork_s ScrollWork_s;
struct ScrollWork_s {
    /* 0x00 */ void* m00;
    /* 0x04 */ u8 m04[0x38 - 0x04];
    /* 0x38 */ u16 width;
    /* 0x3A */ u16 height;
    /* 0x3C */ s16 m3C;
    /* 0x3E */ u8 m3E[0x40 - 0x3E];
    /* 0x40 */ int m40;
    /* 0x44 */ int m44;
    /* 0x48 */ int m48;
    /* 0x4C */ u8 m4C[0x70 - 0x4C];
};  // Size: 0x70

extern ScrollWork_s* Dr3DBg_GetScrollWork_SetNo(int);

#endif /* DR_3DBG_H */
