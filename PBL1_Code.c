#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <conio.h>
	#include <stdbool.h>
	#define MAX 100
    #define MAX_MON 100
    #define MAX_HP 20

	int cheDoTong = 0;
	char fileHPHienTai[50] = "";

	typedef struct {
	    char maSV[20];
	    char lop[20];
	    char tenSV[50];
	    float lab1, lab2, pt1, pt2, presentation, finalTest;
	    float dtb;
	    char diemChu[3];
	} SinhVien;
	
	SinhVien ds[MAX];
	int n = 0;
	int colLocked[6] = {0, 0, 0, 0, 0, 0};
	char *tenCotGlobal[] = {"Lab1", "Lab2", "PT1", "PT2", "Presentation", "Final"};
	float wLab, wPT, wPre, wFinal;
	int soSVToiDa = 0;
	
	typedef struct {
	    char maHP[20];
	    char fileHP[50];
	} HocPhan;

	typedef struct {
	    char tenMon[30];
	    int soHocPhan;
	    HocPhan dsHocPhan[MAX_HP];
	} MonHoc;

	MonHoc dsMonHoc[MAX_MON];
	int soMon = 0;

	typedef struct {
	    char maSV[20];
	    char lop[20];
	    char tenSV[50];
	} SinhVienInfo;

	SinhVienInfo dsSV[MAX];
	int nSV = 0;
	
	void docFilemonhoc() {
	    FILE *fp = fopen("monhoc.txt", "r");
	    if (!fp) {
	        fp = fopen("monhoc.txt", "w");
	        if (fp) {
	            fclose(fp);
	        }
	        soMon = 0;
	        return;
	    }
	    soMon = 0;
	    char line[1024];
	    while (fgets(line, sizeof(line), fp) && soMon < MAX_MON) {
	        line[strcspn(line, "\r\n")] = '\0';
	        if (strlen(line) < 3) continue;
	        char *token = strtok(line, "\t");
	        if (!token) continue;
	        strncpy(dsMonHoc[soMon].tenMon, token, 29);
	        dsMonHoc[soMon].tenMon[29] = '\0';
	        token = strtok(NULL, "\t");
	        if (!token) continue;
	        int tempSoHP = atoi(token);
	        if (tempSoHP > MAX_HP) {
	            tempSoHP = MAX_HP;
	        }
	        dsMonHoc[soMon].soHocPhan = tempSoHP;
	        int i;
	        for (i = 0; i < dsMonHoc[soMon].soHocPhan; i++) {
	            token = strtok(NULL, "\t");
	            if (!token) break;
	            strncpy(dsMonHoc[soMon].dsHocPhan[i].maHP, token, 19);
	            dsMonHoc[soMon].dsHocPhan[i].maHP[19] = '\0';
	            token = strtok(NULL, "\t");
	            if (!token) break;
	            strncpy(dsMonHoc[soMon].dsHocPhan[i].fileHP, token, 49);
	            dsMonHoc[soMon].dsHocPhan[i].fileHP[49] = '\0';
	        }
	        dsMonHoc[soMon].soHocPhan = i;
	        soMon++;
	    }
	    fclose(fp);
	}

	void ghiFilemonhoc() {
	    FILE *fp = fopen("monhoc.txt", "w");
	    if (!fp) return;
	    int i, j;
	    for (i = 0; i < soMon; i++) {
	        int limit = dsMonHoc[i].soHocPhan;
	        if (limit > MAX_HP) limit = MAX_HP;
	        fprintf(fp, "%s\t%d", dsMonHoc[i].tenMon, limit);
	        for (j = 0; j < limit; j++) {
	            fprintf(fp, "\t%s\t%s", dsMonHoc[i].dsHocPhan[j].maHP, dsMonHoc[i].dsHocPhan[j].fileHP);
	        }
	        fprintf(fp, "\n");
	    }
	    fclose(fp);
	}

	bool svCoTrongFile(char tenFile[], char mssv[]) {
	    FILE *fp = fopen(tenFile, "r");
	    if (!fp) return false;
	    char line[256];
	    if (fgets(line, sizeof(line), fp)) {
	        
	    }
	    while (fgets(line, sizeof(line), fp)) {
	        if (strlen(line) < 5) continue;
	        char curMSSV[20];
	        if (sscanf(line, "%19s", curMSSV) >= 1) {
	            if (strcmp(curMSSV, mssv) == 0) {
	                fclose(fp);
	                return true;
	            }
	        }
	    }
	    fclose(fp);
	    return false;
	}

	bool chonHocPhan(char tenFile[], bool choPhepTongHop, const char displayTitle[]) {
	    if (soMon == 0) {
	        printf("\n[!] Chua co mon hoc nao trong he thong!\n");
	        strcpy(tenFile, "");
	        return false;
	    }
	    printf("\n--- %s ---\n", displayTitle);
	    printf("Danh sach mon hoc:\n");
	    int i;
	    for (i = 0; i < soMon; i++) {
	        printf("%d. %s\n", i + 1, dsMonHoc[i].tenMon);
	    }
	    if (choPhepTongHop) {
	        printf("%d. Tong hop\n", soMon + 1);
	    }
	    printf("Chon: ");
	    int lcMon;
	    scanf("%d", &lcMon);
	    if (choPhepTongHop && lcMon == soMon + 1) {
	        strcpy(tenFile, "tonghop");
	        return true;
	    }
	    if (lcMon < 1 || lcMon > soMon) {
	        printf("[!] Lua chon khong hop le!\n");
	        return false;
	    }
	    int monIdx = lcMon - 1;
	    if (dsMonHoc[monIdx].soHocPhan == 0) {
	        printf("[!] Mon hoc nay chua co hoc phan nao!\n");
	        return false;
	    }
	    printf("\nChon hoc phan cua mon %s:\n", dsMonHoc[monIdx].tenMon);
	    for (i = 0; i < dsMonHoc[monIdx].soHocPhan; i++) {
	        printf("%d. %s (%s)\n", i + 1, dsMonHoc[monIdx].dsHocPhan[i].maHP, dsMonHoc[monIdx].dsHocPhan[i].fileHP);
	    }
	    printf("Chon: ");
	    int lcHP;
	    scanf("%d", &lcHP);
	    if (lcHP < 1 || lcHP > dsMonHoc[monIdx].soHocPhan) {
	        printf("[!] Lua chon khong hop le!\n");
	        return false;
	    }
	    strcpy(tenFile, dsMonHoc[monIdx].dsHocPhan[lcHP - 1].fileHP);
	    return true;
	}
	
	bool docFile(char tenFile[]);
	bool daCoDiem(SinhVien sv);
	void docFileSinhVien();
	int timSVTheoMSSV(char mssv[]);
	
	void tinhDiem(SinhVien *sv) {
	    sv->dtb = (sv->lab1 + sv->lab2) * wLab 
	            + (sv->pt1 + sv->pt2) * wPT 
	            + sv->presentation * wPre 
	            + sv->finalTest * wFinal;

	    if (sv->dtb >= 8.5) strcpy(sv->diemChu, "A");
	    else if (sv->dtb >= 7.0) strcpy(sv->diemChu, "B");
	    else if (sv->dtb >= 5.5) strcpy(sv->diemChu, "C");
	    else if (sv->dtb >= 4.0) strcpy(sv->diemChu, "D");
	    else if (daCoDiem(*sv)) strcpy(sv->diemChu, "F");
		else strcpy(sv->diemChu, " ");
	}

	void ghiFile(char tenFile[]) {
        FILE *fp = fopen(tenFile, "w"); 
        int i;
        if (!fp) return;
    
        fprintf(fp, "%f %f %f %f %d %d %d %d %d %d %d\n", 
                    wLab, wPT, wPre, wFinal,
                    colLocked[0], colLocked[1], colLocked[2], colLocked[3], colLocked[4], colLocked[5],
                    soSVToiDa);
    
        for (i = 0; i < n; i++) {
            fprintf(fp, "%s %s %s %f %f %f %f %f %f\n", 
                    ds[i].maSV,
		    		ds[i].lop, 
                    ds[i].tenSV, 
                    ds[i].lab1, ds[i].lab2, 
                    ds[i].pt1, ds[i].pt2, 
                    ds[i].presentation, 
                    ds[i].finalTest);
        }
        fclose(fp);
    }
    bool docFile(char tenFile[]) {
        strcpy(fileHPHienTai, tenFile);
        FILE *fp = fopen(tenFile, "r");
        if (!fp) {
            printf("File khong ton tai\n");
            return false;
        }

        char line[256];

        if (fgets(line, sizeof(line), fp)) {
            int tc = 0;
            int parsed = sscanf(line, "%f %f %f %f %d %d %d %d %d %d %d", 
                            &wLab, &wPT, &wPre, &wFinal, 
                            &colLocked[0], &colLocked[1], &colLocked[2], &colLocked[3], &colLocked[4], &colLocked[5],
                            &tc);
            if (parsed < 4) {
               wLab = 0.1; wPT = 0.1; wPre = 0.2; wFinal = 0.4;
               int ci; for(ci=0;ci<6;ci++) colLocked[ci]=0;
               soSVToiDa = 0;
               rewind(fp); 
            } else {
                soSVToiDa = (parsed >= 11) ? tc : 0;
                if (soSVToiDa > MAX) {
                    soSVToiDa = MAX;
                }
            }
        }

        n = 0;

        while (fgets(line, sizeof(line), fp) && n < MAX) {
            if (strlen(line) < 5) continue;
            int check = sscanf(line, "%19s %19s %[^0-9.-] %f %f %f %f %f %f", 
                               ds[n].maSV,
			    			   ds[n].lop, 
                               ds[n].tenSV, 
                               &ds[n].lab1, &ds[n].lab2, 
                               &ds[n].pt1, &ds[n].pt2, 
                               &ds[n].presentation, &ds[n].finalTest);

            if (check >= 2) { 
            int len = strlen(ds[n].tenSV);
            while(len > 0 && (ds[n].tenSV[len-1] == ' ' || ds[n].tenSV[len-1] == '\t' || 
                              ds[n].tenSV[len-1] == '\r' || ds[n].tenSV[len-1] == '\n')) {
                ds[n].tenSV[--len] = '\0';
            }
            tinhDiem(&ds[n]);
            n++;
            }
        }
        
        fclose(fp);
        return true;
    }
