#ifndef DR_ANIME_H
#define DR_ANIME_H

#include "system/dr_obj.h"

void DrAnime_AnimeInit_SetOpdAnm(DrObj*);
void DrAnime_AnimeInit_SetObjAnm(DrObj*);
void DrAnime_AnimeSet_SetAnm(DrObj*);
void DrAnime_AnimeSetF_SetAnm(DrObj*);
void func_02030404(DrObj*);
void func_0203041c(DrObj*);
void DrAnime_AnimeMove_SetAnm(DrObj*);
void func_020305e0(DrObj*);
void func_020305ec(DrObj*);
void func_02030618(DrObj*);
void func_02030634(DrObj*);
void func_02030644(DrObj*);
void DrAnime_AnimeInit_SetObj(DrObj*);
void DrAnime_AnimeSet_SetObj(DrObj*, u16);
void DrAnime_AnimeSetF_SetObj(DrObj*, u16, u16);
void DrAnime_AnimeSetCF_SetObj(DrObj*);
void func_02030700(DrObj*);
void func_02030728(DrObj*);
void DrAnime_AnimeMove_SetObj(DrObj*);
u16 DrAnime_AnimeEnd_SetObj(DrObj*);
void func_02030754(DrObj*);
void func_02030780(DrObj*);
void DrAnime_AnimePause_SetObj(DrObj*);
void DrAnime_AnimePlay_SetObj(DrObj*);
void DrAnime_AnimeFrameGet_SetObj(DrObj*);

#endif /* DR_ANIME_H */
