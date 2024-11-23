#include "types.h"
#include "game/game.h"

void DrSound_PlayWithLocation(int r0, Vec pos, int sp00) {
    if (DrSave_IsBusy() && (r0 & 0x40000000) != 0) {
        return;
    }
    SdInf_PlayWithLocation(r0, sp00, ((pos.x - get_GameWork()->_464) / 0x1000) - 0x80, ((pos.y - get_GameWork()->_468) / 0x1000) - 0x60);
}
