	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <conio.h>
	#include <stdbool.h>
	#define MAX 100
	int cheDoTong = 0;
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
	    printf("1. Toan\n2. Ly\n3. Hoa\n4. Tong hop\nChon: ");
	    scanf("%d", &lc);
	
	    if (lc == 1) strcpy(tenFile, "toan.dat");
	    else if (lc == 2) strcpy(tenFile, "ly.dat");
	    else if (lc == 3) strcpy(tenFile, "hoa.dat");
	    else if (lc == 4) strcpy(tenFile, "tonghop"); 
	    docFile(tenFile);
	}
	void ghiFile(char tenFile[]) {
    FILE *fp = fopen(tenFile, "w"); 
    int i;
    if (!fp) return;
    
    fprintf(fp, "%f %f %f %f %d %d %d %d %d %d\n", wLab, wPT, wPre, wFinal, colLocked[0], colLocked[1], colLocked[2], colLocked[3], colLocked[4], colLocked[5]);
    
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
        if (sscanf(line, "%f %f %f %f %d %d %d %d %d %d", &wLab, &wPT, &wPre, &wFinal, &colLocked[0], &colLocked[1], &colLocked[2], &colLocked[3], &colLocked[4], &colLocked[5]) < 4) {
            wLab = 0.1; wPT = 0.1; wPre = 0.2; wFinal = 0.4;
            int ci; for(ci=0;ci<6;ci++) colLocked[ci]=0;
            rewind(fp); 
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
            while(len > 0 && (ds[n].tenSV[len-1] == ' ' || ds[n].tenSV[len-1] == '\t' || ds[n].tenSV[len-1] == '\r' || ds[n].tenSV[len-1] == '\n')) {
                ds[n].tenSV[--len] = '\0';
            }
            
            tinhDiem(&ds[n]);
            n++;
        }
    }
    fclose(fp);
    return true;
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
        for (i = 0; i < n; i++) temp[i] = ds[i];
    
        int j;
        for (i = 0; i < n - 1; i++) {
            for (j = i + 1; j < n; j++) {
                if (temp[i].dtb < temp[j].dtb) {
                    SinhVien t = temp[i];
                    temp[i] = temp[j];
                    temp[j] = t;
                }
            }
        }
    
        int limit = n < 9 ? n : 9;
    
        for (i = 0; i < limit; i++) {
            if (temp[i].dtb < 0) break;
            char loaiHB;
            if (i < 3) loaiHB = 'A';
            else if (i < 6) loaiHB = 'B';
            else loaiHB = 'C';
    
            printf("| %-12s| %-12s| %-25s| DTB: %-5.2f | Hoc bong: %c |\n",
                   temp[i].maSV, temp[i].lop, temp[i].tenSV, temp[i].dtb, loaiHB);
        }
    }
}
	void xemDiemChiTiet() {
	    int i; char mssv[20];
	    if (n == 0) { printf("\nDanh sach trong!\n"); return; }
	    printf("\nNhap MSSV: "); scanf("%s", mssv);
	    for (i = 0; i < n; i++) {
	        if (strcmp(ds[i].maSV, mssv) == 0) {

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
	            return;
	        }
	    }
	    printf("\nKhong tim thay!\n");
	}
	
