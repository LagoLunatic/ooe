#include "game/enemy/g_enemy.h"
#include "game/enemy/es_enemy.h"

void* DrFSsub_FileInfoGetHeadAddr(void*);
int DrFSsub_FileInfoGetFileType();
int DrVram_SearchData_SetStartEnd(void*, int, int);

typedef struct unk_HeadAddr {
    int unk1;
    int unk2;
} unk_HeadAddr;

unk_HeadAddr* enemy_read_tbl[];
unk_HeadAddr* gimmick_read_tbl[];

extern DrObjFunc* dev_func;
extern DrObjFunc* dev_i_func;

void EnmSetFirst() {
    /* Nonmatching */
}

void func_02064bf0() {
    /* Nonmatching */
}

void EnemyRead(int, int) {
    /* Nonmatching */
}

unk_HeadAddr* EneGetOpdHeadAddr(int param_1, int param_2) {
    if (param_2 == 1) {
        return enemy_read_tbl[param_1];
    } else if (param_2 == 2) {
        return gimmick_read_tbl[param_1];
    }
    return NULL;
}

void func_02064f9c() {
    /* Nonmatching */
}

void func_02065018() {
    /* Nonmatching */
}

void EneAlignOpd(int, int) {
    /* Nonmatching */
}

void func_020650e4() {
    /* Nonmatching */
}

void func_020650e8() {
    /* Nonmatching */
}

void* func_020651ac(int param_1, int param_2, int param_3, int param_4) {
    unk_HeadAddr* eneOpdHead;
    eneOpdHead = EneGetOpdHeadAddr(param_2, param_3);
    if (eneOpdHead == NULL) {
        return NULL;
    }

    for (; eneOpdHead->unk1 >= 0; eneOpdHead++) {
        if (eneOpdHead->unk2 == param_4 &&
            DrFSsub_FileInfoGetFileType(eneOpdHead->unk1, eneOpdHead->unk2) == param_1)
        {
            return eneOpdHead;
        }
    }
    return NULL;
}

DrObj* gEnemyFreeSet(int param_1, int param_2, Vec3x32 pos, s16 param_6) {
    /* Nonmatching */
    DrObj* obj;
    void* eneOpdHead;

    if (param_1 == 1) {
        eneOpdHead = func_020651ac(1, param_2, 1, 1);
        if (eneOpdHead == NULL) {
            return NULL;
        }
        obj = DrObj_ObjGetNextWork(0x2D, 0x7C, EnemyParam[param_2].mainFunc);
        if (obj == NULL) {
            return NULL;
        }
        obj->m010 = 2;
        DrFSsub_FileInfoGetHeadAddr(NULL);
        if (DrVram_SearchData_SetStartEnd(NULL, 0x12, 0x28) == -1) {
            EnemyRead(param_2, 1);
            EneAlignOpd(param_2, 1);
        }
    } else if (param_1 == 2) {
        obj = DrObj_ObjGetNextWork(0x2D, 0x7C, NULL);
    }

    return obj;
}

DrObj* gEnemyFreeParamSet(int param_1, int param_2, Vec3x32 pos, s16 param_6) {
    /* Nonmatching */
    EnemyParam_s* enmPrm;
    DrObj* obj;
    DrObjFunc initFunc;

    enmPrm = &EnemyParam[param_2];
    obj = DrObj_ObjGetNextWork(0x2D, 0x7C, NULL);
    if (obj != NULL) {
        obj->m010 = 2;
        obj->pos = pos;
        if (param_1 == 1) {
            obj->mainFunc = enmPrm->mainFunc;
            initFunc = enmPrm->initFunc;
        } else if (param_1 == 2) {
            obj->mainFunc = dev_func[param_2];
            initFunc = dev_i_func[param_2];
        }
        (*initFunc)(obj);
    }
    return obj;
}