const char* layTenCuoi(const char *hoTen) {
	const char *p = strrchr(hoTen, ' ');
	if (p) return p + 1;
	return hoTen;
}
bool daCoDiem(SinhVien sv) {
    return sv.lab1 >= 0 && sv.lab2 >= 0 &&
           sv.pt1 >= 0 && sv.pt2 >= 0 &&
           sv.presentation >= 0 && sv.finalTest >= 0;
}
int tatCaDaChot() {
    int ci;
    for (ci = 0; ci < 6; ci++) if (!colLocked[ci]) return 0;
    return 1;
}
int cotCoDuDiem(int colIndex) {
    int si;
    for (si = 0; si < n; si++) {
        float val;
        if (colIndex == 0) val = ds[si].lab1;
        else if (colIndex == 1) val = ds[si].lab2;
        else if (colIndex == 2) val = ds[si].pt1;
        else if (colIndex == 3) val = ds[si].pt2;
        else if (colIndex == 4) val = ds[si].presentation;
        else val = ds[si].finalTest;
        if (val < 0) return 0;
    }
    return 1;
}
void nhapTheoCot(float *diemCot, int indexSV, char tenCot[], int needFlush) {
    char buf[50];
    int ch;
    
    if (needFlush) {
        while ((ch = getchar()) != '\n' && ch != EOF);
    }
    
    printf("  [%d] %-12s %-30s (Hien tai: ", indexSV + 1, ds[indexSV].maSV, ds[indexSV].tenSV);
    if (*diemCot < 0) printf("Chua co");
    else printf("%.1f", *diemCot);
    printf(") -> ");
    
    fgets(buf, sizeof(buf), stdin);
    
    buf[strcspn(buf, "\r\n")] = '\0';
    
    if (strlen(buf) == 0) {
        printf("         => Bo qua (giu nguyen).\n");
        return;
    }
    
    if (strcmp(buf, "-2") == 0) {
        *diemCot = -999;
        return;
    }
    
    float val = atof(buf);
    if (val < 0 || val > 10) {
        printf("         [!] Diem khong hop le (0-10). Bo qua.\n");
        return;
    }
    *diemCot = val;
    printf("         => Da cap nhat: %.1f\n", val);
}

