#include "game/game.h"
#include "types.h"

void DrSound_PlayWithLocation(int soundId, Vec3x32 pos, int sp00) {
    if (DrSave_IsBusy() && (soundId & 0x40000000) != 0) {
        return;
    }
    SdInf_PlayWithLocation(
        soundId,
        sp00,
        FX32_INT(pos.x - get_GameWork()->m464) - 0x80,
        FX32_INT(pos.y - get_GameWork()->m468) - 0x60
    );
}