void sua() {
    char mssv[20];
    int i;
    if (tatCaDaChot()) {
        printf("\n[!] TAT CA COT DA CHOT. Khong the sua!\n");
        return;
    }
    printf("Nhap MSSV can sua: ");
    scanf("%s", mssv);
    for (i = 0; i < n; i++) {
        if (strcmp(ds[i].maSV, mssv) == 0) {
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
            return;
        }
    }
    printf("Khong tim thay!\n");
}
void tongHop3File() {
    SinhVien toan[MAX], ly[MAX], hoa[MAX];
    int nToan = 0, nLy = 0, nHoa = 0;
    int i, j, k;

   
    if (docFile("toan.dat")) {
        nToan = n;
        for(i = 0; i < nToan; i++) toan[i] = ds[i];
    }

    if (docFile("ly.dat")) {
        nLy = n;
        for(i = 0; i < nLy; i++) ly[i] = ds[i];
    }

    if (docFile("hoa.dat")) {
        nHoa = n;
        for(i = 0; i < nHoa; i++) hoa[i] = ds[i];
    }

    
    n = 0;

    
    for(i = 0; i < nToan; i++) {
        for(j = 0; j < nLy; j++) {
            if(strcmp(toan[i].maSV, ly[j].maSV) == 0) {
                for(k = 0; k < nHoa; k++) {
                    if(strcmp(toan[i].maSV, hoa[k].maSV) == 0) {
                        
                        strcpy(ds[n].maSV, toan[i].maSV);
                        strcpy(ds[n].tenSV, toan[i].tenSV);

                       
                        ds[n].dtb = (toan[i].dtb + ly[j].dtb + hoa[k].dtb) / 3.0;

                        
                        if (ds[n].dtb >= 8.5) strcpy(ds[n].diemChu, "A");
                        else if (ds[n].dtb >= 7.0) strcpy(ds[n].diemChu, "B");
                        else if (ds[n].dtb >= 5.5) strcpy(ds[n].diemChu, "C");
                        else if (ds[n].dtb >= 4.0) strcpy(ds[n].diemChu, "D");
                        else strcpy(ds[n].diemChu, "F");

                        n++;
                    }
                }
            }
        }
    }
    
    if (n == 0) {
        printf("\n[!] Khong tim thay sinh vien nao co du diem ca 3 mon de tong hop!\n");
    }
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
	    printf("1. Toan\n2. Ly\n3. Hoa\nChon: ");
	    scanf("%d", &lc);
	
	    if (lc == 1) strcpy(tenFile, "toan.dat");
	    else if (lc == 2) strcpy(tenFile, "ly.dat");
	    else if (lc == 3) strcpy(tenFile, "hoa.dat");
	    else {
	        printf("Lua chon khong hop le!\n");
	        strcpy(tenFile, "toan.dat"); 
	    }
	
	    docFile(tenFile);
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
        else {
            return;
        }
    }

   
    char temp[20];
    strcpy(temp, ma);

    ma[0] = '1';
    strcpy(ma + 1, temp);
}
void themSinhVien() {
    do {
    char tenMoi[50];
    char lop[50];
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    printf("\nNhap lop: ");
    fgets(lop, sizeof(lop), stdin);
    lop[strcspn(lop, "\n")] = '\0';

    printf("Nhap ten sinh vien moi: ");
    fgets(tenMoi, sizeof(tenMoi), stdin);
    tenMoi[strcspn(tenMoi, "\n")] = '\0';

    char files[][20] = {"toan.dat", "ly.dat", "hoa.dat"};

    int f;

    for (f = 0; f < 3; f++) {

        docFile(files[f]);

        if (n >= MAX) {
            printf("\n[!] File %s da day!\n", files[f]);
            continue;
        }

        
        if (n == 0) {
            strcpy(ds[n].maSV, "10225000001");
        } else {
            strcpy(ds[n].maSV, ds[n - 1].maSV);
            tangMaSV(ds[n].maSV);
        }

        
        strcpy(ds[n].tenSV, tenMoi);
        strcpy(ds[n].lop, lop);

     
        ds[n].lab1 = -1;
        ds[n].lab2 = -1;
        ds[n].pt1 = -1;
        ds[n].pt2 = -1;
        ds[n].presentation = -1;
        ds[n].finalTest = -1;

        ds[n].dtb = -1;
        strcpy(ds[n].diemChu, " ");

        n++;

        ghiFile(files[f]);
    }

    printf("\nDa them sinh vien vao ca 3 file thanh cong!\nNhan c de tiep tuc, nhan bat ky de dung");
    } while (getch()=='c');
}
	int main() {
	    char tenFile[50] = "";
		char choice;
	    int i, j;
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
	        printf("|         7. Them sinh vien             8.thoat                        |\n");
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
	        tongHop3File();
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
           
            if (n == 0) {
                printf("\n[!] RAM chua co du lieu. Vui long chon file de nap du lieu truoc!\n");
                chonFileLamViec(tenFile); 
            }

          
            if (n == 0) {
                printf("\nDanh sach trong hoac file khong co du lieu! Khong the sap xep.\n");
                break;
            }

           
            int tieuChi, kieu;
            printf("\n===== CHON TIEU CHI SAP XEP =====\n");
            printf("1. Sap xep theo Ma so sinh vien (MSSV)\n");
            printf("2. Sap xep theo Ten sinh vien\n");
            printf("3. Sap xep theo Lop\n");
            printf("4. Sap xep theo Diem trung binh (DTB)\n");
            printf("Chon tieu chi (1-4): ");
            scanf("%d", &tieuChi);

            if (tieuChi < 1 || tieuChi > 4) {
                printf("[!] Tieu chi khong hop le!\n");
                break;
            }

            printf("\n1. Tang dan\n2. Giam dan\nChon kieu (1-2): ");
            scanf("%d", &kieu);

            if (kieu < 1 || kieu > 2) {
                printf("[!] Kieu sap xep khong hop le!\n");
                break;
            }

  
            for (i = 0; i < n - 1; i++) {
                for (j = i + 1; j < n; j++) {
                    bool canDoiCho = false;

                    switch (tieuChi) {
                        case 1: 
                            if (kieu == 1 && strcmp(ds[i].maSV, ds[j].maSV) > 0) canDoiCho = true;
                            if (kieu == 2 && strcmp(ds[i].maSV, ds[j].maSV) < 0) canDoiCho = true;
                            break;

                        case 2: 
                            if (kieu == 1 && strcmp(ds[i].tenSV, ds[j].tenSV) > 0) canDoiCho = true;
                            if (kieu == 2 && strcmp(ds[i].tenSV, ds[j].tenSV) < 0) canDoiCho = true;
                            break;

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
	    break;
	}
	
	       case '7':
    themSinhVien();
    break;

          case '8':
    break;
	    }
	
	} while (choice != '8');
	    return 0;
	}