void nhap(char tenFileChinh[]) {
    int i, lc;
    char c;

    if (tatCaDaChot()) {
        printf("\n[!] TAT CA COT DA CHOT. Khong the nhap diem!\n");
        return;
    }

    if (n == 0) {
        printf("\nDanh sach rong!\n");
        return;
    }

    while (1) {
        printf("\n===== NHAP DIEM THEO COT =====\n");
        printf("1. Lab 1\n");
        printf("2. Lab 2\n");
        printf("3. PT 1\n");
        printf("4. PT 2\n");
        printf("5. Presentation\n");
        printf("6. Final Test\n");
        printf("7. Nhap tat ca cot (lien tuc)\n");
        printf("0. Hoan tat nhap diem\n");
        printf("Chon cot diem de nhap: ");
        scanf("%d", &lc);

        if (lc == 0) break;

        if (lc >= 1 && lc <= 6) {
            char *tenCot[] = {"Lab1", "Lab2", "PT1", "PT2", "Presentation", "Final"};

            if (colLocked[lc - 1]) {
                printf("\n[!] Cot %s DA CHOT. Khong the nhap!\n", tenCot[lc - 1]);
                continue;
            }

            printf("\n--- NHAP COT: %s ---\n", tenCot[lc - 1]);
            printf("(Enter de bo qua, nhap [-2] de dung som)\n\n");

            for (i = 0; i < n; i++) {
                float *diemPtr;
                if (lc == 1) diemPtr = &ds[i].lab1;
                else if (lc == 2) diemPtr = &ds[i].lab2;
                else if (lc == 3) diemPtr = &ds[i].pt1;
                else if (lc == 4) diemPtr = &ds[i].pt2;
                else if (lc == 5) diemPtr = &ds[i].presentation;
                else diemPtr = &ds[i].finalTest;

                float backup = *diemPtr;
                nhapTheoCot(diemPtr, i, tenCot[lc - 1], i == 0);

                if (*diemPtr == -999) {
                    *diemPtr = backup;
                    break;
                }
                tinhDiem(&ds[i]);
            }
            printf("\n=> Da hoan tat cot %s.\n", tenCot[lc - 1]);

        } else if (lc == 7) {
            char *tenCot[] = {"Lab1", "Lab2", "PT1", "PT2", "Presentation", "Final"};
            int col;
            int dungSom = 0;
            for (col = 0; col < 6 && !dungSom; col++) {
                if (colLocked[col]) {
                    printf("\n--- COT %s: DA CHOT (bo qua) ---\n", tenCot[col]);
                    continue;
                }
                printf("\n--- NHAP COT: %s ---\n", tenCot[col]);
                printf("(Enter de bo qua, nhap [-2] de dung som)\n\n");

                for (i = 0; i < n; i++) {
                    float *diemPtr;
                    float backup;
                    if (col == 0) diemPtr = &ds[i].lab1;
                    else if (col == 1) diemPtr = &ds[i].lab2;
                    else if (col == 2) diemPtr = &ds[i].pt1;
                    else if (col == 3) diemPtr = &ds[i].pt2;
                    else if (col == 4) diemPtr = &ds[i].presentation;
                    else diemPtr = &ds[i].finalTest;

                    backup = *diemPtr;
                    nhapTheoCot(diemPtr, i, tenCot[col], i == 0);

                    if (*diemPtr == -999) {
                        *diemPtr = backup;
                        dungSom = 1;
                        break;
                    }
                    tinhDiem(&ds[i]);
                }
                if (!dungSom)
                    printf("\n=> Da hoan tat cot %s.\n", tenCot[col]);
            }
        } else {
            printf("\n[!] Lua chon khong hop le!\n");
            continue;
        }
    }

    printf("\nLuu thay doi vao file %s? (y/n): ", tenFileChinh);
    c = getch();
    if (c == 'y' || c == 'Y') {
        ghiFile(tenFileChinh);
        printf("\nDa luu thanh cong!\n");
    } else {
        printf("\nDa huy luu.\n");
    }
}
void xemDanhSach() {
    int i;
    if (n == 0) { printf("\nDanh sach trong!\n"); return; }

    if (cheDoTong == 0) {
        char tenLopHP[100] = "";
        int foundHP = 0;
        int m, h;
        for (m = 0; m < soMon; m++) {
            for (h = 0; h < dsMonHoc[m].soHocPhan; h++) {
                if (strcmp(dsMonHoc[m].dsHocPhan[h].fileHP, fileHPHienTai) == 0) {
                    sprintf(tenLopHP, "%s_%s", dsMonHoc[m].tenMon, dsMonHoc[m].dsHocPhan[h].maHP);
                    foundHP = 1;
                    break;
                }
            }
            if (foundHP) break;
        }
        if (!foundHP) {
            strcpy(tenLopHP, "SINH VIEN");
        }
        printf("\n                                              DANH SACH LOP HOC PHAN: %s\n", tenLopHP);
        printf("+-----+-------------+-------------+-----------------------------------+-------+-------+-------+-------+-------+-------+-------+------+\n");
        printf("| %-4s| %-12s| %-12s| %-34s| %-6s| %-6s| %-6s| %-6s| %-6s| %-6s| %-6s| %-5s|\n",
               "STT", "MSSV", "LOP", "Ho va ten", "Lab1", "Lab2", "PT1", "PT2", "Pre", "Final", "DTB", "Loai");
        printf("+-----+-------------+-------------+-----------------------------------+-------+-------+-------+-------+-------+-------+-------+------+\n");
    } else {
        printf("\n                               DANH SACH TONG HOP\n");
        printf("+-----+-------------+-------------+-----------------------------------+---------------+----------+\n");
        printf("| %-4s| %-12s| %-12s| %-34s| %-14s| %-9s|\n", "STT", "MSSV", "LOP", "Ho va ten", "DTB", "Loai");
        printf("+-----+-------------+-------------+-----------------------------------+---------------+----------+\n");
    }

    int demA = 0, demB = 0, demC = 0, demD = 0, demF = 0, completed = 0;

    for (i = 0; i < n; i++) {
        char sDTB[10];
        if (!daCoDiem(ds[i])) strcpy(sDTB, " "); else sprintf(sDTB, "%.2f", ds[i].dtb);

        if (cheDoTong == 0) {
            char sLab1[10], sLab2[10], sPt1[10], sPt2[10], sPre[10], sFinal[10];
            if (ds[i].lab1 < 0) strcpy(sLab1, " "); else sprintf(sLab1, "%.1f", ds[i].lab1);
            if (ds[i].lab2 < 0) strcpy(sLab2, " "); else sprintf(sLab2, "%.1f", ds[i].lab2);
            if (ds[i].pt1 < 0) strcpy(sPt1, " "); else sprintf(sPt1, "%.1f", ds[i].pt1);
            if (ds[i].pt2 < 0) strcpy(sPt2, " "); else sprintf(sPt2, "%.1f", ds[i].pt2);
            if (ds[i].presentation < 0) strcpy(sPre, " "); else sprintf(sPre, "%.1f", ds[i].presentation);
            if (ds[i].finalTest < 0) strcpy(sFinal, " "); else sprintf(sFinal, "%.1f", ds[i].finalTest);

            printf("| %-4d| %-12s| %-12s| %-34s| %-6s| %-6s| %-6s| %-6s| %-6s| %-6s| %-6s| %-5s|\n",
                   i + 1, ds[i].maSV, ds[i].lop, ds[i].tenSV, sLab1, sLab2, sPt1, sPt2, sPre, sFinal, sDTB, ds[i].diemChu);
        } else {
            printf("| %-4d| %-12s| %-12s| %-34s| %-14s| %-9s|\n",
                   i + 1, ds[i].maSV, ds[i].lop, ds[i].tenSV, sDTB, daCoDiem(ds[i]) ? ds[i].diemChu : " ");
        }

        if (strcmp(ds[i].diemChu, "A") == 0) { demA++; completed++; }
        else if (strcmp(ds[i].diemChu, "B") == 0) { demB++; completed++; }    
        else if (strcmp(ds[i].diemChu, "C") == 0) { demC++; completed++; }
        else if (strcmp(ds[i].diemChu, "D") == 0) { demD++; completed++; }
        else if (strcmp(ds[i].diemChu, "F") == 0 && daCoDiem(ds[i])) { demF++; completed++; }
    }
    
    if (cheDoTong == 0)
        printf("+-----+-------------+-------------+-----------------------------------+-------+-------+-------+-------+-------+-------+-------+------+\n");
    else
        printf("+-----+-------------+-------------+-----------------------------------+---------------+----------+\n");

    printf("\nTY LE XEP LOAI:\n");
    printf("A: %.2f%% | B: %.2f%% | C: %.2f%% | D: %.2f%% | F: %.2f%%\n",
           completed ? demA * 100.0 / completed : 0, completed ? demB * 100.0 / completed : 0, 
           completed ? demC * 100.0 / completed : 0, completed ? demD * 100.0 / completed : 0, 
           completed ? demF * 100.0 / completed : 0);
    

}
	void xemDiemChiTiet() {
        char signal;
        do {
	    int i; char mssv[20];
	    if (n == 0) { printf("\nDanh sach trong!\n"); return; }
	    printf("\nNhap MSSV: "); scanf("%s", mssv);
        bool notfound = true;
	    for (i = 0; i < n; i++) {
	        if (strcmp(ds[i].maSV, mssv) == 0) {
                notfound = false;
                bool Cohetdiem = daCoDiem(ds[i]);

                char sLab1[10], sLab2[10], sPt1[10], sPt2[10], sPre[10], sFinal[10], sDTB[10];
				if (ds[i].lab1 < 0) strcpy(sLab1, "      "); else sprintf(sLab1, "%-6.1f", ds[i].lab1);
				if (ds[i].lab2 < 0) strcpy(sLab2, "      "); else sprintf(sLab2, "%-6.1f", ds[i].lab2);
				if (ds[i].pt1 < 0) strcpy(sPt1, "      "); else sprintf(sPt1, "%-6.1f", ds[i].pt1);
				if (ds[i].pt2 < 0) strcpy(sPt2, "      "); else sprintf(sPt2, "%-6.1f", ds[i].pt2);
				if (ds[i].presentation < 0) strcpy(sPre, "      "); else sprintf(sPre, "%-6.1f", ds[i].presentation);
				if (ds[i].finalTest < 0) strcpy(sFinal, "      "); else sprintf(sFinal, "%-6.1f", ds[i].finalTest);
				if (!Cohetdiem) strcpy(sDTB, "      "); else sprintf(sDTB, "%-6.2f", ds[i].dtb);

	            printf("+--------------------------------------------------------------------------------+\n");
	            printf("|                               BANG DIEM CHI TIET                               |\n");
	            printf("| MSSV: %-13.13s | LOP: %-12.12s | Ho ten: %-28.28s |\n", ds[i].maSV, ds[i].lop, ds[i].tenSV);
	            printf("| Lab 1&2 (%-3.0f%% - %-3.0f%%): %-6.6s, %-6.6s                                          |\n", wLab*100, wLab*100, sLab1, sLab2);
	            printf("| PT 1&2  (%-3.0f%% - %-3.0f%%): %-6.6s, %-6.6s                                          |\n", wPT*100, wPT*100, sPt1, sPt2);
	            printf("| Pre (%-3.0f%%):            %-6.6s                                                  |\n", wPre*100, sPre);
	            printf("| Final (%-3.0f%%):          %-6.6s                                                  |\n", wFinal*100, sFinal);
	            printf("|--------------------------------------------------------------------------------|\n");
	            printf("| DIEM TRUNG BINH: %-6.6s | XEP LOAI: %-2.2s                                         |\n", sDTB, ds[i].diemChu);
	            printf("+--------------------------------------------------------------------------------+\n");
	            break;
	        }
	    }
	    if (notfound) printf("\nKhong tim thay!\n");
        printf("An c de tiep tuc, an bat ki de thoat");
        signal = getch();
        } while (signal == 'c' || signal == 'C');
	}
	
