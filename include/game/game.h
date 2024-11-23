#ifndef GAME_H
#define GAME_H

typedef struct GameWork_s {
    u8 _000[0x464 - 0x000];
    int _464;
    int _468;
    u8 _46C[0x6B8 - 0x46C];
    u32 money;
    u8 _6BC[0x740 - 0x6BC];
    u32 pickupFlags[13];
} GameWork_s;

extern GameWork_s GameWork;

inline GameWork_s* get_GameWork() {
    return &GameWork;
}

#endif /* GAME_H */
