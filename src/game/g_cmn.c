#include "system/dr_graph.h"
#include "system/dr_obj.h"
#include "system/dr_pal.h"
#include "system/dr_vram.h"

BOOL DrSetObjOpdCmnFreeFlag(
    DrObj* obj, DrGraphOpd* opd, DrGraphChara* chara, DrGraphPalette* pal, int flag
) {
    int r5 = chara->m2 > 0x10 ? 4 : 1;
    int vramIdx = DrVram_GetFreeVram(chara, 1, r5);
    if (vramIdx == -1) {
        DrObj_ObjClearWork(obj);
        return FALSE;
    }
    DrVram_CharaTrans(chara, vramIdx, 0, 0, flag, r5, 1);
    if (!DrObj_SetOPD(obj, opd, vramIdx)) {
        DrObj_ObjClearWork(obj);
        return FALSE;
    }
    if (pal != NULL) {
        obj->m086 = DrPal_FreePalEntry(pal, 0, 0);
        obj->m088 = (int)obj->m086;
    }
    return TRUE;
}

BOOL DrSetObjOpdCmnFree(DrObj* obj, DrGraphOpd* opd, DrGraphChara* chara, DrGraphPalette* pal) {
    return DrSetObjOpdCmnFreeFlag(obj, opd, chara, pal, 0);
}
