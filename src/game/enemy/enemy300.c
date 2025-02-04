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
    /* 0x000 */ DrObjBase;
    /* 0x0D0 */ uint m0D0;
    // /* 0x0D4 */ u8 m0D4[0x132 - 0x0D4];
    // /* 0x132 */ u8 m132;
    // /* 0x133 */ u8 m133[0x13C - 0x133];
    // /* 0x13C */ u16 m13C;
    // /* 0x13E */ u16 m13E;
    /* 0x0D4 */ u8 m0D4[0x120 - 0x0D4];
    /* 0x120 */ UnkStruct0x120 m120;
    /* 0x140 */ u8 m140[sizeof(DrObj) - 0x140];
};
STATIC_ASSERT(sizeof(Enemy300Spawner) == sizeof(DrObj));

// @fabricatedName
typedef struct Enemy300 Enemy300;
struct Enemy300 {
    /* 0x000 */ DrObjBase;
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
    /* 0x140 */ u8 m140[sizeof(DrObj) - 0x140];
};
STATIC_ASSERT(sizeof(Enemy300) == sizeof(DrObj));

void Enemy300SpawnerMain(DrObj* obj);

// @fabricatedName
static void Enemy300HitAttackCallBack(DrObj* obj_this, DrObj* obj_other, int r2) {
    esEnemyHitAttackCallBackStatus(obj_this, obj_other, r2, 0, 1);
}

// @fabricatedName
static void Enemy300DamageCallBack(DrObj* obj_this, DrObj* obj_other, int r2) {
    if (esEnemyDamageCallBackSub(obj_this, obj_other, r2)) {
        esEnemyDeadSub(obj_this);
        obj_this->state1 = 4;
        obj_this->state2 = 0;
        obj_this->state3 = 0;
    }
}

// @fabricatedName
static void Enemy300Draw(DrObj* obj) {
    esEnemyDraw(obj);
}

void Enemy300Main(DrObj* obj) {
    Enemy300* bat = (Enemy300*)obj;
    Enemy300Spawner* spawner = bat->m0D0;
    int r4 = 0x60;

    if (bat->m0D6 != 0 && spawner->m012 != bat->m0D4) {
        DrObj_ObjClearWork(obj);
        return;
    }

    if (bat->state1 == 3) {
        if (!esLimitCheck(obj, -0x40, -0x40, 0x140, 0x100)) {
            DrObj_ObjClearWork(obj);
            return;
        }
    }

    if (esEnemyCmnStart(obj, 0)) {
        return;
    }

    switch (bat->state1) {
        case 0: {
            if (esLimitPlCheck(obj, -0x70, -0x10, 0x70, 0x80)) {
                bat->m018 = 4 + (DrMath_GetRand() & 0x1F);
                bat->state1 = 1;
            }
            break;
        }
        case 1: {
            if (bat->m018 != 0) {
                bat->m018--;
                break;
            }
            DrAnime_AnimeSetF_SetObj(obj, 1, 2);
            bat->state1 = 2;
            // Fall-through
        }
        case 2: {
            if (DrAnime_AnimeEnd_SetObj(obj)) {
                DrAnime_AnimeSetF_SetObj(obj, 2, 0);
                gEnmDir2Player(obj);
                esEnemyFlipMoveX(obj, bat->m014);
                bat->state1 = 3;
            }
            break;
        }
        case 3: {
            int r6;
            fx32 playerHeadY;
            bat->m018 += 0x200;
            if (get_GameWork()->difficulty != 0) {
                r4 = 0x50;
            }
            r6 = FX_SinIdx(bat->m018);
            playerHeadY = enemyPlayerGetHeadPos(obj);
            bat->vel.y = (r6 * 1.25f) + ((playerHeadY - bat->pos.y) / r4);
            break;
        }
        case 4: {
            DrObj_ObjClearWork(obj);
            return;
        }
    }

    enemyMoveSpeed(obj);
    DrAnime_AnimeMove_SetObj(obj);
    esEnemyCmnEnd(obj, 0);
}

