#ifndef DR_OBJ_H
#define DR_OBJ_H

#include "fx_types.h"

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
    /* 0x01E */ u8 m01E[0x021 - 0x01E];
    /* 0x021 */ u8 opacity;
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
    /* 0x068 */ u8 m068[0x06A - 0x068];
    /* 0x06A */ s16 m06A;
    /* 0x06C */ s16 opdIdx;
    /* 0x06E */ u8 m06E[0x074 - 0x06E];
    /* 0x074 */ uint m074;
    /* 0x078 */ u8 m078[0x07C - 0x078];
    /* 0x07C */ s16 m07C;
    /* 0x07E */ u8 m07E[0x080 - 0x07E];
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
    /* 0x0D0 */ void* m0D0;
    /* 0x0D4 */ u16 m0D4;
    /* 0x0D6 */ u16 m0D6;
    /* 0x0D8 */ uint m0D8;
    /* 0x0DC */ uint m0DC;
    /* 0x0E0 */ u32 m0E0;
    /* 0x0E4 */ uint m0E4;
    /* 0x0E8 */ u8 m0E8[0x0E9 - 0x0E8];
    /* 0x0E9 */ u8 m0E9;
    /* 0x0EA */ u8 m0EA[0x0EC - 0x0EA];
    /* 0x0EC */ u8 m0EC;
    /* 0x0ED */ u8 m0ED;
    /* 0x0EE */ u8 m0EE[0x0F8 - 0x0EE];
    /* 0x0F8 */ int m0F8;
    /* 0x0FC */ u8 m0FC[0x100 - 0x0FC];
    /* 0x100 */ s16 m100;
    /* 0x102 */ s16 m102;
    /* 0x104 */ uint m104;
    /* 0x108 */ u8 m108[0x10C - 0x108];
    /* 0x10C */ uint m10C;
    /* 0x110 */ s8 m110;
    /* 0x111 */ s8 m111;
    /* 0x112 */ s8 m112;
    /* 0x113 */ u8 m113[0x114 - 0x113];
    /* 0x114 */ s8 m114;
    /* 0x115 */ s8 m115;
    /* 0x116 */ s16 m116;
    /* 0x118 */ s16 m118;
    /* 0x11A */ u8 m11A[0x11C - 0x11A];
    /* 0x11C */ u32 m11C;
    /* 0x120 */ u32 m120;
    /* 0x124 */ u16 m124;
    /* 0x126 */ u8 m126[0x128 - 0x126];
    /* 0x128 */ s16 m128;
    /* 0x12A */ u8 m12A[0x12C - 0x12A];
    /* 0x12C */ int m12C;
    /* 0x130 */ u8 m130[0x132 - 0x130];
    /* 0x132 */ u8 m132;
    /* 0x133 */ u8 m133;
    /* 0x134 */ u32 m134;
    /* 0x138 */ s16 m138;
    /* 0x13A */ u8 m13A[0x13C - 0x13A];
    /* 0x13C */ u16 m13C;
    /* 0x13E */ u16 m13E;
    /* 0x140 */ u32 m140;
    /* 0x144 */ s16 m144;
    /* 0x146 */ s16 m146;
    /* 0x148 */ u16 m148;
    /* 0x14A */ u8 m14A[0x14B - 0x14A];
    /* 0x14B */ u8 m14B;
    /* 0x14C */ u8 m14C[0x150 - 0x14C];
    /* 0x150 */ void* m150;
    /* 0x154 */ u8 m154[0x158 - 0x154];
    /* 0x158 */ u8* enebuf;
    /* 0x15C */ int m15C;
};  // Size: 0x160

void DrObjHit_Init(DrObj*, int, DrObjHitCBFunc hitAttackCB, DrObjHitCBFunc damageCB);
void DrObj_DummyFunc(DrObj*);
DrObj* DrObj_ObjGetNextWork(int, int, DrObjFunc);

#endif /* DR_OBJ_H */
