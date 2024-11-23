
void gPlayerRead() {
    /* Nonmatching */
    int r7, r6;
    r7 = DrHeap_GetAreaAddr(11);
    r6 = DrHeap_GetAreaAddr(10);
    if (!DrFS_FileRead()) {
        return;
    }
    RawOpdData_Create(r6);
    DrFS_FileRead();
}
