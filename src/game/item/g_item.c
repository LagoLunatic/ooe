#include "types.h"
#include "system/dr_obj.h"
#include "game/game.h"

void gAgbItemNameWinOpen(u16 r0);

BOOL func_02062c78(DrObj* r4) {
    BOOL r1 = FALSE;
    u32 r2;
    r2 = r4->_0DC;
    if ((r2 & 0x400) != 0) {
        r1 = TRUE;
    } else {
        r2 |= 0x400;
        if ((r2 & 0x4) != 0) {
            r1 = TRUE;
        } else {
            r2 |= 0x10;
        }
    }
    r4->_0DC = r2;
    return r1;
}

void func_02062eb8(DrObj* r4) {
    if (func_02062c78(r4)) { return; }
    if (r4->_13C != 0) {
        get_GameWork()->pickupFlags[r4->_13C >> 5] |= 1 << (r4->_13C & 0x1F);
    }
    gAgbItemNameWinOpen(r4->_0E0 + 0x43D);
    DrSound_PlayWithLocation(0x11a0000d, r4->pos, 0);
    
    switch (r4->_0E0) {
        case 0:
            get_GameWork()->money += 1;
            break;
        case 1:
            get_GameWork()->money += 10;
            break;
        case 2:
            get_GameWork()->money += 50;
            break;
        case 3:
            get_GameWork()->money += 100;
            break;
        case 4:
            get_GameWork()->money += 500;
            break;
        case 5:
            get_GameWork()->money += 1000;
            break;
        case 6:
            get_GameWork()->money += 2000;
            break;
    }
    
    if (get_GameWork()->money > 9999999) {
        get_GameWork()->money = 9999999;
    }
}
