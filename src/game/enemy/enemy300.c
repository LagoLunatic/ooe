#include "game/enemy/es_enemy.h"
#include "game/game.h"
#include "global.h"
#include "system/dr_3dbg.h"
#include "system/dr_anime.h"
#include "system/dr_bghit.h"
#include "system/dr_math.h"
#include "system/dr_objhit.h"
#include "system/dr_opd.h"

extern DrGraphOpd* p_armc02_opd;
extern DrGraphChara f_armc02_chara;
extern DrGraphPalette c_armc02_pal;

// @fabricatedName
typedef struct UnkStruct0x120 UnkStruct0x120;
struct UnkStruct0x120 {
    /* 0x120 */ u32 m120;
    /* 0x124 */ u8 m124[0x132 - 0x124];
    // /* 0x120 */ u8 m120[0x132 - 0x120];
    /* 0x132 */ u8 m132;
    /* 0x133 */ u8 m133;
    /* 0x134 */ u8 m134[0x138 - 0x134];
    /* 0x138 */ s16 m138;
    /* 0x13A */ u8 m13A[0x13C - 0x13A];
    /* 0x13C */ u16 m13C;
    /* 0x13E */ u16 m13E;
};

// @fabricatedName
typedef struct Enemy300Spawner Enemy300Spawner;
struct Enemy300Spawner {
    /* 0x000 */ DrObj base;
    /* 0x0D0 */ uint m0D0;
    // /* 0x0D4 */ u8 m0D4[0x132 - 0x0D4];
    // /* 0x132 */ u8 m132;
    // /* 0x133 */ u8 m133[0x13C - 0x133];
    // /* 0x13C */ u16 m13C;
    // /* 0x13E */ u16 m13E;
    /* 0x0D4 */ u8 m0D4[0x120 - 0x0D4];
    /* 0x120 */ UnkStruct0x120 m120;
    /* 0x140 */ u8 m140[sizeof(DrObjGeneric) - 0x140];
};
STATIC_ASSERT(sizeof(Enemy300Spawner) == sizeof(DrObjGeneric));

// @fabricatedName
typedef struct Enemy300 Enemy300;
struct Enemy300 {
    /* 0x000 */ DrObj base;
    /* 0x0D0 */ Enemy300Spawner* m0D0;
    /* 0x0D4 */ u16 m0D4;
    /* 0x0D6 */ u16 m0D6;
    /* 0x0D8 */ u8 m0D8[0x120 - 0x0D8];
    // /* 0x120 */ u32 m120;
    // /* 0x124 */ u8 m124[0x132 - 0x124];
    // /* 0x132 */ u8 m132;
    // /* 0x133 */ u8 m133;
    // /* 0x134 */ u8 m134[0x138 - 0x134];
    // /* 0x138 */ s16 m138;
    // /* 0x13A */ u8 m13A[0x13C - 0x13A];
    // /* 0x13C */ u16 m13C;
    // /* 0x13E */ u16 m13E;
    /* 0x120 */ UnkStruct0x120 m120;
    /* 0x140 */ u8 m140[sizeof(DrObjGeneric) - 0x140];
};
STATIC_ASSERT(sizeof(Enemy300) == sizeof(DrObjGeneric));

void Enemy300SpawnerMain(DrObj* base);

// @fabricatedName
static void Enemy300HitAttackCallBack(DrObj* base, DrObj* other, int r2) {
    Enemy300* this = (Enemy300*)base;
    esEnemyHitAttackCallBackStatus(super, other, r2, 0, 1);
}

// @fabricatedName
static void Enemy300DamageCallBack(DrObj* base, DrObj* other, int r2) {
    Enemy300* this = (Enemy300*)base;
    if (esEnemyDamageCallBackSub(super, other, r2)) {
        esEnemyDeadSub(super);
        super->state1 = 4;
        super->state2 = 0;
        super->state3 = 0;
    }
}

// @fabricatedName
static void Enemy300Draw(DrObj* base) {
    Enemy300* this = (Enemy300*)base;
    esEnemyDraw(super);
}

