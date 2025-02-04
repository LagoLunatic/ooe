#include "game/enemy/es_enemy.h"
#include "game/g_objhit.h"
#include "game/game.h"
#include "global.h"
#include "system/dr_math.h"
#include "system/dr_objhit.h"
#include "system/dr_opd.h"

// TODO: forward declarations
u8 GetFreeAlphaID();
BOOL gObjOpdCopy(DrObj*, DrObj*);

extern DrGraphOpd* p_scare_opd;
extern DrGraphChara t_scare_chara;
extern DrGraphPalette c_scare_pal;

// @fabricatedName
typedef struct Enemy100 Enemy100;
struct Enemy100 {
    /* 0x000 */ DrObjBase;
    /* 0x0D0 */ u32 m0D0;
    /* 0x0D4 */ u32 m0D4;
    /* 0x0D8 */ u8 m0D8[0x0DC - 0x0D8];
    /* 0x0DC */ u16 m0DC;
    /* 0x0DE */ s16 m0DE;
    /* 0x0E0 */ s16 m0E0;
    /* 0x0E2 */ s16 m0E2;
    /* 0x0E4 */ s16 m0E4;
    /* 0x0E6 */ s16 m0E6;
    /* 0x0E8 */ int m0E8;
    /* 0x0EC */ fx32 m0EC;
    /* 0x0F0 */ DrObj* m0F0;
    /* 0x0F4 */ int m0F4;
    /* 0x0F8 */ u8 m0F8[0x132 - 0x0F8];
    /* 0x132 */ u8 m132;
    /* 0x133 */ u8 m133[0x138 - 0x133];
    /* 0x138 */ s16 m138;
    /* 0x13A */ u8 m13A[0x13C - 0x13A];
    /* 0x13C */ u16 m13C;
    /* 0x13E */ u8 m13E[0x160 - 0x13E];
};  // Size: 0x160
STATIC_ASSERT(sizeof(Enemy100) == sizeof(DrObj));

u16 data_ov019_02223980[6] = {2, 0x2F, 0x30, 8, 0x30, 0x18};

static void func_ov019_022038d4(DrObj* obj2) {
    /* Nonmatching */
    s32 temp;
    s64 temp2;
    u16 temp3;
    fx32 xoffs;
    fx32 yoffs;
    Enemy100* obj = (Enemy100*)obj2;
    Enemy100* parent = (Enemy100*)obj->m0F0;
    if (parent->state1 == 4) {
        DrObj_ObjClearWork(obj2);
        return;
    }
    obj->opacity = parent->opacity;
    if (parent->state1 == 1) {
        return;
    }
    if ((parent->m074 & 0x80) != (obj->m074 & 0x80)) {
        obj->m0F4 = 0x10000 - obj->m0F4;
    }
    if ((parent->m074 & 0x80) != 0) {
        obj->m074 |= 0x80;
        if (obj->m0E8 > 0x8000) {
            temp = 0x18000 - obj->m0E8;
        } else {
            temp = 0x8000 - obj->m0E8;
        }
    } else {
        obj->m074 &= ~0x80;
        temp = obj->m0E8;
    }
    temp3 = parent->angle + temp;
    temp2 = obj->m0EC;
    xoffs = (FX_CosIdx(temp3) * temp2) / 0x1000;
    obj->pos.x = parent->pos.x + xoffs;
    yoffs = (FX_SinIdx(temp3) * temp2) / 0x1000;
    obj->pos.y = parent->pos.y + yoffs;

    switch (obj->state1) {
        case 0:
            if (parent->m0DE != 0) {
                obj->state1 = 1;
                obj->m0F4 = 0;
                break;
            }
            // TODO: inline
            if ((obj->m0E4 < 0 && (obj->m074 & 0x80) == 0 && obj->m0E2 == 0) ||
                (obj->m0E4 > 0 && (obj->m074 & 0x80) != 0 && obj->m0E2 == 0) ||
                (obj->m0E4 < 0 && (obj->m074 & 0x80) != 0 && obj->m0E2 == 1) ||
                (obj->m0E4 > 0 && (obj->m074 & 0x80) == 0 && obj->m0E2 == 1))
            {
                obj->m0F4 += (int)(0.05f * (0x2000 - obj->m0F4));
            } else {
                obj->m0F4 -= (int)(0.05f * (obj->m0F4 - 0xE000));
            }
            break;
        case 1:
            obj->state1 = 0;
            // TODO: inline
            if ((obj->m0E4 < 0 && (obj->m074 & 0x80) == 0) ||
                (obj->m0E4 > 0 && (obj->m074 & 0x80) != 0))
            {
                obj->m0F4 = 0;
            } else {
                obj->m0F4 = 0x10000;
            }
            if (obj->m0E0 != 0) {
                if ((DrMath_GetRand() & 3) != 0) {
                    if (obj->m0E2 != 0) {
                        obj->m0E2 = 0;
                    } else {
                        obj->m0E2 = 1;
                    }
                }
                if (obj->m0E2 != 0) {
                    if (obj->m0F4 == 0) {
                        obj->m0F4 = 0x10000;
                    } else {
                        obj->m0F4 = 0;
                    }
                }
            }
            break;
    }
    obj->angle = obj->m0F4;
}