void sua() {
    char signal;
    do {
    char mssv[20];
    int i;
    if (tatCaDaChot()) {
        printf("\n[!] TAT CA COT DA CHOT. Khong the sua!\n");
        return;
    }
    printf("\nNhap MSSV can sua: ");
    scanf("%s", mssv);
    bool notfound = true;
    for (i = 0; i < n; i++) {
        if (strcmp(ds[i].maSV, mssv) == 0) {
            notfound = false;
            printf("Sua diem cho %s: \n", ds[i].tenSV);
            while(1){
                printf("\n--- MENU SUA DIEM ---\n");
                printf("1. Sua diem lab1, lab2 (Hien tai: %.1f, %.1f) %s %s\n", ds[i].lab1, ds[i].lab2, colLocked[0]?"[CHOT]":"", colLocked[1]?"[CHOT]":"");
                printf("2. Sua diem pt1, pt2 (Hien tai: %.1f, %.1f) %s %s\n", ds[i].pt1, ds[i].pt2, colLocked[2]?"[CHOT]":"", colLocked[3]?"[CHOT]":"");
                printf("3. Sua diem presentation (Hien tai: %.1f) %s\n", ds[i].presentation, colLocked[4]?"[CHOT]":"");
                printf("4. Sua diem finalTest (Hien tai: %.1f) %s\n", ds[i].finalTest, colLocked[5]?"[CHOT]":"");
                printf("5. Sua toan bo diem (chua chot)\n");
                printf("6. Hoan tat sua diem\n");
                printf("Chon: ");
                
                int lc; scanf("%d", &lc);
                if(lc == 1){
                    if (colLocked[0] && colLocked[1]) { printf("  [!] Ca Lab1 va Lab2 da CHOT!\n"); continue; }
                    if (colLocked[0]) printf("  [!] Lab1 da CHOT, chi sua Lab2.\n");
                    if (colLocked[1]) printf("  [!] Lab2 da CHOT, chi sua Lab1.\n");
                    if (!colLocked[0] && !colLocked[1]) {
                        printf("Nhap lai diem lab1, lab2: ");
                        scanf("%f %f", &ds[i].lab1, &ds[i].lab2);
                    } else if (!colLocked[0]) {
                        printf("Nhap lai diem lab1: ");
                        scanf("%f", &ds[i].lab1);
                    } else {
                        printf("Nhap lai diem lab2: ");
                        scanf("%f", &ds[i].lab2);
                    }
                }
                else if(lc == 2){
                    if (colLocked[2] && colLocked[3]) { printf("  [!] Ca PT1 va PT2 da CHOT!\n"); continue; }
                    if (colLocked[2]) printf("  [!] PT1 da CHOT, chi sua PT2.\n");
                    if (colLocked[3]) printf("  [!] PT2 da CHOT, chi sua PT1.\n");
                    if (!colLocked[2] && !colLocked[3]) {
                        printf("Nhap lai diem pt1, pt2: ");
                        scanf("%f %f", &ds[i].pt1, &ds[i].pt2);
                    } else if (!colLocked[2]) {
                        printf("Nhap lai diem pt1: ");
                        scanf("%f", &ds[i].pt1);
                    } else {
                        printf("Nhap lai diem pt2: ");
                        scanf("%f", &ds[i].pt2);
                    }
                }
                else if(lc == 3){
                    if (colLocked[4]) { printf("  [!] Presentation da CHOT!\n"); continue; }
                    printf("Nhap lai diem presentation: ");
                    scanf("%f", &ds[i].presentation);
                }
                else if(lc == 4){
                    if (colLocked[5]) { printf("  [!] Final Test da CHOT!\n"); continue; }
                    printf("Nhap lai diem final test: ");
                    scanf("%f", &ds[i].finalTest);
                }
                else if(lc == 5){
                    if (tatCaDaChot()) { printf("  [!] Tat ca cot da CHOT!\n"); continue; }
                    printf("Diem cu: ");
                    if (!colLocked[0]) printf("L1=%.1f ", ds[i].lab1);
                    if (!colLocked[1]) printf("L2=%.1f ", ds[i].lab2);
                    if (!colLocked[2]) printf("P1=%.1f ", ds[i].pt1);
                    if (!colLocked[3]) printf("P2=%.1f ", ds[i].pt2);
                    if (!colLocked[4]) printf("Pre=%.1f ", ds[i].presentation);
                    if (!colLocked[5]) printf("Final=%.1f", ds[i].finalTest);
                    printf("\nNhap lai cac cot CHUA CHOT: ");
                    if (!colLocked[0]) scanf("%f", &ds[i].lab1);
                    if (!colLocked[1]) scanf("%f", &ds[i].lab2);
                    if (!colLocked[2]) scanf("%f", &ds[i].pt1);
                    if (!colLocked[3]) scanf("%f", &ds[i].pt2);
                    if (!colLocked[4]) scanf("%f", &ds[i].presentation);
                    if (!colLocked[5]) scanf("%f", &ds[i].finalTest);
                }
                else {
                    break;
                }   
            }
            tinhDiem(&ds[i]);
            printf("Da cap nhat tam thoi!\n");
            break;
        }
    }
    if (notfound) printf("Khong tim thay!\n");
    printf("An c de tiep tuc, an bat ki de thoat");
    signal = getch();
    } while (signal == 'c' || signal == 'C');
}

	void UI_Welcome() {
	    printf("______________________________________________________________________________________________________\n");
	    printf("|                                                                                                    |\n");
	    printf("|                                PBL 1: DO AN LAP TRINH TINH TOAN                                    |\n");
	    printf("|                                             NHOM: 5                                                |\n");
	    printf("|                                 DE TAI: QUAN LY DIEM SINH VIEN                                     |\n");
	    printf("|____________________________________________________________________________________________________|\n");
	    printf("\n                                                                      Chao mung ban da quay tro lai");
	    printf("\n");
	}
	void chonFileLamViec(char tenFile[]) {
	    char selected[50];
	    if (chonHocPhan(selected, false, "CHON FILE LAM VIEC")) {
	        strcpy(tenFile, selected);
	        docFile(tenFile);
	    } else {
	        printf("Giu nguyen file lam viec hien tai: %s\n", tenFile);
	    }
	}


