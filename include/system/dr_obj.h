#ifndef DR_OBJ_H
#define DR_OBJ_H

#include "global.h"

// @fabricatedName
typedef struct Color_s Color_s;
struct Color_s {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
    /* 0x3 */ u8 m3;
};  // Size: 0x4

typedef struct DrObj DrObj;

typedef void (*DrObjFunc)(DrObj*);
typedef void (*DrObjHitCBFunc)(DrObj*, DrObj*, int);

// @fabricatedName
struct DrObj {
    /* 0x000 */ DrObjFunc mainFunc;
    /* 0x004 */ DrObjFunc drawFunc;
    /* 0x008 */ DrObjFunc destructFunc;
    /* 0x00C */ u8 m00C[0x00D - 0x00C];
    /* 0x00D */ u8 state1;
    /* 0x00E */ u8 state2;
    /* 0x00F */ u8 state3;
    /* 0x010 */ s16 m010;
    /* 0x012 */ u16 m012;
    /* 0x014 */ int m014;
    /* 0x018 */ s16 m018;
    /* 0x01A */ s16 m01A;
    /* 0x01C */ s16 m01C;
    /* 0x01E */ u8 m01E[0x020 - 0x01E];
    /* 0x020 */ u8 m020;
    /* 0x021 */ s8 opacity;
    /* 0x022 */ u8 m022[0x024 - 0x022];
    /* 0x024 */ int m024;
    /* 0x028 */ int m028;
    /* 0x02C */ int m02C;
    /* 0x030 */ Vec3x32 pos;
    /* 0x03C */ Vec2x32 vel;
    /* 0x044 */ uint m044;
    /* 0x048 */ int m048;
    /* 0x04C */ int m04C;
    /* 0x050 */ int m050;
    /* 0x054 */ u32 m054;
    /* 0x058 */ int m058;
    /* 0x05C */ u8 m05C[0x064 - 0x05C];
    /* 0x064 */ s16 angle;
    /* 0x066 */ s16 m066;
    /* 0x068 */ s16 m068;
    /* 0x06A */ s16 m06A;
    /* 0x06C */ s16 opdId;
    /* 0x06E */ u8 m06E[0x074 - 0x06E];
    /* 0x074 */ uint m074;
    /* 0x078 */ u8 m078[0x07C - 0x078];
    /* 0x07C */ s16 m07C;
    /* 0x07E */ s16 m07E;
    /* 0x080 */ u16 m080;
    /* 0x082 */ u8 m082[0x084 - 0x082];
    /* 0x084 */ s16 patternIdx;
    /* 0x086 */ s16 m086;
    /* 0x088 */ s16 m088;
    /* 0x08A */ s16 m08A;
    /* 0x08C */ s16 m08C;
    /* 0x08E */ u16 polygonAttr;
    /* 0x090 */ Color_s upperLeftColor;
    /* 0x094 */ Color_s lowerLeftColor;
    /* 0x098 */ Color_s lowerRightColor;
    /* 0x09C */ Color_s upperRightColor;
    /* 0x0A0 */ s16 opdOffsetX;
    /* 0x0A2 */ s16 opdOffsetY;
    /* 0x0A4 */ u8 m0A4[0x0A6 - 0x0A4];
    /* 0x0A6 */ u8 m0A6;
    /* 0x0A7 */ u8 m0A7;
    /* 0x0A8 */ u16 m0A8;
    /* 0x0AA */ u8 m0AA[0x0AC - 0x0AA];
    /* 0x0AC */ u32 m0AC;
    /* 0x0B0 */ u8 m0B0[0x0B8 - 0x0B0];
    /* 0x0B8 */ u16 m0B8;
    /* 0x0BA */ s16 m0BA;
    /* 0x0BC */ s16 m0BC;
    /* 0x0BE */ s16 m0BE;
    /* 0x0C0 */ u8 m0C0[0x0C8 - 0x0C0];
    /* 0x0C8 */ DrObjHitCBFunc m0C8;
    /* 0x0CC */ DrObjHitCBFunc m0CC;
};  // Size: 0x0D0

// @fabricatedName
typedef struct DrObjGeneric DrObjGeneric;
struct DrObjGeneric {
    /* 0x000 */ DrObj base;
    /* 0x0D0 */ u8 m0D0[0x160 - 0x0D0];
};  // Size: 0x160

void DrObjHit_Init(DrObj*, int, DrObjHitCBFunc hitAttackCB, DrObjHitCBFunc damageCB);
void DrObj_DummyFunc(DrObj*);
DrObj* DrObj_ObjGetNextWork(int, int, DrObjFunc);
void DrObj_ObjClearWork(DrObj*);

#endif /* DR_OBJ_H */
