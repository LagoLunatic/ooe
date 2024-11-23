#include "types.h"

void DrFade_GetParam(int r0, int r1) {
    
}

BOOL DrFade_FadeSet(int r0, int r1, int r2, int r3, int sp00) {
    
}

BOOL func_02027184(int r0, int r1) {
    
}

void func_020271c8() {
    
}

void func_02027230() {
    
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
        if (!func_02027184(r0, i)) {
            r5++;
        }
    }
    return r5 != 5;
}
