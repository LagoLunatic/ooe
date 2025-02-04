#include "game/g_agb.h"
#include "game/game.h"
#include "global.h"
#include "system/dr_obj.h"
#include "system/dr_sound.h"

// @fabricatedName
typedef struct Item Item;
struct Item {
    /* 0x000 */ DrObjBase;
    /* 0x0D0 */ u8 m0D0[0x0DC - 0x0D0];
    /* 0x0DC */ u32 m0DC;
    /* 0x0E0 */ u32 m0E0;
    /* 0x0E4 */ u8 m0E4[0x13c - 0x0E4];
    /* 0x13C */ u16 m13C;
    /* 0x13E */ u8 m13E[sizeof(DrObj) - 0x13E];
};
STATIC_ASSERT(sizeof(Item) == sizeof(DrObj));

BOOL func_02062c78(DrObj* obj) {
    Item* item = (Item*)obj;
    BOOL r1 = FALSE;
    u32 r2;
    r2 = item->m0DC;
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
    item->m0DC = r2;
    return r1;
}

void func_02062eb8(DrObj* obj) {
    Item* item = (Item*)obj;
    if (func_02062c78(obj)) return;
    if (item->m13C != 0) {
        get_GameWork()->pickupFlags[item->m13C >> 5] |= 1 << (item->m13C & 0x1F);
    }
    gAgbItemNameWinOpen(item->m0E0 + 0x43D);
    DrSound_PlayWithLocation(0x11a0000d, item->pos, 0);

    switch (item->m0E0) {
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
