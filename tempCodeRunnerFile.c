void tongHopFile() {
    if (soMon == 0 || !docFile((char*)fileMon[0])) {n = 0; return;}
    SinhVien tempDS[MAX];
    int nTemp = n;
    int i, f, j;
    for (i = 0; i < nTemp; i++) {
        tempDS[i] = ds[i];
    }
    float tongDTB[MAX];
    int soMonCompleted[MAX];
    for (i = 0; i < nTemp; i++) {
        tongDTB[i] = 0.0f;
        soMonCompleted[i] = 0;
    }
    for (f = 0; f < soMon; f++) {
        if (!docFile((char*)fileMon[f])) continue;
        
        for (i = 0; i < nTemp; i++) {
            for (j = 0; j < n; j++) {
                if (strcmp(tempDS[i].maSV, ds[j].maSV) == 0) {
                    if (daCoDiem(ds[j])) {
                        tongDTB[i] += ds[j].dtb;
                        soMonCompleted[i]++;
                    }
                    break;
                }
            }
        }
    }
    n = 0;
    for (i = 0; i < nTemp; i++) {
        if (soMonCompleted[i] > 0) {
            ds[n] = tempDS[i];
            ds[n].dtb = tongDTB[i] / (float)soMonCompleted[i];
    
            if (ds[n].dtb >= 8.5) strcpy(ds[n].diemChu, "A");
            else if (ds[n].dtb >= 7.0) strcpy(ds[n].diemChu, "B");
            else if (ds[n].dtb >= 5.5) strcpy(ds[n].diemChu, "C");
            else if (ds[n].dtb >= 4.0) strcpy(ds[n].diemChu, "D");
            else strcpy(ds[n].diemChu, "F");
            n++;
        }
    }
    if (n == 0) printf("\n[!] Khong tim thay sinh vien nao co du diem de tong hop!\n");
}