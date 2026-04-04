#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>

#define MAX 100

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
float wLab = 0.1, wPT = 0.1, wPre = 0.2, wFinal = 0.4;

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

void docFile(char tenFile[]) {
    FILE *fp = fopen(tenFile, "r");
    int len;
    if (!fp) return;

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
    printf("\nDANH SACH SINH VIEN (Trong so: %.0f%%-%.0f%%-%.0f%%-%.0f%%)\n", wLab*100, wPT*100, wPre*100, wFinal*100);
    printf("----------------------------------------------------------------------------------------------------------\n");
    printf("%-12s %-25s %-5s %-5s %-5s %-5s %-5s %-5s %-6s %-5s\n", 
           "MSSV", "Ho va ten", "L1", "L2", "P1", "P2", "Pre", "End", "DTB", "Loai");
    printf("----------------------------------------------------------------------------------------------------------\n");
    for (i = 0; i < n; i++) {
        printf("%-12s %-25s %-5.1f %-5.1f %-5.1f %-5.1f %-5.1f %-5.1f %-6.1f %-5s\n", 
               ds[i].maSV, ds[i].tenSV, ds[i].lab1, ds[i].lab2, ds[i].pt1, ds[i].pt2, ds[i].presentation, ds[i].finalTest, ds[i].dtb, ds[i].diemChu);
    }
    printf("----------------------------------------------------------------------------------------------------------\n");
}

void xemDiemChiTiet() {
    int i; char mssv[20];
    if (n == 0) { printf("\nDanh sach trong!\n"); return; }
    printf("\nNhap MSSV: "); scanf("%s", mssv);
    for (i = 0; i < n; i++) {
        if (strcmp(ds[i].maSV, mssv) == 0) {
            printf("\n--- BANG DIEM CHI TIET ---");
            printf("\nMSSV: %s | Ho ten: %s", ds[i].maSV, ds[i].tenSV);
            printf("\nLab 1&2 (%.0f%%): %.1f, %.1f", wLab*100, ds[i].lab1, ds[i].lab2);
            printf("\nPT 1&2 (%.0f%%):  %.1f, %.1f", wPT*100, ds[i].pt1, ds[i].pt2);
            printf("\nPre (%.0f%%):     %.1f", wPre*100, ds[i].presentation);
            printf("\nFinal (%.0f%%):   %.1f", wFinal*100, ds[i].finalTest);
            printf("\n--------------------------");
            printf("\nDIEM TRUNG BINH: %.2f | XEP LOAI: %s\n", ds[i].dtb, ds[i].diemChu);
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
int main() {
    char tenFile[50], choice;
    int i, j;
    printf("Nhap ten file: "); scanf("%s", tenFile);
    docFile(tenFile);
    do {
        printf("\n===== MENU (%s) [%s] =====\n", tenFile, isLocked ? "DA CHOT" : "MO");
        printf("1. Nhap diem\n2. Xem danh sach\n3. Sua diem\n4. Sap xep\n5. Xem diem (MSSV)\n6. Luu & Thoat\n7. CHOT VINH VIEN\nChon: ");
        scanf(" %c", &choice); 
        switch (choice) {
            case '1': nhap(tenFile); break;
            case '2': xemDanhSach(); break; 
            case '3': sua(); break;
            case '4': 
                for (i = 0; i < n - 1; i++) 
                    for (j = i + 1; j < n; j++) 
                        if (ds[i].dtb < ds[j].dtb) { SinhVien t = ds[i]; ds[i] = ds[j]; ds[j] = t; }
                xemDanhSach(); break;
            case '5': xemDiemChiTiet(); break;
            case '6': ghiFile(tenFile); break;
            case '7': 
                if (!isLocked) {
                    printf("\nXac nhan CHOT? (y/n): ");
                    if (getch() == 'y') { isLocked = 1; ghiFile(tenFile); printf("\nDA CHOT!\n"); }
                } else printf("\nDa chot roi!\n");
                break;
        }
    } while (choice != '7');
    return 0;
}
