#include "global.h"

void DrFade_Init() {
}

void DrFade_Init_SetParam() {
}

void DrFade_FadeMain() {
}

void DrFade_GetParam(int r0, int r1) {
}

BOOL DrFade_FadeSet(int r0, int r1, int r2, int r3, int sp00) {
}

BOOL DrFade_IsFade(int r0, int r1) {
}

void DrFade_FadeReset() {
}

void DrFade_FadeReset_All() {
}

BOOL DrFade_FadeSet_All(int r0, int r1, int r2, int r3) {
    int i;
    for (i = 0; i < 5; i++) {
        if (!DrFade_FadeSet(r0, i, r1, r2, r3)) {
            return FALSE;
        }
    }
    return TRUE;
}

int DrFade_IsFade_All(int r0) {
    int i, r5;
    i = 0;
    r5 = 0;
    for (i = 0; i < 5; i++) {
        if (!DrFade_IsFade(r0, i)) {
            r5++;
        }
    }
    return r5 != 5;
}
