#include "game/game.h"
#include "global.h"
#include "system/dr_save.h"

// TODO: forward declarations
void SdInf_PlayWithLocation(int, int, int, int);

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