void Enemy300Main(DrObj* base) {
    Enemy300* this = (Enemy300*)base;
    Enemy300Spawner* spawner = this->m0D0;
    int r4 = 0x60;

    if (this->m0D6 != 0 && spawner->base.m012 != this->m0D4) {
        DrObj_ObjClearWork(super);
        return;
    }

    if (super->state1 == 3) {
        if (!esLimitCheck(super, -0x40, -0x40, 0x140, 0x100)) {
            DrObj_ObjClearWork(super);
            return;
        }
    }

    if (esEnemyCmnStart(super, 0)) {
        return;
    }

    switch (super->state1) {
        case 0: {
            if (esLimitPlCheck(super, -0x70, -0x10, 0x70, 0x80)) {
                super->m018 = 4 + (DrMath_GetRand() & 0x1F);
                super->state1 = 1;
            }
            break;
        }
        case 1: {
            if (super->m018 != 0) {
                super->m018--;
                break;
            }
            DrAnime_AnimeSetF_SetObj(super, 1, 2);
            super->state1 = 2;
            // Fall-through
        }
        case 2: {
            if (DrAnime_AnimeEnd_SetObj(super)) {
                DrAnime_AnimeSetF_SetObj(super, 2, 0);
                gEnmDir2Player(super);
                esEnemyFlipMoveX(super, super->m014);
                super->state1 = 3;
            }
            break;
        }
        case 3: {
            int r6;
            fx32 playerHeadY;
            super->m018 += 0x200;
            if (get_GameWork()->difficulty != 0) {
                r4 = 0x50;
            }
            r6 = FX_SinIdx(super->m018);
            playerHeadY = enemyPlayerGetHeadPos(super);
            super->vel.y = (r6 * 1.25f) + ((playerHeadY - super->pos.y) / r4);
            break;
        }
        case 4: {
            DrObj_ObjClearWork(super);
            return;
        }
    }

    enemyMoveSpeed(super);
    DrAnime_AnimeMove_SetObj(super);
    esEnemyCmnEnd(super, 0);
}

void Enemy300Init(DrObj* base) {
    Enemy300* this = (Enemy300*)base;

    if (!DrSetObjOpdCmnFree(super, p_armc02_opd, &f_armc02_chara, &c_armc02_pal)) {
        return;
    }

    if (this->m120.m13C != 0) {
        super->mainFunc = &Enemy300SpawnerMain;
        if (get_GameWork()->difficulty != 0) {
            // Fakematch? Need the redundant cast to match the codegen.
            // bat->m120.m13C /= 2;
            ((UnkStruct0x120*)&this->m120)->m13C /= 2;
        }
        if (this->m120.m13C < 1) {
            this->m120.m13C = 1;
        }
        super->m018 = this->m120.m13C;
        return;
    }

    super->drawFunc = &Enemy300Draw;
    super->pos.z = 0x6A00;
    if (this->m120.m13E == 0) {
        int temp = DrBgHit_GetAdjustU_Loop(super->pos.x, super->pos.y, 0, -0xC0);
        if (temp != 0x7FFF) {
            super->pos.y += TO_FX32(-temp);
        }
    }

    DrAnime_AnimeInit_SetObj(super);
    DrAnime_AnimeSet_SetObj(super, 0);

    if (super->pos.x < get_GameWork()->player->pos.x) {
        super->m074 |= 0x80;
    } else {
        super->m074 &= ~0x80;
    }

    esParamSet(super);
    DrObjHit_Init(super, 7, &Enemy300HitAttackCallBack, &Enemy300DamageCallBack);

    super->m014 = TO_FX32(1.25);
    if (get_GameWork()->difficulty != 0) {
        super->m014 = TO_FX32(2.0);
    }

    DrObjHit_SetAttackParam(super, this->m120.m138, 2);
    esObjHitParamDamageSet(super, this->m120.m132);
    DrObjHit_SetDefaultInvinTimer(super);
}

// @fabricatedName
static void Enemy300SpawnerMain(DrObj* base) {
    Enemy300Spawner* this = (Enemy300Spawner*)base;
    Enemy300* bat;
    ScrollWork_s* scrollWork;
    int r4, r6, r7;
    EnemyParam_s* enmPrm;
    int r0;

    r0 = this->m120.m13C;

    if (this->m120.m13E != 0) {
        super->m01A++;
        if (super->m01A > this->m120.m13E * 10) {
            super->m01A = 0;
        }
        if (super->m01A > this->m120.m13E) {
            return;
        }
    }

    super->m018++;
    if (super->m018 < r0) {
        return;
    }
    super->m018 = 0;

    r4 = TO_FX32(0xA0);
    scrollWork = Dr3DBg_GetScrollWork_SetNo(0);
    if (FX32_INT(get_GameWork()->player->pos.x) > (scrollWork->width / 2)) {
        r4 = -r4;
    }
    r7 = super->pos.y;
    r6 = get_GameWork()->m464;
    r6 += TO_FX32(0x80);
    r6 += r4;

    bat = (Enemy300*)DrObj_ObjGetNextWork(0x2D, 0x7C, DrObj_DummyFunc);
    if (bat == NULL) {
        return;
    }

    bat->base.pos.x = r6;
    bat->base.pos.y = r7;
    bat->m120.m133 = 1;
    bat->m120.m132 = this->m120.m132;
    bat->base.m010 = 2;

    enmPrm = &EnemyParam[bat->m120.m132];
    bat->base.mainFunc = enmPrm->mainFunc;
    enmPrm->initFunc(&bat->base);

    bat->base.pos.y = get_GameWork()->player->pos.y - TO_FX32(0x30);
    DrAnime_AnimeSetF_SetObj(&bat->base, 2, 0);
    gEnmDir2Player(&bat->base);
    esEnemyFlipMoveX(&bat->base, bat->base.m014);
    bat->base.state1 = 3;
    bat->m0D0 = this;
    bat->m0D4 = super->m012;
    bat->m0D6 = 1;
    this->m0D0++;
}
