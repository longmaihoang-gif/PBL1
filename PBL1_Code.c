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

void tinhDiem(SinhVien *sv) {
    sv->dtb = (sv->lab1 + sv->lab2) * wLab 
            + (sv->pt1 + sv->pt2) * wPT 
            + sv->presentation * wPre 
            + sv->finalTest * wFinal;

    if (sv->dtb >= 8.5) strcpy(sv->diemChu, "A");
    else if (sv->dtb >= 7.0) strcpy(sv->diemChu, "B");
    else if (sv->dtb >= 5.5) strcpy(sv->diemChu, "C");
    else if (sv->dtb >= 4.0) strcpy(sv->diemChu, "D");
    else strcpy(sv->diemChu, "F");
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
                ds[i].maSV, ds[i].tenSV, ds[i].lab1, ds[i].lab2, ds[i].pt1, ds[i].pt2, ds[i].presentation, ds[i].finalTest);
    }
    fclose(fp);
}

bool docFile(char tenFile[]) {
    FILE *fp = fopen(tenFile, "r");
    int len;
    if (!fp){
        printf("file khong ton tai\n");
        return false;
    }
    char line[200];
    if (fgets(line, sizeof(line), fp)) {
        int count = sscanf(line, "%f %f %f %f %d", &wLab, &wPT, &wPre, &wFinal, &isLocked);
        if (count == 4) isLocked = 0;
        else if (count < 4) {
            wLab = 0.1; wPT = 0.1; wPre = 0.2; wFinal = 0.4; isLocked = 0;
            rewind(fp);
        }
    }

    n = 0;
    while (n < MAX && fscanf(fp, "%s", ds[n].maSV) != EOF) {
        if (fscanf(fp, " %[^0-9]", ds[n].tenSV) != 1) break;
        len = strlen(ds[n].tenSV);
        while(len > 0 && (ds[n].tenSV[len-1] == ' ' || ds[n].tenSV[len-1] == '\t')) 
            ds[n].tenSV[--len] = '\0';
        if (fscanf(fp, "%f %f %f %f %f %f", &ds[n].lab1, &ds[n].lab2, &ds[n].pt1, &ds[n].pt2, &ds[n].presentation, &ds[n].finalTest) == 6) {
            tinhDiem(&ds[n]);
            n++;
        }
    }
    fclose(fp);
    return true;
}
void nhap(char tenFileChinh[]) {
    int i, j, n_cu, n1, trung;
    char tamMSSV[20], c;
    if (isLocked) {
        printf("\n[!] FILE DA CHOT. Khong the nhap them!\n");
        return;
    }
    n_cu = n; 
    printf("\nNhap so sinh vien muon them: ");
    scanf("%d", &n1);
    for (i = 0; i < n1; i++) {
        if (n >= MAX) break;
        do {
            trung = 0;
            printf("\nNhap MSSV sinh vien thu %d: ", i + 1);
            scanf("%s", tamMSSV);
            for (j = 0; j < n; j++) {
                if (strcmp(ds[j].maSV, tamMSSV) == 0) {
                    printf("Loi: MSSV da ton tai!\n");
                    trung = 1; break;
                }
            }
        } while (trung == 1);
        strcpy(ds[n].maSV, tamMSSV);
        getchar(); 
        printf("Ten: ");
        fgets(ds[n].tenSV, sizeof(ds[n].tenSV), stdin);
        ds[n].tenSV[strcspn(ds[n].tenSV, "\n")] = 0;
        printf("Diem (L1 L2 P1 P2 Pre Final): ");
        scanf("%f %f %f %f %f %f", &ds[n].lab1, &ds[n].lab2, &ds[n].pt1, &ds[n].pt2, &ds[n].presentation, &ds[n].finalTest);
        tinhDiem(&ds[n]);
        n++;
    }
    printf("\nLuu vao file? (y/n): ");
    c = getch();
    if (c == 'y' || c == 'Y') ghiFile(tenFileChinh);
    else n = n_cu; 
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

    

    int demA=0, demB=0, demC=0, demD=0, demF=0;

    for (i = 0; i < n; i++) {

        if (cheDoTong == 0) {
           
            printf("| %-12s| %-35s| %-5.1f| %-5.1f| %-5.1f| %-5.1f| %-5.1f| %-7.1f| %-6.2f| %-5s|\n",
                ds[i].maSV, ds[i].tenSV,
                ds[i].lab1, ds[i].lab2,
                ds[i].pt1, ds[i].pt2,
                ds[i].presentation,
                ds[i].finalTest,
                ds[i].dtb,
                ds[i].diemChu);
        } else {
            
            printf("| %-12s| %-35s| %-15.2f| %-10s|\n",
                ds[i].maSV, ds[i].tenSV, ds[i].dtb, ds[i].diemChu);
        }

        
        if (strcmp(ds[i].diemChu,"A")==0) demA++;
        else if (strcmp(ds[i].diemChu,"B")==0) demB++;
        else if (strcmp(ds[i].diemChu,"C")==0) demC++;
        else if (strcmp(ds[i].diemChu,"D")==0) demD++;
        else demF++;
    }
    
    if(cheDoTong == 0)
    printf("+-------------------------------------------------------------------------------------------------------------+\n");
    else
    printf("+-------------------------------------------------------------------------------+\n");

    printf("\nTY LE XEP LOAI:\n");
    printf("A: %.2f%% | B: %.2f%% | C: %.2f%% | D: %.2f%% | F: %.2f%%\n",
        demA*100.0/n, demB*100.0/n, demC*100.0/n, demD*100.0/n, demF*100.0/n);
    
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
            printf("\n+------------------------------------------------------------------+");
            printf("\n|                        BANG DIEM CHI TIET                        |");
            printf("\n| MSSV: %-12s | Ho ten: %-35s |", ds[i].maSV, ds[i].tenSV);
            printf("\n| Lab 1&2 (%-3.0f%% - %-3.0f%%): %-6.1f, %-6.1f                            |", wLab*100, wLab*100, ds[i].lab1, ds[i].lab2);
            printf("\n| PT 1&2 (%-3.0f%% - %-3.0f%%):  %-6.1f, %-6.1f                            |", wPT*100, wPT*100, ds[i].pt1, ds[i].pt2);
            printf("\n| Pre (%-3.0f%%):     %-6.1f                                           |", wPre*100, ds[i].presentation);
            printf("\n| Final (%-3.0f%%):   %-6.1f                                           |", wFinal*100, ds[i].finalTest);
            printf("\n|------------------------------------------------------------------|");
            printf("\n| DIEM TRUNG BINH: %-6.2f | XEP LOAI: %-28s |", ds[i].dtb, ds[i].diemChu);
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
                printf("1. Sua diem lab1, lab2\n");
                printf("2. Sua diem pt1, pt2\n");
                printf("3. Sua diem presentation\n");
                printf("4. Sua diem finalTest\n");
                printf("5. Sua toan bo diem\n");
                printf("6. Hoan tat sua diem\n");
                printf("Chon: ");
                int lc; scanf("%d",&lc);
                if(lc==1){
                    printf("Nhap lai diem lab1, lab2: ");
                    scanf("%f %f",&ds[i].lab1,&ds[i].lab2);
                }
                else if(lc==2){
                    printf("Nhap lai diem pt1, pt2: ");
                    scanf("%f %f",&ds[i].pt1,&ds[i].pt2);
                }
                else if(lc==3){
                    printf("Nhap lai diem presentation: ");
                    scanf("%f",&ds[i].presentation);
                }
                else if(lc==4){
                    printf("Nhap lai diem final test: ");
                    scanf("%f",&ds[i].finalTest);
                }
                else if(lc==5){
                    printf("Nhap lai toan bo diem: ");
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
    int nToan=0, nLy=0, nHoa=0;
    int i, j;

    
    docFile("toan.txt");
    nToan = n;
    for(i=0;i<nToan;i++) toan[i] = ds[i];

    docFile("ly.txt");
    nLy = n;
    for(i=0;i<nLy;i++) ly[i] = ds[i];

    docFile("hoa.txt");
    nHoa = n;
    for(i=0;i<nHoa;i++) hoa[i] = ds[i];

    
    n = 0;

    
    for(i=0;i<nToan;i++){
        for(j=0;j<nLy;j++){
            if(strcmp(toan[i].maSV, ly[j].maSV)==0){
                int k;
                for(k=0;k<nHoa;k++){
                    if(strcmp(toan[i].maSV, hoa[k].maSV)==0){

                        strcpy(ds[n].maSV, toan[i].maSV);
                        strcpy(ds[n].tenSV, toan[i].tenSV);

                        
                        ds[n].dtb = (toan[i].dtb + ly[j].dtb + hoa[k].dtb)/3;

                      
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
int main() {
    char tenFile[50], choice;
    int i, j;
    UI_Welcome();
    printf("Nhap ten file: "); scanf("%s", tenFile);
    if(!docFile(tenFile)) return;
    do {
        printf("\n");
        printf("                       MENU (%s) [%s]\n", tenFile, isLocked ? "DA CHOT" : "MO");
        printf("+----------------------------------------------------------------------+\n");
        printf("|         1. Nhap diem                  2. Xem danh sach               |\n");
        printf("|         3. Sua diem                   4. Sap xep                     |\n");
        printf("|         5. Xem diem (MSSV)            6. CHOT VINH VIEN              |\n");
        printf("|         7. Luu & Thoat                8. Chuyen file                 |\n");
        printf("+----------------------------------------------------------------------+\n");
        printf("   Nhap so de chon tinh nang: ");
        scanf(" %c", &choice); 
        switch (choice) {
        case '1': nhap(tenFile); break;

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
            // Tai lai file chinh de tranh ghi de du lieu sai
            docFile(tenFile);
            break;
        }

        case '3': sua(); break;

        case '4': {
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

        case '5': xemDiemChiTiet(); break;

        case '6': 
            if (!isLocked) {
                printf("\nXac nhan CHOT? (y/n): ");
                if (getch() == 'y') {
                    isLocked = 1;
                    ghiFile(tenFile);
                    printf("\nChot diem thanh cong!\n");
                } else printf("\nThao tac bi huy boi nguoi dung\n");
            } else printf("\nDa chot roi!\n");
            break;

        case '7': 
            ghiFile(tenFile); 
            break;

        case '8': {
            printf("\nNhap ten file muon chuyen sang: ");
            scanf("%s", tenFile);

            docFile(tenFile);
            printf("\nDa chuyen sang file: %s\n", tenFile);
            break;
        }
    }

} while (choice != '7');
    return 0;
}