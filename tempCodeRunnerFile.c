
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