void Enemy300Init(DrObj* obj) {
    Enemy300* bat = (Enemy300*)obj;
    if (!DrSetObjOpdCmnFree(obj, p_armc02_opd, &f_armc02_chara, &c_armc02_pal)) {
        return;
    }

    if (bat->m120.m13C != 0) {
        bat->mainFunc = &Enemy300SpawnerMain;
        if (get_GameWork()->difficulty != 0) {
            // Fakematch? Need the redundant cast to match the codegen.
            // bat->m120.m13C /= 2;
            ((UnkStruct0x120*)&bat->m120)->m13C /= 2;
        }
        if (bat->m120.m13C < 1) {
            bat->m120.m13C = 1;
        }
        bat->m018 = bat->m120.m13C;
        return;
    }

    bat->drawFunc = &Enemy300Draw;
    bat->pos.z = 0x6A00;
    if (bat->m120.m13E == 0) {
        int temp = DrBgHit_GetAdjustU_Loop(bat->pos.x, bat->pos.y, 0, -0xC0);
        if (temp != 0x7FFF) {
            bat->pos.y += TO_FX32(-temp);
        }
    }

    DrAnime_AnimeInit_SetObj(obj);
    DrAnime_AnimeSet_SetObj(obj, 0);

    if (bat->pos.x < get_GameWork()->player->pos.x) {
        bat->m074 |= 0x80;
    } else {
        bat->m074 &= ~0x80;
    }

    esParamSet(obj);
    DrObjHit_Init(obj, 7, &Enemy300HitAttackCallBack, &Enemy300DamageCallBack);

    bat->m014 = TO_FX32(1.25);
    if (get_GameWork()->difficulty != 0) {
        bat->m014 = TO_FX32(2.0);
    }

    DrObjHit_SetAttackParam(obj, bat->m120.m138, 2);
    esObjHitParamDamageSet(obj, bat->m120.m132);
    DrObjHit_SetDefaultInvinTimer(obj);
}

// @fabricatedName
static void Enemy300SpawnerMain(DrObj* obj) {
    Enemy300Spawner* spawner = (Enemy300Spawner*)obj;
    Enemy300* bat;
    ScrollWork_s* scrollWork;
    int r4, r6, r7;
    EnemyParam_s* enmPrm;
    int r0;

    r0 = spawner->m120.m13C;

    if (spawner->m120.m13E != 0) {
        spawner->m01A++;
        if (spawner->m01A > spawner->m120.m13E * 10) {
            spawner->m01A = 0;
        }
        if (spawner->m01A > spawner->m120.m13E) {
            return;
        }
    }

    spawner->m018++;
    if (spawner->m018 < r0) {
        return;
    }
    spawner->m018 = 0;

    r4 = TO_FX32(0xA0);
    scrollWork = Dr3DBg_GetScrollWork_SetNo(0);
    if (FX32_INT(get_GameWork()->player->pos.x) > (scrollWork->width / 2)) {
        r4 = -r4;
    }
    r7 = spawner->pos.y;
    r6 = get_GameWork()->m464;
    r6 += TO_FX32(0x80);
    r6 += r4;

    bat = (Enemy300*)DrObj_ObjGetNextWork(0x2D, 0x7C, DrObj_DummyFunc);
    if (bat == NULL) {
        return;
    }

    bat->pos.x = r6;
    bat->pos.y = r7;
    bat->m120.m133 = 1;
    bat->m120.m132 = spawner->m120.m132;
    bat->m010 = 2;

    enmPrm = &EnemyParam[bat->m120.m132];
    bat->mainFunc = enmPrm->mainFunc;
    enmPrm->initFunc((DrObj*)bat);

    bat->pos.y = get_GameWork()->player->pos.y - TO_FX32(0x30);
    DrAnime_AnimeSetF_SetObj((DrObj*)bat, 2, 0);
    gEnmDir2Player((DrObj*)bat);
    esEnemyFlipMoveX((DrObj*)bat, bat->m014);
    bat->state1 = 3;
    bat->m0D0 = spawner;
    bat->m0D4 = spawner->m012;
    bat->m0D6 = 1;
    spawner->m0D0++;
}
