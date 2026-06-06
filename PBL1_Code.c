#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <conio.h>
	#include <stdbool.h>
	#define MAX 100
    #define MAX_MON 100
    #define MIN_TC 3
	int cheDoTong = 0;

	typedef struct {
	    char maSV[20];
	    char lop[20];
	    char tenSV[50];
	    float lab1, lab2, pt1, pt2, presentation, finalTest;
	    float dtb;
	    char diemChu[3];
	    int soTinChiTichLuy;
	} SinhVien;
	
	SinhVien ds[MAX];
	int n = 0;
		int colLocked[6] = {0, 0, 0, 0, 0, 0};
	char *tenCotGlobal[] = {"Lab1", "Lab2", "PT1", "PT2", "Presentation", "Final"};
	float wLab, wPT, wPre, wFinal;
	int soTinChi = 0;
	
	char tenMon[MAX_MON][30];
	char fileMon[MAX_MON][50];
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
	            fprintf(fp, "Toan\tRes/toan.dat\n");
	            fprintf(fp, "Ly\tRes/ly.dat\n");
	            fprintf(fp, "Hoa\tRes/hoa.dat\n");
	            fclose(fp);
	        }
	        soMon = 3;
	        strcpy(tenMon[0], "Toan"); strcpy(fileMon[0], "Res/toan.dat");
	        strcpy(tenMon[1], "Ly");   strcpy(fileMon[1], "Res/ly.dat");
	        strcpy(tenMon[2], "Hoa");  strcpy(fileMon[2], "Res/hoa.dat");
	        return;
	    }
	    soMon = 0;
	    char line[256];
	    while (fgets(line, sizeof(line), fp) && soMon < MAX_MON) {
	        /* Bo qua dong trong */
	        line[strcspn(line, "\r\n")] = '\0';
	        if (strlen(line) < 3) continue;
	        /* Tach ten mon va duong dan theo dau TAB */
	        char *tab = strchr(line, '\t');
	        if (!tab) continue;
	        *tab = '\0';
	        char *ten  = line;
	        char *file = tab + 1;
	        /* Bo khoang trang thua o dau file path */
	        while (*file == ' ' || *file == '\t') file++;
	        if (strlen(ten) == 0 || strlen(file) == 0) continue;
	        strncpy(tenMon[soMon],  ten,  29); tenMon[soMon][29]  = '\0';
	        strncpy(fileMon[soMon], file, 49); fileMon[soMon][49] = '\0';
	        soMon++;
	    }
	    fclose(fp);
	}
	
	bool docFile(char tenFile[]);
	bool daCoDiem(SinhVien sv);
	
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
	void chonFileXem(char tenFile[]) {
	    int lc;
	    printf("\nChon file xem:\n");
	    int i;
	    for (i = 0; i < soMon; i++) {
	        printf("%d. %s\n", i + 1, tenMon[i]);
	    }
	    printf("%d. Tong hop\nChon: ", soMon + 1);
	    scanf("%d", &lc);

	    if (lc >= 1 && lc <= soMon) {
	        strcpy(tenFile, fileMon[lc - 1]);
	    } else if (lc == soMon + 1) {
	        strcpy(tenFile, "tonghop");
	    } else {
	        printf("Lua chon khong hop le!\n");
	        if (soMon > 0) strcpy(tenFile, fileMon[0]);
	        else strcpy(tenFile, "");
	    }
	    if (strlen(tenFile) > 0) {
	        docFile(tenFile);
	    }
	}
	void ghiFile(char tenFile[]) {
        FILE *fp = fopen(tenFile, "w"); 
        int i;
        if (!fp) return;
    
        fprintf(fp, "%f %f %f %f %d %d %d %d %d %d %d\n", 
                    wLab, wPT, wPre, wFinal,
                    colLocked[0], colLocked[1], colLocked[2], colLocked[3], colLocked[4], colLocked[5],
                    soTinChi);
    
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
               soTinChi = 0;
               rewind(fp); 
            } else {
                soTinChi = (parsed >= 11) ? tc : 0;
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
        printf("\n                                              DANH SACH SINH VIEN\n");
        printf("+-------------+-------------+-----------------------------------+-------+-------+-------+-------+-------+-------+-------+------+\n");
        printf("| %-12s| %-12s| %-34s| %-6s| %-6s| %-6s| %-6s| %-6s| %-6s| %-6s| %-5s|\n",
               "MSSV", "LOP", "Ho va ten", "Lab1", "Lab2", "PT1", "PT2", "Pre", "Final", "DTB", "Loai");
        printf("+-------------+-------------+-----------------------------------+-------+-------+-------+-------+-------+-------+-------+------+\n");
    } else {
        printf("\n                               DANH SACH TONG HOP\n");
        printf("+-------------+-------------+-----------------------------------+---------------+----------+\n");
        printf("| %-12s| %-12s| %-34s| %-14s| %-9s|\n", "MSSV", "LOP", "Ho va ten", "DTB", "Loai");
        printf("+-------------+-------------+-----------------------------------+---------------+----------+\n");
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

            
            printf("| %-12s| %-12s| %-34s| %-6s| %-6s| %-6s| %-6s| %-6s| %-6s| %-6s| %-5s|\n",
                   ds[i].maSV, ds[i].lop, ds[i].tenSV, sLab1, sLab2, sPt1, sPt2, sPre, sFinal, sDTB, ds[i].diemChu);
        } else {
            printf("| %-12s| %-12s| %-34s| %-14s| %-9s|\n",
                   ds[i].maSV, ds[i].lop, ds[i].tenSV, sDTB, daCoDiem(ds[i]) ? ds[i].diemChu : " ");
        }

        if (strcmp(ds[i].diemChu, "A") == 0) { demA++; completed++; }
        else if (strcmp(ds[i].diemChu, "B") == 0) { demB++; completed++; }    
        else if (strcmp(ds[i].diemChu, "C") == 0) { demC++; completed++; }
        else if (strcmp(ds[i].diemChu, "D") == 0) { demD++; completed++; }
        else if (strcmp(ds[i].diemChu, "F") == 0 && daCoDiem(ds[i])) { demF++; completed++; }
    }
    
    if (cheDoTong == 0)
        printf("+-------------+-------------+-----------------------------------+-------+-------+-------+-------+-------+-------+-------+------+\n");
    else
        printf("+-------------+-------------+-----------------------------------+---------------+----------+\n");

    printf("\nTY LE XEP LOAI:\n");
    printf("A: %.2f%% | B: %.2f%% | C: %.2f%% | D: %.2f%% | F: %.2f%%\n",
           completed ? demA * 100.0 / completed : 0, completed ? demB * 100.0 / completed : 0, 
           completed ? demC * 100.0 / completed : 0, completed ? demD * 100.0 / completed : 0, 
           completed ? demF * 100.0 / completed : 0);
    
    if (cheDoTong == 1) {
        printf("\n                    *===== DANH SACH HOC BONG =====*\n\n");
    
        SinhVien temp[MAX];
        int countHB = 0;
        for (i = 0; i < n; i++) {
            if (ds[i].soTinChiTichLuy >= MIN_TC) {
                temp[countHB++] = ds[i];
            }
        }
    
        int j;
        for (i = 0; i < countHB - 1; i++) {
            for (j = i + 1; j < countHB; j++) {
                if (temp[i].dtb < temp[j].dtb) {
                    SinhVien t = temp[i];
                    temp[i] = temp[j];
                    temp[j] = t;
                }
            }
        }
    
        int limit = countHB < 9 ? countHB : 9;
    
        for (i = 0; i < limit; i++) {
            if (temp[i].dtb < 0) break;
            char loaiHB;
            if (i < 3) loaiHB = 'A';
            else if (i < 6) loaiHB = 'B';
            else loaiHB = 'C';
    
            printf("| %-12s| %-12s| %-25s| DTB: %-5.2f | TC: %-2d | Hoc bong: %c |\n",
                   temp[i].maSV, temp[i].lop, temp[i].tenSV, temp[i].dtb, temp[i].soTinChiTichLuy, loaiHB);
        }
    }
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
void tongHopFile() {
    if (soMon == 0 || !docFile((char*)fileMon[0])) {n = 0; return;}
    SinhVien tempDS[MAX];
    int nTemp = n;
    int i, f, j;
    for (i = 0; i < nTemp; i++) {
        tempDS[i] = ds[i];
    }
    float tongDiemNhanTinChi[MAX];
    int tongTinChiSV[MAX];
    int soMonCompleted[MAX];
    for (i = 0; i < nTemp; i++) {
        tongDiemNhanTinChi[i] = 0.0f;
        tongTinChiSV[i] = 0;
        soMonCompleted[i] = 0;
    }
    for (f = 0; f < soMon; f++) {
        if (!docFile((char*)fileMon[f])) continue;
        // docFile se load soTinChi cua mon nay vao bien toan cuc soTinChi
        int tc = (soTinChi > 0) ? soTinChi : 1; // Fallback neu tin chi chua set
        
        for (i = 0; i < nTemp; i++) {
            for (j = 0; j < n; j++) {
                if (strcmp(tempDS[i].maSV, ds[j].maSV) == 0) {
                    if (daCoDiem(ds[j])) {
                        tongDiemNhanTinChi[i] += ds[j].dtb * tc;
                        tongTinChiSV[i] += tc;
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
            if (tongTinChiSV[i] > 0) {
                ds[n].dtb = tongDiemNhanTinChi[i] / (float)tongTinChiSV[i];
            } else {
                ds[n].dtb = 0.0f;
            }
            ds[n].soTinChiTichLuy = tongTinChiSV[i];
    
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
	    int lc;
	    printf("\nChon file lam viec:\n");
	    int i;
	    for (i = 0; i < soMon; i++) {
	        printf("%d. %s\n", i + 1, tenMon[i]);
	    }
	    printf("Chon: ");
	    scanf("%d", &lc);
	
	    if (lc >= 1 && lc <= soMon) {
	        strcpy(tenFile, fileMon[lc - 1]);
	    } else {
	        printf("Lua chon khong hop le!\n");
	        if (soMon > 0) strcpy(tenFile, fileMon[0]);
	        else strcpy(tenFile, "");
	    }
	
	    if (strlen(tenFile) > 0) {
	        docFile(tenFile);
	    }
	}
	void tangMaSV(char ma[]) {
        int len = strlen(ma);
        int i = len - 1;

        while (i >= 0) {
            if (ma[i] >= '0' && ma[i] <= '8') {
                ma[i]++;
                return;
            }
            else if (ma[i] == '9') {
                ma[i] = '0';
                i--;
            }
           else return;
        }
   
        char temp[20];
         strcpy(temp, ma);

        ma[0] = '1';
        strcpy(ma + 1, temp);
    }
/* === Doc/Ghi file sinhvien.txt (danh sach tong) === */
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

/* === Tinh nang 1: Them sinh vien moi vao danh sach === */
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

/* === Tinh nang 2: Them sinh vien vao mon hoc === */
void themSVVaoMonHoc() {
    char signal;
    do {
        int lcFile, i;
        char tenFile[50];
        char mssv[20];

        docFilemonhoc();
        docFileSinhVien();

        if (nSV == 0) {
            printf("\n[!] Danh sach sinh vien trong! Hay them sinh vien moi truoc.\n");
            return;
        }

        printf("\nChon mon hoc:\n");
        for (i = 0; i < soMon; i++) {
            printf("%d. %s\n", i + 1, tenMon[i]);
        }
        printf("Chon: ");
        scanf("%d", &lcFile);

        if (lcFile < 1 || lcFile > soMon) {
            printf("\n[!] Lua chon khong hop le!\n");
            printf("\nAn c de tiep tuc, an bat ky de thoat");
            signal = getch();
            continue;
        }

        strcpy(tenFile, fileMon[lcFile - 1]);
        docFile(tenFile);

        printf("\n--- THEM SINH VIEN VAO MON %s ---\n", tenMon[lcFile - 1]);
        printf("(Nhap MSSV de them, nhap 0 de dung)\n\n");

        while (1) {
            int idx, found;
            printf("MSSV: ");
            scanf("%19s", mssv);

            if (strcmp(mssv, "0") == 0) break;

            /* Kiem tra ton tai trong danh sach tong */
            idx = timSVTheoMSSV(mssv);
            if (idx < 0) {
                printf("  [!] MSSV %s khong ton tai trong danh sach sinh vien!\n\n", mssv);
                continue;
            }

            /* Kiem tra da co trong mon chua */
            found = 0;
            for (i = 0; i < n; i++) {
                if (strcmp(ds[i].maSV, mssv) == 0) {
                    found = 1;
                    break;
                }
            }
            if (found) {
                printf("  [!] %s - %s da co trong mon nay roi!\n\n", mssv, dsSV[idx].tenSV);
                continue;
            }

            if (n >= MAX) {
                printf("  [!] Danh sach day! Khong the them.\n");
                break;
            }

            /* Them sinh vien vao mang diem - 6 cot diem mac dinh -1 */
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

            printf("  [OK] Da them %s - %s vao mon %s\n\n", mssv, dsSV[idx].tenSV, tenMon[lcFile - 1]);
        }

        ghiFile(tenFile);
        printf("\nDa luu file %s thanh cong!\n", tenFile);

        printf("\nAn c de tiep tuc, an bat ky de thoat");
        signal = getch();
    } while (signal == 'c' || signal == 'C');
}

/* === Menu chinh: Them sinh vien === */
void themSinhVien() {
    int lc;
    printf("\n===== THEM SINH VIEN =====\n");
    printf("1. Them sinh vien moi (vao danh sach tong)\n");
    printf("2. Them sinh vien vao mon hoc\n");
    printf("0. Quay lai\n");
    printf("Chon: ");
    scanf("%d", &lc);

    switch (lc) {
        case 1: themSVMoi(); break;
        case 2: themSVVaoMonHoc(); break;
        default: break;
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
           
            printf("\nChon file can sap xep:\n");
            for (i = 0; i < soMon; i++)
                printf("%d. %s\n", i + 1, tenMon[i]);
            printf("Chon: ");
            int lcFile;
            scanf("%d", &lcFile);

            if (lcFile < 1 || lcFile > soMon) {
                printf("[!] Lua chon file khong hop le!\n");
                return;
            }

            strcpy(tenFile, fileMon[lcFile - 1]);
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
    int ch, i;
    char tenMon_new[30];
    char duongDan[60];   /* Res/xxx.dat */
    float wL, wP, wPre_new, wF;
    int soTC;
   
    while ((ch = getchar()) != '\n' && ch != EOF);

    printf("\n===== THEM MON HOC MOI =====\n");
   
    printf("Nhap ten mon hoc (vd: Tin, Anh, ...): ");
    fgets(tenMon_new, sizeof(tenMon_new), stdin);
    tenMon_new[strcspn(tenMon_new, "\r\n")] = '\0';

    if (strlen(tenMon_new) == 0) {
        printf("[!] Ten mon khong duoc de trong!\n");
        return;
    }

    for (i = 0; i < soMon; i++) {
        if (strcmp(tenMon[i], tenMon_new) == 0) {
            printf("[!] Mon '%s' da ton tai trong danh sach!\n", tenMon_new);
            return;
        }
    }

    if (soMon >= MAX_MON) {
        printf("[!] Da dat toi da so mon hoc!\n");
        return;
    }

    char tenLower[30];
    strcpy(tenLower, tenMon_new);
    for (i = 0; tenLower[i]; i++)
        if (tenLower[i] >= 'A' && tenLower[i] <= 'Z')
            tenLower[i] += 32;
    sprintf(duongDan, "Res/%s.dat", tenLower);

    {
        FILE *check = fopen(duongDan, "r");
        if (check) {
            fclose(check);
            printf("[!] File '%s' da ton tai tren disk!\n", duongDan);
            printf("    Ban co muon ghi de va dung file nay? (y/n): ");
            ch = getch();
            if (ch != 'y' && ch != 'Y') {
                printf("\nDa huy.\n");
                return;
            }
            printf("\n");
        }
    }

    /* Nhap so tin chi */
    printf("\nNhap so tin chi cua mon hoc: ");
    scanf("%d", &soTC);
    if (soTC < 1 || soTC > 10) {
        printf("[!] So tin chi khong hop le (1-10). Vui long nhap lai!\n");
        return;
    }

    /* Nhap trong so */
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

    float tong = wL * 2 + wP * 2 + wPre_new + wF;
    printf("  => Tong kiem tra: %.2f (phai = 1.0)\n", tong);
    if (tong < 0.99f || tong > 1.01f) {
        printf("[!] Tong trong so != 1.0. Vui long nhap lai!\n");
        return;
    }

    /* Tao file moi chi co header, khong co sinh vien nao */
    {
        FILE *fp = fopen(duongDan, "w");
        if (!fp) {
            printf("[!] Khong the tao file '%s'. Kiem tra thu muc Res/ da ton tai chua!\n", duongDan);
            return;
        }
        /* Header: trong so + trang thai chot (tat ca = 0) + so tin chi */
        fprintf(fp, "%f %f %f %f 0 0 0 0 0 0 %d\n", wL, wP, wPre_new, wF, soTC);
        fclose(fp);
    }

    strcpy(tenMon[soMon], tenMon_new);
    strcpy(fileMon[soMon], duongDan);
    soMon++;
   
    {
        FILE *fp = fopen("monhoc.txt", "w");
        if (fp) {
            for (i = 0; i < soMon; i++)
                fprintf(fp, "%s\t%s\n", tenMon[i], fileMon[i]);
            fclose(fp);
        }
    }

    printf("\n[OK] Da tao mon hoc '%s':\n", tenMon_new);
    printf("     File du lieu : %s\n", duongDan);
    printf("     So tin chi   : %d\n", soTC);
    printf("     Trong so     : Lab=%.2f PT=%.2f Pre=%.2f Final=%.2f\n",
           wL, wP, wPre_new, wF);
    printf("     Danh sach SV : Trong (them SV qua menu 'Them sinh vien')\n");
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
	        printf("|         7. Them sinh vien             8. Them mon hoc moi            |\n");
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
	    char file[50];
	    chonFileXem(file);
	
	    if (strcmp(file, "tonghop") == 0) {
	        tongHopFile();
	        cheDoTong = 1;
	        xemDanhSach();
	        cheDoTong = 0;
	    } else {
	        docFile(file);
	        cheDoTong = 0;
	        xemDanhSach();
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
    themSinhVien();
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