void docFileSinhVien() {
    FILE *fp = fopen("sinhvien.txt", "r");
    char line[256];
    nSV = 0;
    if (!fp) return;
    while (fgets(line, sizeof(line), fp) && nSV < MAX) {
        char *p1, *p2;
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) < 3) continue;
        p1 = strchr(line, '\t');
        if (!p1) continue;
        *p1 = '\0';
        p2 = strchr(p1 + 1, '\t');
        if (!p2) continue;
        *p2 = '\0';
        strcpy(dsSV[nSV].maSV, line);
        strcpy(dsSV[nSV].lop, p1 + 1);
        strcpy(dsSV[nSV].tenSV, p2 + 1);
        nSV++;
    }
    fclose(fp);
}

void ghiFileSinhVien() {
    FILE *fp = fopen("sinhvien.txt", "w");
    int i;
    if (!fp) return;
    for (i = 0; i < nSV; i++) {
        fprintf(fp, "%s\t%s\t%s\n", dsSV[i].maSV, dsSV[i].lop, dsSV[i].tenSV);
    }
    fclose(fp);
}

int timSVTheoMSSV(char mssv[]) {
    int i;
    for (i = 0; i < nSV; i++) {
        if (strcmp(dsSV[i].maSV, mssv) == 0) return i;
    }
    return -1;
}


void themSVMoi() {
    char signal;
    do {
        char mssv[20], ten[50], lop[20];
        int ch;

        docFileSinhVien();

        printf("\nNhap MSSV: ");
        scanf("%19s", mssv);

        if (timSVTheoMSSV(mssv) >= 0) {
            printf("\n[!] MSSV %s da ton tai trong danh sach! Huy bo.\n", mssv);
        } else {
            while ((ch = getchar()) != '\n' && ch != EOF);

            printf("Nhap lop: ");
            fgets(lop, sizeof(lop), stdin);
            lop[strcspn(lop, "\r\n")] = '\0';

            printf("Nhap ho ten sinh vien: ");
            fgets(ten, sizeof(ten), stdin);
            ten[strcspn(ten, "\r\n")] = '\0';

            if (nSV >= MAX) {
                printf("\n[!] Danh sach da day!\n");
            } else {
                strcpy(dsSV[nSV].maSV, mssv);
                strcpy(dsSV[nSV].lop, lop);
                strcpy(dsSV[nSV].tenSV, ten);
                nSV++;
                ghiFileSinhVien();
                printf("\n[OK] Da them sinh vien %s - %s vao danh sach!\n", mssv, ten);
            }
        }

        printf("\nAn c de tiep tuc them, an bat ky de thoat");
        signal = getch();
    } while (signal == 'c' || signal == 'C');
}

void themSVVaoMonHoc() {
    char signal;
    do {
        int i;
        docFilemonhoc();
        docFileSinhVien();

        if (nSV == 0) {
            printf("\n[!] Danh sach sinh vien trong! Hay them sinh vien moi truoc.\n");
            return;
        }

        if (soMon == 0) {
            printf("\n[!] Chua co mon hoc nao!\n");
            return;
        }

        printf("\n--- THEM SINH VIEN VAO MON HOC ---\n");
        printf("Danh sach mon hoc:\n");
        for (i = 0; i < soMon; i++) {
            printf("%d. %s\n", i + 1, dsMonHoc[i].tenMon);
        }
        printf("Chon: ");
        int lcMon;
        scanf("%d", &lcMon);

        if (lcMon < 1 || lcMon > soMon) {
            printf("\n[!] Lua chon khong hop le!\n");
            printf("\nAn c de tiep tuc, an bat ky de thoat");
            signal = getch();
            continue;
        }

        int monIdx = lcMon - 1;
        if (dsMonHoc[monIdx].soHocPhan == 0) {
            printf("\n[!] Mon hoc nay chua co hoc phan nao!\n");
            printf("\nAn c de tiep tuc, an bat ky de thoat");
            signal = getch();
            continue;
        }

        printf("\nChon hoc phan cua mon %s:\n", dsMonHoc[monIdx].tenMon);
        for (i = 0; i < dsMonHoc[monIdx].soHocPhan; i++) {
            printf("%d. %s (%s)\n", i + 1, dsMonHoc[monIdx].dsHocPhan[i].maHP, dsMonHoc[monIdx].dsHocPhan[i].fileHP);
        }
        printf("Chon: ");
        int lcHP;
        scanf("%d", &lcHP);

        if (lcHP < 1 || lcHP > dsMonHoc[monIdx].soHocPhan) {
            printf("\n[!] Lua chon khong hop le!\n");
            printf("\nAn c de tiep tuc, an bat ky de thoat");
            signal = getch();
            continue;
        }

        int hpIdx = lcHP - 1;
        char tenFile[50];
        strcpy(tenFile, dsMonHoc[monIdx].dsHocPhan[hpIdx].fileHP);

        docFile(tenFile);

        printf("\n--- THEM SINH VIEN VAO HOC PHAN %s - MON %s ---\n", 
               dsMonHoc[monIdx].dsHocPhan[hpIdx].maHP, dsMonHoc[monIdx].tenMon);
        printf("(Nhap MSSV de them, nhap 0 de dung)\n\n");

        while (1) {
            char mssv[20];
            int idx, found;
            printf("MSSV: ");
            scanf("%19s", mssv);

            if (strcmp(mssv, "0") == 0) break;

            
            idx = timSVTheoMSSV(mssv);
            if (idx < 0) {
                printf("  [!] MSSV %s khong ton tai trong danh sach sinh vien!\n\n", mssv);
                continue;
            }

            
            found = 0;
            for (i = 0; i < n; i++) {
                if (strcmp(ds[i].maSV, mssv) == 0) {
                    found = 1;
                    break;
                }
            }
            if (found) {
                printf("  [!] %s - %s da co trong hoc phan nay roi!\n\n", mssv, dsSV[idx].tenSV);
                continue;
            }

          
            int checkHP;
            bool registeredInOther = false;
            for (checkHP = 0; checkHP < dsMonHoc[monIdx].soHocPhan; checkHP++) {
                if (checkHP == hpIdx) continue;
                if (svCoTrongFile(dsMonHoc[monIdx].dsHocPhan[checkHP].fileHP, mssv)) {
                    printf("  [!] Vi pham! %s - %s da dang ky hoc phan %s cua mon nay roi!\n", 
                           mssv, dsSV[idx].tenSV, dsMonHoc[monIdx].dsHocPhan[checkHP].maHP);
                    printf("      Moi sinh vien chi duoc hoc 1 hoc phan moi mon!\n\n");
                    registeredInOther = true;
                    break;
                }
            }

            if (registeredInOther) {
                continue;
            }

            if (n >= soSVToiDa) {
                printf("  [!] Lop hoc phan da dat so luong sinh vien toi da (%d)! Khong the them.\n", soSVToiDa);
                break;
            }

          
            strcpy(ds[n].maSV, mssv);
            strcpy(ds[n].lop, dsSV[idx].lop);
            strcpy(ds[n].tenSV, dsSV[idx].tenSV);
            ds[n].lab1        = -1;
            ds[n].lab2        = -1;
            ds[n].pt1         = -1;
            ds[n].pt2         = -1;
            ds[n].presentation = -1;
            ds[n].finalTest   = -1;
            ds[n].dtb         = -1;
            strcpy(ds[n].diemChu, " ");
            n++;

            printf("  [OK] Da them %s - %s vao hoc phan %s - mon %s\n\n", 
                   mssv, dsSV[idx].tenSV, dsMonHoc[monIdx].dsHocPhan[hpIdx].maHP, dsMonHoc[monIdx].tenMon);
        }

        ghiFile(tenFile);
        printf("\nDa luu file %s thanh cong!\n", tenFile);

        printf("\nAn c de tiep tuc, an bat ky de thoat");
        signal = getch();
    } while (signal == 'c' || signal == 'C');
}


