	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <conio.h>
	#include <stdbool.h>
	#define MAX 100
	int cheDoTong = 0;
	typedef struct {
	    char maSV[20];
	    char tenSV[50];
	    float lab1, lab2, pt1, pt2, presentation, finalTest;
	    float dtb;
	    char diemChu[3];
	} SinhVien;
	
	SinhVien ds[MAX];
	int n = 0;
	int isLocked = 0; 
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
    
    fprintf(fp, "%f %f %f %f %d\n", wLab, wPT, wPre, wFinal, isLocked);
    
    for (i = 0; i < n; i++) {
        fprintf(fp, "%s %s %f %f %f %f %f %f\n", 
                ds[i].maSV, 
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
        if (sscanf(line, "%f %f %f %f %d", &wLab, &wPT, &wPre, &wFinal, &isLocked) < 4) {
            wLab = 0.1; wPT = 0.1; wPre = 0.2; wFinal = 0.4; isLocked = 0;
            rewind(fp); 
        }
    }

    n = 0;
    while (fgets(line, sizeof(line), fp) && n < MAX) {
        if (strlen(line) < 5) continue;

       
        int check = sscanf(line, "%19s %[^0-9.-] %f %f %f %f %f %f", 
                           ds[n].maSV, 
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
void nhap(char tenFileChinh[]) {
    int i;
    char c;

    if (isLocked) {
        printf("\n[!] FILE DA CHOT. Khong the nhap diem!\n");
        return;
    }

    if (n == 0) {
        printf("\nDanh sach rong!\n");
        return;
    }

    printf("\n===== NHAP DIEM CHO DANH SACH =====\n");

    for (i = 0; i < n; i++) {
        printf("\n[%d] %s - %s\n", i + 1, ds[i].maSV, ds[i].tenSV);
        
        bool coDiem = daCoDiem(ds[i]);
        if (coDiem) {
            printf("  -> Diem cu: L1:%.1f, L2:%.1f, P1:%.1f, P2:%.1f, Pre:%.1f, Final:%.1f\n",
                    ds[i].lab1, ds[i].lab2, ds[i].pt1, ds[i].pt2, ds[i].presentation, ds[i].finalTest);
            printf("  Nhap moi (hoac [-1] de GIU NGUYEN): ");
        } else {
            printf("  Chua co diem. Nhap (L1 L2 P1 P2 Pre Final): ");
        }

        float l1;
        if (scanf("%f", &l1) != 1) break;

       
        if (l1 == -2) break; 

        if (l1 == -1) {
            if (coDiem) {
                printf("  -> Da giu nguyen diem cu.\n");
            } else {
                printf("  [!] SV nay chua co diem, khong the dung -1!\n");
                i--;
                continue;
            }
        } else {
           
            ds[i].lab1 = l1;
            scanf("%f %f %f %f %f", &ds[i].lab2, &ds[i].pt1, &ds[i].pt2, &ds[i].presentation, &ds[i].finalTest);
            tinhDiem(&ds[i]);
            printf("  -> Cap nhat thanh cong.\n");
        }

       
        if (i < n - 1) {
            int tiepTuc;
            printf("  => Nhap [-2] de DUNG & LUU | Nhap [-3] de TIEP TUC: ");
            scanf("%d", &tiepTuc);
            if (tiepTuc == -2) break;
           
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
	        printf("\n                                          DANH SACH SINH VIEN\n");
	        printf("+-------------------------------------------------------------------------------------------------------------+\n");
	        printf("| %-12s| %-35s| %-5s| %-5s| %-5s| %-5s| %-5s| %-7s| %-6s| %-5s|\n",
	            "MSSV", "Ho ten", "L1", "L2", "P1", "P2", "Pre", "Final", "DTB", "Loai");
	        printf("+-------------------------------------------------------------------------------------------------------------+\n");
	    } else {
	        printf("\n                             DANH SACH SINH VIEN\n");
	        printf("+-------------------------------------------------------------------------------+\n");
	        printf("| %-12s| %-35s| %-15s| %-10s|\n", "MSSV", "Ho va ten", "DTB", "Loai");
	        printf("+-------------------------------------------------------------------------------+\n");
	    }
	
	    
	
	    int demA=0, demB=0, demC=0, demD=0, demF=0, completed=0;
	
	    for (i = 0; i < n; i++) {

			char sDTB[10];
			if (!daCoDiem(ds[i])) strcpy(sDTB, "      "); else sprintf(sDTB, "%-6.2f", ds[i].dtb);
	
	        if (cheDoTong == 0) {

				char sLab1[10], sLab2[10], sPt1[10], sPt2[10], sPre[10], sFinal[10];
				if (ds[i].lab1 < 0) strcpy(sLab1, "     "); else sprintf(sLab1, "%-5.1f", ds[i].lab1);
				if (ds[i].lab2 < 0) strcpy(sLab2, "     "); else sprintf(sLab2, "%-5.1f", ds[i].lab2);
				if (ds[i].pt1 < 0) strcpy(sPt1, "     "); else sprintf(sPt1, "%-5.1f", ds[i].pt1);
				if (ds[i].pt2 < 0) strcpy(sPt2, "     "); else sprintf(sPt2, "%-5.1f", ds[i].pt2);
				if (ds[i].presentation < 0) strcpy(sPre, "     "); else sprintf(sPre, "%-5.1f", ds[i].presentation);
				if (ds[i].finalTest < 0) strcpy(sFinal, "       "); else sprintf(sFinal, "%-7.1f", ds[i].finalTest);

	            printf("| %-12s| %-35s| %s| %s| %s| %s| %s| %s| %s| %-5s|\n",
                    ds[i].maSV, ds[i].tenSV, 
                    sLab1, sLab2,
					sPt1, sPt2,
					sPre,
					sFinal,
                    sDTB, ds[i].diemChu);
	        } else {
	            
	            printf("| %-12s| %-35s| %-15s| %-10s|\n",
	                ds[i].maSV, ds[i].tenSV, sDTB, daCoDiem(ds[i])? ds[i].diemChu: "");
	        }
	
	        
	        if (strcmp(ds[i].diemChu,"A")==0){demA++; completed++;}
	        else if (strcmp(ds[i].diemChu,"B")==0){demB++; completed++;}	
	        else if (strcmp(ds[i].diemChu,"C")==0){demC++; completed++;}
	        else if (strcmp(ds[i].diemChu,"D")==0){demD++; completed++;}
	        else if (strcmp(ds[i].diemChu,"F")==0 && daCoDiem(ds[i])) {demF++; completed++;}
	    }
	    
	    if(cheDoTong == 0)
	    printf("+-------------------------------------------------------------------------------------------------------------+\n");
	    else
	    printf("+-------------------------------------------------------------------------------+\n");
	
	    printf("\nTY LE XEP LOAI:\n");
	    printf("A: %.2f%% | B: %.2f%% | C: %.2f%% | D: %.2f%% | F: %.2f%%\n",
	        completed? demA*100.0/completed: 0, completed? demB*100.0/completed: 0, completed? demC*100.0/completed: 0, completed? demD*100.0/completed: 0, completed? demF*100.0/completed: 0);
	    
	if (cheDoTong == 1) {
	    printf("\n                   *===== DANH SACH HOC BONG =====*\n\n");
	
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
            if(temp[i].dtb < 0) break;

	        char loaiHB;
	
	        if (i < 3) loaiHB = 'A';
	        else if (i < 6) loaiHB = 'B';
	        else loaiHB = 'C';
	
	        printf("| %-12s| %-25s| DTB: %-5.2f | Hoc bong: %c |\n",
	            temp[i].maSV, temp[i].tenSV, temp[i].dtb, loaiHB);
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

	            printf("\n+------------------------------------------------------------------+");
	            printf("\n|                        BANG DIEM CHI TIET                        |");
	            printf("\n| MSSV: %-12s | Ho ten: %-35s |", ds[i].maSV, ds[i].tenSV);
	            printf("\n| Lab 1&2 (%-3.0f%% - %-3.0f%%): %s, %s                            |", wLab*100, wLab*100, sLab1, sLab2);
	            printf("\n| PT 1&2 (%-3.0f%% - %-3.0f%%):  %s, %s                            |", wPT*100, wPT*100, sPt1, sPt2);
	            printf("\n| Pre (%-3.0f%%):     %s                                           |", wPre*100, sPre);
	            printf("\n| Final (%-3.0f%%):   %s                                           |", wFinal*100, sFinal);
	            printf("\n|------------------------------------------------------------------|");
	            printf("\n| DIEM TRUNG BINH: %s | XEP LOAI: %-28s |", sDTB, ds[i].diemChu);
	            printf("\n+------------------------------------------------------------------+\n");
	            return;
	        }
	    }
	    printf("\nKhong tim thay!\n");
	}
	
void sua() {
    char mssv[20];
    int i;
    if (isLocked) {
        printf("\n[!] Du lieu da CHOT. Khong the sua!\n");
        return;
    }
    printf("Nhap MSSV can sua: ");
    scanf("%s", mssv);
    for (i = 0; i < n; i++) {
        if (strcmp(ds[i].maSV, mssv) == 0) {
            printf("Sua diem cho %s: \n", ds[i].tenSV);
            while(1){
                printf("\n--- MENU SUA DIEM ---\n");
                printf("1. Sua diem lab1, lab2 (Hien tai: %.1f, %.1f)\n", ds[i].lab1, ds[i].lab2);
                printf("2. Sua diem pt1, pt2 (Hien tai: %.1f, %.1f)\n", ds[i].pt1, ds[i].pt2);
                printf("3. Sua diem presentation (Hien tai: %.1f)\n", ds[i].presentation);
                printf("4. Sua diem finalTest (Hien tai: %.1f)\n", ds[i].finalTest);
                printf("5. Sua toan bo diem\n");
                printf("6. Hoan tat sua diem\n");
                printf("Chon: ");
                
                int lc; scanf("%d", &lc);
                if(lc == 1){
                    printf("Nhap lai diem lab1, lab2: ");
                    scanf("%f %f", &ds[i].lab1, &ds[i].lab2);
                }
                else if(lc == 2){
                    printf("Nhap lai diem pt1, pt2: ");
                    scanf("%f %f", &ds[i].pt1, &ds[i].pt2);
                }
                else if(lc == 3){
                    printf("Nhap lai diem presentation: ");
                    scanf("%f", &ds[i].presentation);
                }
                else if(lc == 4){
                    printf("Nhap lai diem final test: ");
                    scanf("%f", &ds[i].finalTest);
                }
                else if(lc == 5){
                    printf("Diem cu: L1=%.1f, L2=%.1f, P1=%.1f, P2=%.1f, Pre=%.1f, Final=%.1f\n", 
                            ds[i].lab1, ds[i].lab2, ds[i].pt1, ds[i].pt2, ds[i].presentation, ds[i].finalTest);
                    printf("Nhap lai toan bo 6 cot diem: ");
                    scanf("%f %f %f %f %f %f", &ds[i].lab1, &ds[i].lab2, &ds[i].pt1, &ds[i].pt2, &ds[i].presentation, &ds[i].finalTest);
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
	int main() {
	    char tenFile[50] = "";
		char choice;
	    int i, j;
	    UI_Welcome();
	    do {
	        printf("\n");
	        printf("                       MENU (%s) [%s]\n", tenFile, isLocked ? "DA CHOT" : "MO");
	        printf("+----------------------------------------------------------------------+\n");
	        printf("|         1. Nhap diem                  2. Xem danh sach               |\n");
	        printf("|         3. Sua diem                   4. Sap xep                     |\n");
	        printf("|         5. Xem diem (MSSV)            6. CHOT VINH VIEN              |\n");
	        printf("|         7. Thoat                                                     |\n");
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
	        	chonFileLamViec(tenFile); 
	            int kieu;
	            printf("\n1. Tang dan\n2. Giam dan\nChon: ");
	            scanf("%d", &kieu);
	            int i; 
	            for (i = 0; i < n - 1; i++) {
	                for (j = i + 1; j < n; j++) {
	                    if ((kieu == 1 && ds[i].dtb > ds[j].dtb) ||   
	                        (kieu == 2 && ds[i].dtb < ds[j].dtb)) {
	                        SinhVien t = ds[i];
	                        ds[i] = ds[j];
	                        ds[j] = t;
	                    }
	                }
	            }
	            xemDanhSach();
	            break;
	        }
	
	        case '5':
	    chonFileLamViec(tenFile);
	    xemDiemChiTiet();
	    break;
	
	       case '6':
	    chonFileLamViec(tenFile);
	    if (!isLocked) {
	        printf("\nXac nhan CHOT? (y/n): ");
	        if (getch() == 'y') {
	            isLocked = 1;
	            ghiFile(tenFile);
	            printf("\nChot diem thanh cong!\n");
	        }
	    } else printf("\nDa chot roi!\n");
	    break;
	
	        case '7':
	    break; 
	    }
	
	} while (choice != '7');
	    return 0;
	}
