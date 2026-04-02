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

// --- 1. TINH DIEM VA XEP LOAI ---
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

// --- 2. GHI FILE ---
void ghiFile(char tenFile[]) {
    FILE *fp = fopen(tenFile, "w"); 
    int i; // Khai bao bien chay ra ngoai de tranh loi C99
    if (!fp) return;
    for (i = 0; i < n; i++) {
        fprintf(fp, "%s %s %f %f %f %f %f %f\n", 
                ds[i].maSV, ds[i].tenSV, ds[i].lab1, ds[i].lab2, ds[i].pt1, ds[i].pt2, ds[i].presentation, ds[i].finalTest);
    }
    fclose(fp);
}

// --- 3. NHAP/THEM SINH VIEN (DA SUA DE XOA KHOI RAM KHI CHON 'N') ---
void nhap(char tenFileChinh[]) {
    int i, j; // Khai bao bien chay
    int n_cu, n1, trung;
    char tamMSSV[20];
    char c;

    if (isLocked) {
        printf("\n[!] Du lieu da CHOT. Khong the nhap them!\n");
        return;
    }

    n_cu = n; 
    printf("\nNhap so sinh vien muon them: ");
    scanf("%d", &n1);
    
    for (i = 0; i < n1; i++) {
        if (n >= MAX) break;
        do {
            trung = 0;
            printf("\nNhap MSSV moi cho sinh vien thu %d: ", i + 1);
            scanf("%s", tamMSSV);
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

    printf("\nBan co muon LUU du lieu vua nhap vao file khong? (y/n): ");
    c = getch();
    if (c == 'y' || c == 'Y') {
        ghiFile("ly_backup.dat");
        ghiFile(tenFileChinh);
        printf("\n[OK] Da luu vao file %s va ly_backup.dat!\n", tenFileChinh);
    } else {
        n = n_cu; 
        printf("\n[HUY] Da huy bo du lieu. Danh sach se giu nguyen.\n");
    }
}

// --- 4. XEM DANH SACH ---
void xemDiem() {
    int i;
    if (n == 0) {
        printf("\n[!] Danh sach trong!\n");
        return;
    }
    printf("\n%-12s %-22s %-5s %-5s %-5s %-5s %-5s %-5s %-6s %-5s\n", 
           "MSSV", "Ho Ten", "L1", "L2", "P1", "P2", "Pre", "End", "DTB", "Grade");
    printf("------------------------------------------------------------------------------------\n");
    for (i = 0; i < n; i++) {
        printf("%-12s %-22s %-5.1f %-5.1f %-5.1f %-5.1f %-5.1f %-5.1f %-6.1f %-5s\n",
               ds[i].maSV, ds[i].tenSV, ds[i].lab1, ds[i].lab2, ds[i].pt1, ds[i].pt2, 
               ds[i].presentation, ds[i].finalTest, ds[i].dtb, ds[i].diemChu);
    }
}
// --- 5. DOC FILE ---
void docFile(char tenFile[]) {
    FILE *fp = fopen(tenFile, "r");
    int len;
    if (!fp) { n = 0; return; }
    n = 0;
    while (n < MAX && fscanf(fp, "%s", ds[n].maSV) != EOF) {
        if (fscanf(fp, " %[^0-9]", ds[n].tenSV) != 1) break;
        len = strlen(ds[n].tenSV);
        while(len > 0 && (ds[n].tenSV[len-1] == ' ' || ds[n].tenSV[len-1] == '\t')) ds[n].tenSV[--len] = '\0';
        if (fscanf(fp, "%f %f %f %f %f %f", &ds[n].lab1, &ds[n].lab2, &ds[n].pt1, &ds[n].pt2, &ds[n].presentation, &ds[n].finalTest) == 6) {
            tinhDiem(&ds[n]);
            n++;
        }
    }
    fclose(fp);
}
// --- 6. SUA DIEM ---
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
            printf("Sua diem cho %s: ", ds[i].tenSV);
            while(1){
            	printf("1.sua diem lab1,lab2\n");
            	printf("2.sua diem pt1,pt2\n");
            	printf("3.sua diem presentation\n");
            	printf("4.sua diem finalTest\n");
            	printf("5.sua toan bo diem\n");
            	printf("6.hoan tat sua diem\n");
            	int lc; scanf("%d",&lc);
            	if(lc==1){
            		printf("nhap lai diem lab1,lab2 :");
            		scanf("%f%f",&ds[i].lab1,&ds[i].lab2);
				}
				else if(lc==2){
					printf("nhap lai diem pt1,pt2 :");
					scanf("%f%f",&ds[i].pt1,&ds[i].pt2);
				}
				else if(lc==3){
					printf("nhap lai diem presentation :");
					scanf("%f",&ds[i].presentation);
				}
				else if(lc==4){
					printf("nhap lai diem final test :");
					scanf("%f",&ds[i].finalTest);
				}
				else if(lc==5){
					printf("nhap lai toan bo diem :");
					scanf("%f %f %f %f %f %f", &ds[i].lab1, &ds[i].lab2, &ds[i].pt1, &ds[i].pt2, &ds[i].presentation, &ds[i].finalTest);
				}
				else {
					break;
				}	
			}
            //scanf("%f %f %f %f %f %f", &ds[i].lab1, &ds[i].lab2, &ds[i].pt1, &ds[i].pt2, &ds[i].presentation, &ds[i].finalTest);
            tinhDiem(&ds[i]);
            printf("Da cap nhat tam thoi!\n");
            return;
        }
    }
    printf("Khong tim thay!\n");
}
int main() {
    char tenFile[50], choice;
    int i, j; // Bien chay cho sap xep
    SinhVien tam;
    printf("Nhap ten file (vidu: ly.dat): "); 
    scanf("%s", tenFile);
    docFile(tenFile);
    do {
        printf("\n===== MENU (%s) [%s] =====\n", tenFile, isLocked ? "DA CHOT" : "DANG MO");
        printf("1. Nhap/Them diem\n2. Xem danh sach diem\n3. Sua diem\n4. Sap xep theo DTB\n5. Luu file & Thoat\n6. CHOT & LUU FILE\nChon: ");
        scanf(" %c", &choice); 
        switch (choice) {
            case '1': nhap(tenFile); break;
            case '2': xemDiem(); break; 
            case '3': sua(); break;
            case '4': 
                for (i = 0; i < n - 1; i++) {
                    for (j = i + 1; j < n; j++) {
                        if (ds[i].dtb < ds[j].dtb) {
                            tam = ds[i]; ds[i] = ds[j]; ds[j] = tam;
                        }
                    }
                }
                xemDiem();
                break;
            case '5':
                ghiFile(tenFile);
                printf("Da luu. Tam biet!\n");
                break;
            case '6': 
                printf("\nGiao vien xac nhan (khong the sua diem)? (y/n): ");
                if (getch() == 'y') { ghiFile(tenFile); isLocked = 1; printf("\nDa xac nhan!\n"); }
                break;
        }
    } while (choice != '5');
    return 0;
}