void loaiBoSinhVienKhoiMonHoc() {
    char file[50];
    if (!chonHocPhan(file, false, "LOAI SINH VIEN KHOI HOC PHAN")) {
        return;
    }
    docFile(file);
    if (n == 0) {
        printf("\nLop hoc phan nay hien tai khong co sinh vien!\n");
        return;
    }
    printf("\n--- LOAI SINH VIEN KHOI HOC PHAN ---\n");
    printf("(Nhap MSSV de loai, nhap 0 de dung)\n\n");
    bool changed = false;
    while (1) {
        char mssv[20];
        printf("MSSV: ");
        scanf("%19s", mssv);
        if (strcmp(mssv, "0") == 0) break;
        
        int idx = -1;
        int i;
        for (i = 0; i < n; i++) {
            if (strcmp(ds[i].maSV, mssv) == 0) {
                idx = i;
                break;
            }
        }
        if (idx < 0) {
            printf("  [!] MSSV %s khong co trong lop hoc phan nay!\n\n", mssv);
            continue;
        }
        
        
        if (ds[idx].finalTest >= 0) {
            printf("  [!] Khong the loai! %s - %s da hoan thanh diem cuoi ky cua lop nay.\n\n", mssv, ds[idx].tenSV);
            continue;
        }
        
       
        char tenRemoved[50];
        strcpy(tenRemoved, ds[idx].tenSV);
        for (i = idx; i < n - 1; i++) {
            ds[i] = ds[i + 1];
        }
        n--;
        changed = true;
        printf("  [OK] Da loai sinh vien %s - %s khoi hoc phan!\n\n", mssv, tenRemoved);
    }
    if (changed) {
        ghiFile(file);
        printf("\nDa cap nhat va luu lai file %s!\n", file);
    }
}

void loaiBoSinhVienKhoiHeThong() {
    docFileSinhVien();
    if (nSV == 0) {
        printf("\nDanh sach sinh vien tong hien tai rong!\n");
        return;
    }
    printf("\n--- LOAI SINH VIEN KHOI HE THONG ---\n");
    printf("(Nhap MSSV de loai, nhap 0 de dung)\n\n");
    bool changedSV = false;
    while (1) {
        char mssv[20];
        printf("MSSV: ");
        scanf("%19s", mssv);
        if (strcmp(mssv, "0") == 0) break;
        
        int idx = timSVTheoMSSV(mssv);
        if (idx < 0) {
            printf("  [!] MSSV %s khong ton tai trong danh sach tong!\n\n", mssv);
            continue;
        }
        
        char tenRemoved[50];
        strcpy(tenRemoved, dsSV[idx].tenSV);
        
        
        int i;
        for (i = idx; i < nSV - 1; i++) {
            dsSV[i] = dsSV[i + 1];
        }
        nSV--;
        changedSV = true;
        
       
        int f, c;
        for (f = 0; f < soMon; f++) {
            for (c = 0; c < dsMonHoc[f].soHocPhan; c++) {
                char fileHP[50];
                strcpy(fileHP, dsMonHoc[f].dsHocPhan[c].fileHP);
                
                if (docFile(fileHP)) {
                    int sectionIdx = -1;
                    for (i = 0; i < n; i++) {
                        if (strcmp(ds[i].maSV, mssv) == 0) {
                            sectionIdx = i;
                            break;
                        }
                    }
                    if (sectionIdx >= 0) {
                        for (i = sectionIdx; i < n - 1; i++) {
                            ds[i] = ds[i + 1];
                        }
                        n--;
                        ghiFile(fileHP);
                        printf("    => Da loai MSSV %s khoi file hoc phan: %s\n", mssv, fileHP);
                    }
                }
            }
        }
        printf("  [OK] Da loai sinh vien %s - %s khoi danh sach tong!\n\n", mssv, tenRemoved);
    }
    if (changedSV) {
        ghiFileSinhVien();
        printf("\nDa cap nhat va luu lai file sinhvien.txt!\n");
    }
}

void loaiBoSinhVien(char tenFile[]) {
    int lc;
    while (1) {
        printf("\n===== MENU LOAI BO SINH VIEN =====\n");
        printf("1. Loai bo sinh vien ra khoi mon hoc (hoc phan)\n");
        printf("2. Loai bo sinh vien ra khoi danh sach tong & tat ca mon hoc\n");
        printf("0. Quay lai\n");
        printf("Chon: ");
        scanf("%d", &lc);
        if (lc == 0) break;
        if (lc == 1) {
            loaiBoSinhVienKhoiMonHoc();
        } else if (lc == 2) {
            loaiBoSinhVienKhoiHeThong();
        } else {
            printf("[!] Lua chon khong hop le!\n");
        }
    }

    if (strlen(tenFile) > 0) {
        docFile(tenFile);
    }
}