static void func_ov019_02203b8c(DrObj* parent, int r1, int r2, int r3, int r4) {
    DrObj* objtemp;
    Enemy100* obj;
    objtemp = DrObj_ObjGetNextWork(0x7D, 0xE8, func_ov019_022038d4);
    if (objtemp == NULL) {
        return;
    }
    obj = (Enemy100*)objtemp;
    if (!gObjOpdCopy(parent, objtemp)) {
        DrObj_ObjClearWork(objtemp);
        return;
    }

    obj->drawFunc = esEnemyDraw;
    obj->m0E4 = r1;
    obj->m0E6 = r2;
    obj->patternIdx = r4;
    obj->m0F0 = parent;
    obj->m0EC = FX_Sqrt(TO_FX32(r1 * r1 + r2 * r2));
    obj->m0E8 = FX_Atan2Idx(r2, r1);
    obj->m074 |= 0x04;
    obj->angle = 0;
    obj->pos.z = parent->pos.z + r3;

    if (r4 == 3 || r4 == 4) {
        obj->m0E0 = 1;
    } else {
        obj->m0E0 = 0;
    }

    if (parent->m074 & 0x20) {
        obj->m074 |= 0x20;
        obj->m08A = parent->m08A;
    }

    // TODO: inline?
    if ((obj->m0E4 < 0 && (obj->m074 & 0x80) == 0) || (obj->m0E4 > 0 && (obj->m074 & 0x80) != 0)) {
        obj->m0F4 = 0;
    } else {
        obj->m0F4 = 0x10000;
    }

    obj->m020 = GetFreeAlphaID();
    objtemp->opacity = 0x1F;
    func_ov019_022038d4(objtemp);
}

// @fabricatedName
static void Enemy300HitAttackCallBack(DrObj* obj_this, DrObj* obj_other, int r2) {
    esEnemyHitAttackCallBack(obj_this, obj_other, r2);
}

// @fabricatedName
static void Enemy100DamageCallBack(DrObj* obj_this, DrObj* obj_other, int r2) {
    Enemy100* scarecrow = (Enemy100*)obj_this;
    if (esEnemyDamageCallBackSub(obj_this, obj_other, r2)) {
        esEnemyDeadSub(obj_this);
        obj_this->state1 = 1;
        scarecrow->m0D0 = 0;
    }
}

void Enemy100Main(DrObj* obj) {
    /* Nonmatching */
}

void Enemy100Init(DrObj* obj) {
    Enemy100* scarecrow = (Enemy100*)obj;
    if (!DrSetObjOpdCmnFree(obj, p_scare_opd, &t_scare_chara, &c_scare_pal)) {
        return;
    }

    obj->drawFunc = &esEnemyDraw;
    obj->pos.z = 0x5680;
    obj->m018 = 0;
    obj->vel.y = TO_FX32(-2.5f);
    obj->m04C = 0x200;
    esEnemyYukaOrosu(obj, &data_ov019_02223980);
    obj->state1 = 3;
    scarecrow->m0D0 = 0;

    if (scarecrow->m13C == 0) {
        obj->patternIdx = 1;
        obj->m074 |= 0x20;
        obj->m08A = 3;
    } else if (scarecrow->m13C == 1) {
        obj->patternIdx = 6;
        obj->m074 |= 0x20;
        obj->m08A = 4;
    } else if ((DrMath_GetRand() & 3) == 0) {
        obj->patternIdx = 6;
        obj->m074 |= 0x20;
        obj->m08A = 4;
    } else {
        obj->patternIdx = 1;
        obj->m074 |= 0x20;
        obj->m08A = 3;
    }

    obj->m020 = GetFreeAlphaID();
    obj->opacity = 0x1F;
    obj->m074 |= 0x4;
    obj->angle = 0;
    gEnmDir2Player(obj);
    if ((obj->m074 & 0x80)) {
        scarecrow->m0DC = 1;
        obj->vel.x = TO_FX32(1.0f);
    } else {
        scarecrow->m0DC = 0;
        obj->vel.x = TO_FX32(-1.0f);
    }
    esParamSet(obj);
    DrObjHit_Init(obj, 7, &Enemy300HitAttackCallBack, &Enemy100DamageCallBack);
    DrObjHit_SetAttackParam(obj, scarecrow->m138, 0);
    esObjHitParamDamageSet(obj, scarecrow->m132);
    DrObjHit_SetDefaultInvinTimer(obj);
    gObjHit_SetHitOpd_BothBoth(obj);
    Enemy100Main(obj);
    if (obj->patternIdx == 1) {
        func_ov019_02203b8c(obj, -10, -8, 100, 4);
        func_ov019_02203b8c(obj, -12, -8, -100, 3);
        func_ov019_02203b8c(obj, 5, -11, 100, 5);
        func_ov019_02203b8c(obj, 3, -11, -100, 2);
    }
}
