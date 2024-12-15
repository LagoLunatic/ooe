#include "game/game.h"
#include "system/dr_opd.h"

typedef struct EnemyParam_s EnemyParam_s;
struct EnemyParam_s {
    /* 0x00 */ DrObjFunc initFunc;
    /* 0x04 */ DrObjFunc mainFunc;
    /* 0x08 */ u8 m08[0x0E - 0x08];
    /* 0x0E */ s16 m0E;
    /* 0x10 */ u8 m10[0x24 - 0x10];
};  // Size: 0x24
extern EnemyParam_s EnemyParam[121];

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

extern DrGraphOpd* p_armc02_opd;
extern DrGraphChara f_armc02_chara;
extern DrGraphPalette c_armc02_pal;

void esEnemyDraw(DrObj*);
BOOL esEnemyCmnStart(DrObj* obj, uint flags);
BOOL esEnemyDamageCallBackSub(DrObj* r0, DrObj* r1, int r2);
BOOL esLimitCheck(DrObj* obj, int, int, int, int);

extern s16 FX_SinCosTable_[0x1000][2];
inline s16 FX_SinIdx(s16 r0) {
    return FX_SinCosTable_[(u16)r0 >> 4][0];
}

// @fabricatedName
inline void tempfakeinline(u8* obj_120) {
    // TODO fakematch
    *(u16*)(obj_120 + 0x1C) /= 2;
}

//

// // @fabricatedName
// typedef struct Enemy300 Enemy300;
// struct Enemy300 {
//     DrObj base;
// };

void Enemy300SpawnerMain(DrObj* obj);

// @fabricatedName
void Enemy300HitAttackCallBack(DrObj* bat, DrObj* other, int r2) {
    esEnemyHitAttackCallBackStatus(bat, other, r2, 0, 1);
}

// @fabricatedName
void Enemy300DamageCallBack(DrObj* bat, DrObj* other, int r2) {
    if (esEnemyDamageCallBackSub(bat, other, r2)) {
        esEnemyDeadSub(bat);
        bat->state1 = 4;
        bat->state2 = 0;
        bat->state3 = 0;
    }
}

// @fabricatedName
void Enemy300Draw(DrObj* obj) {
    esEnemyDraw(obj);
}

void Enemy300Main(DrObj* obj) {
    void* temp = obj->m0D0;
    int r4 = 0x60;

    // fakematch
    if (obj->m0D6 != 0 && *(u16*)((u8*)temp + 0x12) != obj->m0D4) {
        DrObj_ObjClearWork(obj);
        return;
    }

    if (obj->state1 == 3) {
        if (!esLimitCheck(obj, -0x40, -0x40, 0x140, 0x100)) {
            DrObj_ObjClearWork(obj);
            return;
        }
    }

    if (esEnemyCmnStart(obj, 0)) {
        return;
    }

    switch (obj->state1) {
        case 0: {
            if (esLimitPlCheck(obj, -0x70, -0x10, 0x70, 0x80)) {
                obj->m018 = 4 + (DrMath_GetRand() & 0x1F);
                obj->state1 = 1;
            }
            break;
        }
        case 1: {
            if (obj->m018 != 0) {
                obj->m018--;
                break;
            }
            DrAnime_AnimeSetF_SetObj(obj, 1, 2);
            obj->state1 = 2;
            // Fall-through
        }
        case 2: {
            if (DrAnime_AnimeEnd_SetObj(obj)) {
                DrAnime_AnimeSetF_SetObj(obj, 2, 0);
                gEnmDir2Player(obj);
                esEnemyFlipMoveX(obj, obj->m014);
                obj->state1 = 3;
            }
            break;
        }
        case 3: {
            int r6, playerHeadY;
            obj->m018 += 0x200;
            if (get_GameWork()->difficulty != 0) {
                r4 = 0x50;
            }
            r6 = FX_SinIdx(obj->m018);
            playerHeadY = enemyPlayerGetHeadPos(obj);
            obj->vel.y = (r6 * 1.25f) + ((playerHeadY - obj->pos.y) / r4);
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
    if (!DrSetObjOpdCmnFree(obj, p_armc02_opd, &f_armc02_chara, &c_armc02_pal)) {
        return;
    }

    if (obj->m13C != 0) {
        obj->mainFunc = &Enemy300SpawnerMain;
        if (get_GameWork()->difficulty != 0) {
            tempfakeinline((u8*)&obj->m120);  // fakematch?
        }
        if (obj->m13C < 1) {
            obj->m13C = 1;
        }
        obj->m018 = obj->m13C;
        return;
    }

    obj->drawFunc = &Enemy300Draw;
    obj->pos.z = 0x6A00;
    if (obj->m13E == 0) {
        int temp = DrBgHit_GetAdjustU_Loop(obj->pos.x, obj->pos.y, 0, -0xC0);
        if (temp != 0x7FFF) {
            obj->pos.y += TO_FX32(-temp);
        }
    }

    DrAnime_AnimeInit_SetObj(obj);
    DrAnime_AnimeSet_SetObj(obj, 0);

    if (obj->pos.x < get_GameWork()->player->pos.x) {
        obj->m074 |= 0x80;
    } else {
        obj->m074 &= ~0x80;
    }

    esParamSet(obj);
    DrObjHit_Init(obj, 7, &Enemy300HitAttackCallBack, &Enemy300DamageCallBack);

    obj->m014 = TO_FX32(1.25);
    if (get_GameWork()->difficulty != 0) {
        obj->m014 = TO_FX32(2.0);
    }

    DrObjHit_SetAttackParam(obj, obj->m138, 2);
    esObjHitParamDamageSet(obj, obj->m132);
    DrObjHit_SetDefaultInvinTimer(obj);
}

// @fabricatedName
void Enemy300SpawnerMain(DrObj* obj) {
    DrObj* bat;
    ScrollWork_s* scrollWork;
    int r4, r6, r7;
    EnemyParam_s* enmPrm;
    int r0;

    r0 = obj->m13C;

    if (obj->m13E != 0) {
        obj->m01A++;
        if (obj->m01A > obj->m13E * 10) {
            obj->m01A = 0;
        }
        if (obj->m01A > obj->m13E) {
            return;
        }
    }

    obj->m018++;
    if (obj->m018 < r0) {
        return;
    }
    obj->m018 = 0;

    r4 = TO_FX32(0xA0);
    scrollWork = Dr3DBg_GetScrollWork_SetNo(0);
    if (FX32_INT(get_GameWork()->player->pos.x) > (scrollWork->width / 2)) {
        r4 = -r4;
    }
    r7 = obj->pos.y;
    r6 = get_GameWork()->m464;
    r6 += TO_FX32(0x80);
    r6 += r4;

    bat = DrObj_ObjGetNextWork(0x2D, 0x7C, DrObj_DummyFunc);
    if (bat == NULL) {
        return;
    }

    bat->pos.x = r6;
    bat->pos.y = r7;
    bat->m133 = 1;
    bat->m132 = obj->m132;
    bat->m010 = 2;

    enmPrm = &EnemyParam[bat->m132];
    bat->mainFunc = enmPrm->mainFunc;
    enmPrm->initFunc(bat);

    bat->pos.y = get_GameWork()->player->pos.y - TO_FX32(0x30);
    DrAnime_AnimeSetF_SetObj(bat, 2, 0);
    gEnmDir2Player(bat);
    esEnemyFlipMoveX(bat, bat->m014);
    bat->state1 = 3;
    bat->m0D0 = obj;
    bat->m0D4 = obj->m012;
    bat->m0D6 = 1;
    (*(uint*)&obj->m0D0)++;  // TODO fakematch
}