void quanLySinhVien(char tenFile[]) {
    int lc;
    while (1) {
        printf("\n===== QUAN LY SINH VIEN =====\n");
        printf("1. Them sinh vien moi (vao danh sach tong)\n");
        printf("2. Them sinh vien vao mon hoc\n");
        printf("3. Loai bo sinh vien\n");
        printf("0. Quay lai\n");
        printf("Chon: ");
        scanf("%d", &lc);
        if (lc == 0) break;

        switch (lc) {
            case 1: themSVMoi(); break;
            case 2: themSVVaoMonHoc(); break;
            case 3: loaiBoSinhVien(tenFile); break;
            default: printf("[!] Lua chon khong hop le!\n"); break;
        }
    }
}
    void Sapxep (char tenFile[]) {
            int tieuChi, kieu, i, j;
  
            printf("\n===== CHON TIEU CHI SAP XEP =====\n");
            printf("1. Sap xep theo Ma so sinh vien (MSSV)\n");
            printf("2. Sap xep theo Ten sinh vien\n");
            printf("3. Sap xep theo Lop\n");
            printf("4. Sap xep theo Diem trung binh (DTB)\n");
            printf("0. Quay lai\n");
            printf("Chon tieu chi (1-4): ");
            scanf("%d", &tieuChi);

            if (tieuChi == 0) return;

            if (tieuChi < 1 || tieuChi > 4) {
                printf("[!] Tieu chi khong hop le!\n");
                return;
            }
           
            char selectedFile[50];
            if (!chonHocPhan(selectedFile, false, "CHON FILE CAN SAP XEP")) {
                return;
            }
            strcpy(tenFile, selectedFile);
            if (!docFile(tenFile)) {
                printf("[!] Khong the doc file!\n");
                return;
            }

            if (n == 0) {
                printf("\nDanh sach trong hoac file khong co du lieu! Khong the sap xep.\n");
                return;
            }
        
            printf("\n1. Tang dan\n2. Giam dan\nChon kieu (1-2): ");
            scanf("%d", &kieu);

            if (kieu < 1 || kieu > 2) {
                printf("[!] Kieu sap xep khong hop le!\n");
                return;
            }
    
            for (i = 0; i < n - 1; i++) {
                for (j = i + 1; j < n; j++) {
                    bool canDoiCho = false;

                    switch (tieuChi) {
                        case 1:
                            if (kieu == 1 && strcmp(ds[i].maSV, ds[j].maSV) > 0) canDoiCho = true;
                            if (kieu == 2 && strcmp(ds[i].maSV, ds[j].maSV) < 0) canDoiCho = true;
                            break;

                        case 2: {
                            int cmpTen = strcmp(layTenCuoi(ds[i].tenSV), layTenCuoi(ds[j].tenSV));
                            if (cmpTen == 0) cmpTen = strcmp(ds[i].tenSV, ds[j].tenSV);
                            if (kieu == 1 && cmpTen > 0) canDoiCho = true;
                            if (kieu == 2 && cmpTen < 0) canDoiCho = true;
                            break;
                        }

                        case 3:
                            if (kieu == 1 && strcmp(ds[i].lop, ds[j].lop) > 0) canDoiCho = true;
                            if (kieu == 2 && strcmp(ds[i].lop, ds[j].lop) < 0) canDoiCho = true;
                            break;

                        case 4:
                            if (kieu == 1 && ds[i].dtb > ds[j].dtb) canDoiCho = true;
                            if (kieu == 2 && ds[i].dtb < ds[j].dtb) canDoiCho = true;
                            break;
                    }

                    if (canDoiCho) {
                        SinhVien t = ds[i];
                        ds[i] = ds[j];
                        ds[j] = t;
                    }
                }
            }

            printf("\n=> Da sap xep xong! Duoi day la danh sach sau khi sap xep:\n");
            xemDanhSach();

            char cLuu;
            printf("\nBan co muon luu thu tu sap xep moi nay vao file %s khong? (y/n): ", tenFile);
            fflush(stdin);
            cLuu = getch();
            if (cLuu == 'y' || cLuu == 'Y') {
                ghiFile(tenFile);
                printf("\nDa luu thay doi thu tu vao file thanh cong!\n");
            } else {
                printf("\nChi hien thi tam thoi, khong thay doi trong file.\n");
            }
    }
    void Chotcotdiem(char tenFile[]) {
        	int lcChot;
	        printf("\n===== CHOT COT DIEM =====\n");
	        int ci;
	        for (ci = 0; ci < 6; ci++) {
	            printf("%d. %-15s [%s]\n", ci + 1, tenCotGlobal[ci], colLocked[ci] ? "DA CHOT" : "CHUA CHOT");
	        }
	        printf("7. Chot TAT CA cot chua chot\n");
	        printf("0. Quay lai\n");
	        printf("Chon cot de chot: ");
	        scanf("%d", &lcChot);
	        
	        if (lcChot >= 1 && lcChot <= 6) {
	            int idx = lcChot - 1;
	            if (colLocked[idx]) {
	                printf("\n[!] Cot %s da chot roi!\n", tenCotGlobal[idx]);
	            } else if (!cotCoDuDiem(idx)) {
	                printf("\n[!] Cot %s con sinh vien CHUA CO DIEM. Khong the chot!\n", tenCotGlobal[idx]);
	            } else {
	                printf("\nXac nhan CHOT cot %s? (y/n): ", tenCotGlobal[idx]);
	                if (getch() == 'y') {
	                    colLocked[idx] = 1;
	                    ghiFile(tenFile);
	                    printf("\nDa chot cot %s thanh cong!\n", tenCotGlobal[idx]);
	                } else {
	                    printf("\nDa huy.\n");
	                }
	            }
	        } else if (lcChot == 7) {
	            int coTheChot = 0, khongDu = 0;
	            for (ci = 0; ci < 6; ci++) {
	                if (!colLocked[ci]) {
	                    if (cotCoDuDiem(ci)) coTheChot++;
	                    else khongDu++;
	                }
	            }
	            if (coTheChot == 0) {
	                printf("\n[!] Khong co cot nao du dieu kien de chot!\n");
	                if (khongDu > 0) printf("    (%d cot con thieu diem)\n", khongDu);
	            } else {
	                printf("\nSe chot %d cot du dieu kien", coTheChot);
	                if (khongDu > 0) printf(" (%d cot thieu diem se bo qua)", khongDu);
	                printf(". Xac nhan? (y/n): ");
	                if (getch() == 'y') {
	                    for (ci = 0; ci < 6; ci++) {
	                        if (!colLocked[ci] && cotCoDuDiem(ci)) {
	                            colLocked[ci] = 1;
	                            printf("  => Da chot cot %s\n", tenCotGlobal[ci]);
	                        } else if (!colLocked[ci]) {
	                            printf("  => Bo qua cot %s (thieu diem)\n", tenCotGlobal[ci]);
	                        }
	                    }
	                    ghiFile(tenFile);
	                    printf("\nChot thanh cong!\n");
	                } else {
	                    printf("\nDa huy.\n");
	                }
	            }
	        }
    }

