#include "game/enemy/es_enemy.h"
#include "game/g_cmn.h"
#include "game/g_objhit.h"
#include "game/game.h"
#include "global.h"
#include "system/dr_math.h"
#include "system/dr_objhit.h"
#include "system/dr_opd.h"

extern DrGraphOpd* p_scare_opd;
extern DrGraphChara t_scare_chara;
extern DrGraphPalette c_scare_pal;

// @fabricatedName
typedef struct Enemy100 Enemy100;
struct Enemy100 {
    /* 0x000 */ DrObj base;
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
    /* 0x0F0 */ Enemy100* m0F0;
    /* 0x0F4 */ int m0F4;
    /* 0x0F8 */ u8 m0F8[0x132 - 0x0F8];
    /* 0x132 */ u8 m132;
    /* 0x133 */ u8 m133[0x138 - 0x133];
    /* 0x138 */ s16 m138;
    /* 0x13A */ u8 m13A[0x13C - 0x13A];
    /* 0x13C */ u16 m13C;
    /* 0x13E */ u8 m13E[0x160 - 0x13E];
};  // Size: 0x160
STATIC_ASSERT(sizeof(Enemy100) == sizeof(DrObjGeneric));

u16 data_ov019_02223980[6] = {2, 0x2F, 0x30, 8, 0x30, 0x18};

static void func_ov019_022038d4(Enemy100* this) {
    /* Nonmatching */
    s32 temp;
    s64 temp2;
    u16 temp3;
    fx32 xoffs;
    fx32 yoffs;
    Enemy100* parent = this->m0F0;
    if (parent->base.state1 == 4) {
        DrObj_ObjClearWork(super);
        return;
    }
    super->opacity = parent->base.opacity;
    if (parent->base.state1 == 1) {
        return;
    }
    if ((parent->base.m074 & 0x80) != (super->m074 & 0x80)) {
        this->m0F4 = 0x10000 - this->m0F4;
    }
    if ((parent->base.m074 & 0x80) != 0) {
        super->m074 |= 0x80;
        if (this->m0E8 > 0x8000) {
            temp = 0x18000 - this->m0E8;
        } else {
            temp = 0x8000 - this->m0E8;
        }
    } else {
        super->m074 &= ~0x80;
        temp = this->m0E8;
    }

    temp3 = parent->base.angle + temp;
    temp2 = this->m0EC;
    xoffs = (FX_CosIdx(temp3) * temp2) / 0x1000;
    super->pos.x = parent->base.pos.x + xoffs;
    yoffs = (FX_SinIdx(temp3) * temp2) / 0x1000;
    super->pos.y = parent->base.pos.y + yoffs;

    switch (super->state1) {
        case 0:
            if (parent->m0DE != 0) {
                super->state1 = 1;
                this->m0F4 = 0;
                break;
            }
            // TODO: inline
            if ((this->m0E4 < 0 && (super->m074 & 0x80) == 0 && this->m0E2 == 0) ||
                (this->m0E4 > 0 && (super->m074 & 0x80) != 0 && this->m0E2 == 0) ||
                (this->m0E4 < 0 && (super->m074 & 0x80) != 0 && this->m0E2 == 1) ||
                (this->m0E4 > 0 && (super->m074 & 0x80) == 0 && this->m0E2 == 1))
            {
                this->m0F4 += (int)(0.05f * (0x2000 - this->m0F4));
            } else {
                this->m0F4 -= (int)(0.05f * (this->m0F4 - 0xE000));
            }
            break;
        case 1:
            super->state1 = 0;
            // TODO: inline
            if ((this->m0E4 < 0 && (super->m074 & 0x80) == 0) ||
                (this->m0E4 > 0 && (super->m074 & 0x80) != 0))
            {
                this->m0F4 = 0;
            } else {
                this->m0F4 = 0x10000;
            }
            if (this->m0E0 != 0) {
                if ((DrMath_GetRand() & 3) != 0) {
                    if (this->m0E2 != 0) {
                        this->m0E2 = 0;
                    } else {
                        this->m0E2 = 1;
                    }
                }
                if (this->m0E2 != 0) {
                    if (this->m0F4 == 0) {
                        this->m0F4 = 0x10000;
                    } else {
                        this->m0F4 = 0;
                    }
                }
            }
            break;
    }
    super->angle = this->m0F4;
}

