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
    printf("Nhap so sinh vien: ");
    scanf("%d", &n1);
    int So_luong_moi = n + n1;
    if (So_luong_moi > MAX) {
        printf("So luong sinh vien qua lon (>%d),vui long nhap lai!\n", MAX);
        return;
    }
    int i; 
    for (i = n; i < So_luong_moi; i++) {
        printf("\nSinh vien %d:\n", i + 1);
        printf("MSSV: ");
        scanf("%s", ds[i].maSV);
        getchar();
        printf("Ten: ");
        fgets(ds[i].tenSV, sizeof(ds[i].tenSV), stdin);
        ds[i].tenSV[strcspn(ds[i].tenSV, "\n")] = 0;
        printf("Lab1: "); scanf("%f", &ds[i].lab1);
        printf("Lab2: "); scanf("%f", &ds[i].lab2);
        printf("PT1: "); scanf("%f", &ds[i].pt1);
        printf("PT2: "); scanf("%f", &ds[i].pt2);
        printf("Presentation: "); scanf("%f", &ds[i].presentation);
        printf("Final: "); scanf("%f", &ds[i].finalTest);
        tinhDiem(&ds[i]);
    }
    n += n1;
}
void ghiFile(char tenFile[]) {
    FILE *fp = fopen(tenFile, "w"); 
    int i;
    for (i = 0; i < n; i++) {
        fprintf(fp, "%s %s %f %f %f %f %f %f\n", 
                ds[i].maSV, ds[i].tenSV, 
                ds[i].lab1, ds[i].lab2, ds[i].pt1, ds[i].pt2, 
                ds[i].presentation, ds[i].finalTest);
    }
    fclose(fp);
}
void docFile(char tenFile[]) {
    FILE *fp = fopen(tenFile, "r");
    if (!fp) {
        n = 0;
        printf("Chua co du lieu cu.\n");
        return;
    }
    n = 0;
    while (n < MAX && fscanf(fp, "%s", ds[n].maSV) != EOF) {
        if (fscanf(fp, " %[^0-9]", ds[n].tenSV) != 1) break;
        int len = strlen(ds[n].tenSV);
        while(len > 0 && (ds[n].tenSV[len-1] == ' ' || ds[n].tenSV[len-1] == '\t')) {
            ds[n].tenSV[len-1] = '\0';
            len--;
        }
        if (fscanf(fp, "%f %f %f %f %f %f", 
                   &ds[n].lab1, &ds[n].lab2, &ds[n].pt1, &ds[n].pt2, 
                   &ds[n].presentation, &ds[n].finalTest) == 6) {
            
            tinhDiem(&ds[n]);
            n++;
        }
    }
    fclose(fp);

    if (n > 0) printf("\n--- Da nap %d sinh vien! ---\n", n);
    else printf("\nLoi: Khong doc duoc du lieu!\n");
}
void xem() {
    char mssv[20];
    printf("Nhap MSSV: ");
    scanf("%s", mssv);
    int i; 
    for (i = 0; i < n; i++) {
        if (strcmp(ds[i].maSV, mssv) == 0) {
            printf("\nTim thay:\n");
            printf("Ten: %s\nDTB: %.2f\nDiem chu: %s\n",
                   ds[i].tenSV, ds[i].dtb, ds[i].diemChu);
            return;
        }
    }
    printf("Khong tim thay!\n");
}
void sua() {
    char c;
    do {
    //chuc nang sua diem
    char mssv[20];
    printf("Nhap MSSV can sua: ");
    scanf("%s", mssv);
    int i; 
    for (i = 0; i < n; i++) {
        if (strcmp(ds[i].maSV, mssv) == 0) {
            printf("Nhap lai diem:\n");
            printf("Lab1: "); scanf("%f", &ds[i].lab1);
            printf("Lab2: "); scanf("%f", &ds[i].lab2);
            printf("PT1: "); scanf("%f", &ds[i].pt1);
            printf("PT2: "); scanf("%f", &ds[i].pt2);
            printf("Presentation: "); scanf("%f", &ds[i].presentation);
            printf("Final: "); scanf("%f", &ds[i].finalTest);
            tinhDiem(&ds[i]);
            printf("Da cap nhat!\n");
            break;
        }
    }
    if (i==n) printf("Khong tim thay!\n");
    printf("An c de tiep tuc, an nut khac de thoat\n");
    c = getch();
    } while (c == 'c');
    return;
}
void sapXep() {
	int i; 
    for (i = 0; i < n - 1; i++) {
    	int j; 
        for (j = i + 1; j < n; j++) {
            if (ds[i].dtb < ds[j].dtb) {
                SinhVien temp = ds[i];
                ds[i] = ds[j];
                ds[j] = temp;
            }
        }
    }
}
void inDanhSach() {
    printf("\n%-10s %-20s %-5s %-5s\n",
           "MSSV", "Ten", "DTB", "Chu");
    int i; 
    for (i = 0; i < n; i++) {
        printf("%-10s %-20s %-5.2f %-5s\n",
               ds[i].maSV,
               ds[i].tenSV,
               ds[i].dtb,
               ds[i].diemChu);
    }
}
int main() {
    char tenFile[50];
    char choice;
    printf("Nhap ten file mon hoc (vd: toan.dat): ");
    scanf("%s", tenFile);
    docFile(tenFile);
    //Debug mode
    //Chuc nang
    do {
        printf("\n===== MENU =====\n");
        printf("1. Nhap diem\n");
        printf("2. Xem diem\n");
        printf("3. Sua diem\n");
        printf("4. Sap xep\n");
        printf("5. In danh sach\n");
        printf("6. Thoat\n");
        printf("Chon: ");
        choice = getch();
        switch (choice) {
            case '1':
                nhap();
                ghiFile(tenFile);
                break;
            case '2':
                xem();
                break;
            case '3':
                sua();
                ghiFile(tenFile);
                break;
            case '4':
                sapXep();
                inDanhSach();
                break;
            case '5':
                inDanhSach();
                break;
            case '6':
                printf("Thoat!\n");
                break;
            default:
                printf("Sai!\n");
        }

    } while (choice != '6');

    return 0;
}
