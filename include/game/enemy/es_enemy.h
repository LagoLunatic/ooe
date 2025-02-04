#ifndef ES_ENEMY_H
#define ES_ENEMY_H

// Unsure about the filename of this TU.
// Some of the functions in here have names that start with es but not esEnemy.
// e.g. esPalTransRoll, esLimitPlCheck, esLimitCheck
// Also, what does es stand for? EnemySet?

#include "system/dr_obj.h"

typedef struct EnemyParam_s EnemyParam_s;
struct EnemyParam_s {
    /* 0x00 */ DrObjFunc initFunc;
    /* 0x04 */ DrObjFunc mainFunc;
    /* 0x08 */ u8 m08[0x0E - 0x08];
    /* 0x0E */ s16 m0E;
    /* 0x10 */ u8 m10[0x24 - 0x10];
};  // Size: 0x24

extern EnemyParam_s EnemyParam[121];

void esEnemyDraw(DrObj*);
BOOL esEnemyCmnStart(DrObj* obj, uint flags);
BOOL esEnemyDamageCallBackSub(DrObj* r0, DrObj* r1, int r2);
BOOL esEnemyHitAttackCallBackStatus(DrObj* r0, DrObj* r1, int r2, int, int);
BOOL esLimitCheck(DrObj* obj, int, int, int, int);
void esEnemyYukaOrosu(DrObj*, void*);
void esEnemyHitAttackCallBack();
void esEnemyDeadSub(DrObj*);
void gEnmDir2Player(DrObj*);
void esParamSet(DrObj*);
void esEnemyCmnEnd(DrObj*, int);
void enemyMoveSpeed(DrObj*);
BOOL esLimitPlCheck(DrObj*, int, int, int, int);
void esEnemyFlipMoveX(DrObj*, int);
fx32 enemyPlayerGetHeadPos(DrObj*);

#endif /* ES_ENEMY_H */