static void func_ov019_02203b8c(Enemy100* this, int r1, int r2, int r3, int r4) {
    Enemy100* obj = (Enemy100*)DrObj_ObjGetNextWork(0x7D, 0xE8, (DrObjFunc)func_ov019_022038d4);
    if (obj == NULL) {
        return;
    }
    if (!gObjOpdCopy(super, &obj->base)) {
        DrObj_ObjClearWork(&obj->base);
        return;
    }

    obj->base.drawFunc = esEnemyDraw;
    obj->m0E4 = r1;
    obj->m0E6 = r2;
    obj->base.patternIdx = r4;
    obj->m0F0 = this;
    obj->m0EC = FX_Sqrt(TO_FX32(r1 * r1 + r2 * r2));
    obj->m0E8 = FX_Atan2Idx(r2, r1);
    obj->base.m074 |= 0x04;
    obj->base.angle = 0;
    obj->base.pos.z = super->pos.z + r3;

    if (r4 == 3 || r4 == 4) {
        obj->m0E0 = 1;
    } else {
        obj->m0E0 = 0;
    }

    if (super->m074 & 0x20) {
        obj->base.m074 |= 0x20;
        obj->base.m08A = super->m08A;
    }

    // TODO: inline?
    if ((obj->m0E4 < 0 && (obj->base.m074 & 0x80) == 0) ||
        (obj->m0E4 > 0 && (obj->base.m074 & 0x80) != 0))
    {
        obj->m0F4 = 0;
    } else {
        obj->m0F4 = 0x10000;
    }

    obj->base.m020 = GetFreeAlphaID();
    obj->base.opacity = 0x1F;
    func_ov019_022038d4(obj);
}

// @fabricatedName
static void Enemy100HitAttackCallBack(DrObj* base, DrObj* other, int r2) {
    Enemy100* this = (Enemy100*)base;
    esEnemyHitAttackCallBack(super, other, r2);
}

// @fabricatedName
static void Enemy100DamageCallBack(DrObj* base, DrObj* other, int r2) {
    Enemy100* this = (Enemy100*)base;
    if (esEnemyDamageCallBackSub(super, other, r2)) {
        esEnemyDeadSub(super);
        super->state1 = 1;
        this->m0D0 = 0;
    }
}

void Enemy100Main(DrObj* base) {
    /* Nonmatching */

    Enemy100* this = (Enemy100*)base;
}

void Enemy100Init(DrObj* base) {
    Enemy100* this = (Enemy100*)base;

    if (!DrSetObjOpdCmnFree(super, p_scare_opd, &t_scare_chara, &c_scare_pal)) {
        return;
    }

    super->drawFunc = &esEnemyDraw;
    super->pos.z = 0x5680;
    super->m018 = 0;
    super->vel.y = TO_FX32(-2.5f);
    super->m04C = 0x200;
    esEnemyYukaOrosu(super, &data_ov019_02223980);
    super->state1 = 3;
    this->m0D0 = 0;

    if (this->m13C == 0) {
        super->patternIdx = 1;
        super->m074 |= 0x20;
        super->m08A = 3;
    } else if (this->m13C == 1) {
        super->patternIdx = 6;
        super->m074 |= 0x20;
        super->m08A = 4;
    } else if ((DrMath_GetRand() & 3) == 0) {
        super->patternIdx = 6;
        super->m074 |= 0x20;
        super->m08A = 4;
    } else {
        super->patternIdx = 1;
        super->m074 |= 0x20;
        super->m08A = 3;
    }

    super->m020 = GetFreeAlphaID();
    super->opacity = 0x1F;
    super->m074 |= 0x4;
    super->angle = 0;
    gEnmDir2Player(super);
    if ((super->m074 & 0x80)) {
        this->m0DC = 1;
        super->vel.x = TO_FX32(1.0f);
    } else {
        this->m0DC = 0;
        super->vel.x = TO_FX32(-1.0f);
    }
    esParamSet(super);
    DrObjHit_Init(super, 7, &Enemy100HitAttackCallBack, &Enemy100DamageCallBack);
    DrObjHit_SetAttackParam(super, this->m138, 0);
    esObjHitParamDamageSet(super, this->m132);
    DrObjHit_SetDefaultInvinTimer(super);
    gObjHit_SetHitOpd_BothBoth(super);
    Enemy100Main(super);
    if (super->patternIdx == 1) {
        func_ov019_02203b8c(this, -10, -8, 100, 4);
        func_ov019_02203b8c(this, -12, -8, -100, 3);
        func_ov019_02203b8c(this, 5, -11, 100, 5);
        func_ov019_02203b8c(this, 3, -11, -100, 2);
    }
}