void themMonHoc() {
    int ch, i, j;
    char tenMon_new[30];
    float wL, wP, wPre_new, wF;
    int soSVMax;
    int soHP;
   
    while ((ch = getchar()) != '\n' && ch != EOF);

    printf("\n===== THEM MON HOC MOI =====\n");

    docFilemonhoc(); 

    if (soMon >= MAX_MON) {
        printf("[!] Da dat toi da so mon hoc!\n");
        return;
    }

    
    bool trung = true;
    do {
        printf("Nhap ten mon hoc (vd: Tin, Anh, ...): ");
        fgets(tenMon_new, sizeof(tenMon_new), stdin);
        tenMon_new[strcspn(tenMon_new, "\r\n")] = '\0';

        if (strlen(tenMon_new) == 0) {
            printf("[!] Ten mon khong duoc de trong! Vui long nhap lai.\n");
            trung = true;
            continue;
        }

        trung = false;
        for (i = 0; i < soMon; i++) {
            if (strcmp(dsMonHoc[i].tenMon, tenMon_new) == 0) {
                printf("[!] Mon '%s' da ton tai! Vui long nhap ten khac.\n", tenMon_new);
                trung = true;
                break;
            }
        }
    } while (trung);

    
    printf("Nhap so luong sinh vien toi da cua hoc phan: ");
    scanf("%d", &soSVMax);
    if (soSVMax < 1 || soSVMax > MAX) {
        printf("[!] So luong sinh vien toi da khong hop le (1-%d). Vui long nhap lai!\n", MAX);
        return;
    }

    
    float tong;
    do {
        while ((ch = getchar()) != '\n' && ch != EOF);
        printf("\nNhap trong so (tong = 1.0):\n");
        printf("  w_Lab  (moi lab, co 2 bai) : ");
        scanf("%f", &wL);
        printf("  w_PT   (moi PT,  co 2 bai) : ");
        scanf("%f", &wP);
        printf("  w_Pre  (Presentation)       : ");
        scanf("%f", &wPre_new);
        printf("  w_Final (Final Test)        : ");
        scanf("%f", &wF);

        tong = wL * 2 + wP * 2 + wPre_new + wF;
        printf("  => Tong kiem tra: %.2f (phai = 1.0)\n", tong);
        if (tong < 0.99f || tong > 1.01f) {
            printf("[!] Tong trong so != 1.0. Vui long nhap lai!\n");
        }
    } while (tong < 0.99f || tong > 1.01f);

  
    printf("\nNhap so luong hoc phan cua mon nay: ");
    scanf("%d", &soHP);
    if (soHP < 1 || soHP > MAX_HP) {
        printf("[!] So hoc phan khong hop le (1-%d)!\n", MAX_HP);
        return;
    }

    char tenLower[30];
    int lenLower = 0;
    for (i = 0; tenMon_new[i]; i++) {
        char cChar = tenMon_new[i];
        if (cChar != ' ' && cChar != '\t') {
            if (cChar >= 'A' && cChar <= 'Z') {
                tenLower[lenLower++] = cChar + 32;
            } else {
                tenLower[lenLower++] = cChar;
            }
        }
    }
    tenLower[lenLower] = '\0';

    MonHoc newMon;
    strcpy(newMon.tenMon, tenMon_new);
    newMon.soHocPhan = soHP;

    while ((ch = getchar()) != '\n' && ch != EOF);

    for (i = 0; i < soHP; i++) {
        char maHP_new[20];
        char duongDan[60];
        
        printf("  Nhap ma hoc phan %d (vd: NH25, NH26): ", i + 1);
        fgets(maHP_new, sizeof(maHP_new), stdin);
        maHP_new[strcspn(maHP_new, "\r\n")] = '\0';
        
        if (strlen(maHP_new) == 0) {
            printf("  [!] Ma hoc phan khong duoc de trong! Vui long nhap lai.\n");
            i--;
            continue;
        }

        bool dup = false;
        for (j = 0; j < i; j++) {
            if (strcmp(newMon.dsHocPhan[j].maHP, maHP_new) == 0) {
                dup = true;
                break;
            }
        }
        if (dup) {
            printf("  [!] Ma hoc phan '%s' bi trung! Vui long nhap lai.\n", maHP_new);
            i--;
            continue;
        }

        char maHPLower[20];
        int lenHP = 0;
        for (j = 0; maHP_new[j]; j++) {
            char cChar = maHP_new[j];
            if (cChar != ' ' && cChar != '\t') {
                if (cChar >= 'A' && cChar <= 'Z') {
                    maHPLower[lenHP++] = cChar + 32;
                } else {
                    maHPLower[lenHP++] = cChar;
                }
            }
        }
        maHPLower[lenHP] = '\0';

        sprintf(duongDan, "Res/%s_%s.dat", tenLower, maHPLower);

        FILE *check = fopen(duongDan, "r");
        if (check) {
            fclose(check);
            printf("  [!] File '%s' da ton tai tren disk!\n", duongDan);
            printf("      Ban co muon ghi de va dung file nay? (y/n): ");
            ch = getch();
            printf("\n");
            if (ch != 'y' && ch != 'Y') {
                printf("  [!] Nhap lai hoc phan %d.\n", i + 1);
                i--;
                continue;
            }
        }

        FILE *fp = fopen(duongDan, "w");
        if (!fp) {
            printf("  [!] Khong the tao file '%s'. Kiem tra thu muc Res/ da ton tai chua!\n", duongDan);
            i--;
            continue;
        }
        fprintf(fp, "%f %f %f %f 0 0 0 0 0 0 %d\n", wL, wP, wPre_new, wF, soSVMax);
        fclose(fp);

        strcpy(newMon.dsHocPhan[i].maHP, maHP_new);
        strcpy(newMon.dsHocPhan[i].fileHP, duongDan);
    }

    dsMonHoc[soMon] = newMon;
    soMon++;
   
    ghiFilemonhoc();

    printf("\n[OK] Da tao mon hoc '%s' voi %d hoc phan:\n", tenMon_new, soHP);
    for (i = 0; i < soHP; i++) {
        printf("  - Hoc phan: %s -> File: %s\n", newMon.dsHocPhan[i].maHP, newMon.dsHocPhan[i].fileHP);
    }
    printf("     So SV toi da : %d\n", soSVMax);
    printf("     Trong so     : Lab=%.2f PT=%.2f Pre=%.2f Final=%.2f\n",
           wL, wP, wPre_new, wF);
    printf("     monhoc.txt   : Da cap nhat.\n");
}

	int main() {
	    char tenFile[50] = "";
		char choice;
	    docFilemonhoc();
	    UI_Welcome();
	    do {
	        printf("\n");
	        printf("                       MENU (%s)\n", tenFile);
	        if (strlen(tenFile) > 0) {
	            int ci;
	            printf("  Trang thai chot: ");
	            for (ci = 0; ci < 6; ci++) {
	                printf("%s:%s ", tenCotGlobal[ci], colLocked[ci] ? "CHOT" : "MO");
	            }
	            printf("\n");
	        }
	        printf("+----------------------------------------------------------------------+\n");
	        printf("|         1. Nhap diem                  2. Xem danh sach               |\n");
	        printf("|         3. Sua diem                   4. Sap xep                     |\n");
	        printf("|         5. Xem diem (MSSV)            6. Chot cot diem               |\n");
	        printf("|         7. Quan ly sinh vien          8. Them mon hoc moi            |\n");
	        printf("|         9. Thoat                                                     |\n");
	        printf("+----------------------------------------------------------------------+\n");
	        printf("   Nhap so de chon tinh nang: ");
	        scanf(" %c", &choice); 
	        switch (choice) {
	        case '1':
	    chonFileLamViec(tenFile);
	    nhap(tenFile);
	    break;
	
	       case '2': {
	    int subLc;
	    printf("\n===== XEM DANH SACH =====\n");
	    printf("1. Xem danh sach sinh vien tong\n");
	    printf("2. Xem bang diem hoc phan\n");
	    printf("0. Quay lai\n");
	    printf("Chon: ");
	    scanf("%d", &subLc);
	    if (subLc == 1) {
	        docFileSinhVien();
	        if (nSV == 0) {
	            printf("\nDanh sach sinh vien tong rong!\n");
	        } else {
	            int i;
	            printf("\n                       DANH SACH SINH VIEN TONG\n");
	            printf("+-------------+-------------+-----------------------------------+\n");
	            printf("| %-12s| %-12s| %-34s|\n", "MSSV", "LOP", "Ho va ten");
	            printf("+-------------+-------------+-----------------------------------+\n");
	            for (i = 0; i < nSV; i++) {
	                printf("| %-12s| %-12s| %-34s|\n", dsSV[i].maSV, dsSV[i].lop, dsSV[i].tenSV);
	            }
	            printf("+-------------+-------------+-----------------------------------+\n");
	        }
	    } else if (subLc == 2) {
	        char file[50];
	        if (chonHocPhan(file, false, "CHON HOC PHAN CAN XEM")) {
	            docFile(file);
	            cheDoTong = 0;
	            xemDanhSach();
	        }
	    }
	    break;
	}
	
	        case '3':
	    chonFileLamViec(tenFile);
	    sua();
	    ghiFile(tenFile);
	    break;
	
	        case '4': {
            Sapxep(tenFile);
            break;
        }
	
	        case '5':
	    chonFileLamViec(tenFile);
	    xemDiemChiTiet();
	    break;
	
	       case '6': {
	    chonFileLamViec(tenFile);
	    if (tatCaDaChot()) {
	        printf("\nTat ca cot da chot roi!\n");
	    } else {
            Chotcotdiem(tenFile);
	    }
	    break;
	}
	
	       case '7':
    quanLySinhVien(tenFile);
    break;

          case '8':
    themMonHoc();
    break;

          case '9':
    break;
	    }
	
	} while (choice != '9');
	    return 0;
	}