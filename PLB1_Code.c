#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

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

void tinhDiem(SinhVien *sv) {
    sv->dtb = (sv->lab1 + sv->lab2) * 0.1 
            + (sv->pt1 + sv->pt2) * 0.1 
            + sv->presentation * 0.2 
            + sv->finalTest * 0.4;

    if (sv->dtb >= 8.5) strcpy(sv->diemChu, "A");
    else if (sv->dtb >= 7.0) strcpy(sv->diemChu, "B");
    else if (sv->dtb >= 5.5) strcpy(sv->diemChu, "C");
    else if (sv->dtb >= 4.0) strcpy(sv->diemChu, "D");
    else strcpy(sv->diemChu, "F");
}

void nhap() {
    int n1;
    printf("Nhap so sinh vien muon them: ");
    scanf("%d", &n1);
    int i; 
    for (i = 0; i < n1; i++) {
        if (n >= MAX) break;
        char tamMSSV[20];
        int trung;
        do {
            trung = 0;
            printf("\nNhap MSSV moi: ");
            scanf("%s", tamMSSV);
            int j; 
            for (j = 0; j < n; j++) {
                if (strcmp(ds[j].maSV, tamMSSV) == 0) {
                    printf("Loi: MSSV %s da ton tai!\n", tamMSSV);
                    trung = 1; break;
                }
            }
        } while (trung == 1);

        strcpy(ds[n].maSV, tamMSSV);
        getchar();
        printf("Ten: ");
        fgets(ds[n].tenSV, sizeof(ds[n].tenSV), stdin);
        ds[n].tenSV[strcspn(ds[n].tenSV, "\n")] = 0;
        printf("Diem (Lab1 Lab2 PT1 PT2 Pre Final): ");
        scanf("%f %f %f %f %f %f", &ds[n].lab1, &ds[n].lab2, &ds[n].pt1, &ds[n].pt2, &ds[n].presentation, &ds[n].finalTest);
        tinhDiem(&ds[n]);
        n++;
    }
}

void ghiFile(char tenFile[]) {
    FILE *fp = fopen(tenFile, "w"); 
    int i; 
    for (i = 0; i < n; i++) {
        fprintf(fp, "%s %s %f %f %f %f %f %f\n", 
                ds[i].maSV, ds[i].tenSV, ds[i].lab1, ds[i].lab2, ds[i].pt1, ds[i].pt2, ds[i].presentation, ds[i].finalTest);
    }
    fclose(fp);
}

void docFile(char tenFile[]) {
    FILE *fp = fopen(tenFile, "r");
    if (!fp) { n = 0; return; }
    n = 0;
    while (n < MAX && fscanf(fp, "%s", ds[n].maSV) != EOF) {
        if (fscanf(fp, " %[^0-9]", ds[n].tenSV) != 1) break;
        int len = strlen(ds[n].tenSV);
        while(len > 0 && (ds[n].tenSV[len-1] == ' ' || ds[n].tenSV[len-1] == '\t')) ds[n].tenSV[--len] = '\0';
        if (fscanf(fp, "%f %f %f %f %f %f", &ds[n].lab1, &ds[n].lab2, &ds[n].pt1, &ds[n].pt2, &ds[n].presentation, &ds[n].finalTest) == 6) {
            tinhDiem(&ds[n]);
            n++;
        }
    }
    fclose(fp);
}

void sua() {
    if (isLocked == 1) {
        printf("\n[THONG BAO] Du lieu da duoc Giao vien CHOT. Khong the sua!\n");
        return;
    }
    char mssv[20];
    printf("Nhap MSSV can sua: ");
    scanf("%s", mssv);
    int i; 
    for (i = 0; i < n; i++) {
        if (strcmp(ds[i].maSV, mssv) == 0) {
            printf("Sua diem cho %s: ", ds[i].tenSV);
            scanf("%f %f %f %f %f %f", &ds[i].lab1, &ds[i].lab2, &ds[i].pt1, &ds[i].pt2, &ds[i].presentation, &ds[i].finalTest);
            tinhDiem(&ds[i]);
            printf("Da cap nhat tam thoi!\n");
            return;
        }
    }
    printf("Khong tim thay!\n");
}

int main() {
    char tenFile[50], choice;
    printf("Nhap ten file: "); scanf("%s", tenFile);
    docFile(tenFile);

    do {
        printf("\n===== MENU (%s) [%s] =====\n", tenFile, isLocked ? "DA CHOT" : "DANG MO");
        printf("1. Nhap/Them diem\n2. Xem diem\n3. Sua diem\n4. Sap xep\n5. In danh sach\n6. CHOT & LUU FILE\n7. Thoat\nChon: ");
        scanf(" %c", &choice); 

        switch (choice) {
            case '1': 
                nhap(); 
                /
                printf("\nBan co muon LUU vao file ngay bay gio khong? (y/n): ");
                if (getch() == 'y') {
                    ghiFile(tenFile);
                    isLocked = 1; 
                    printf("\nDa luu va CHOT du lieu thanh cong!\n");
                } else {
                    isLocked = 0; 
                    printf("\nDu lieu moi chi nam tren RAM (chua luu vao file).\n");
                }
                break;
            case '2':  break; 
            case '3': sua(); break;
            case '6': 
                printf("\nXac nhan CHOT va LUU? (y/n): ");
                if (getch() == 'y') { ghiFile(tenFile); isLocked = 1; printf("\nDa luu!\n"); }
                break;
            case '7': printf("Thoat!\n"); break;
        }
    } while (choice != '7');
    return 0;
}
