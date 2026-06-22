#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>

// Nhúng các thư viện giao diện đồ họa Dear ImGui & GLFW
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> 
#ifdef _WIN32
#include <windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#define MAX 100
#define MAX_MON 100
#define MAX_HP 20

// ============================================================================
// PART 1: GIỮ NGUYÊN TOÀN BỘ STRUCT VÀ BIẾN GLOBAL TỪ FILE C CŨ
// ============================================================================
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
const char *tenCotGlobal[] = {"Lab1", "Lab2", "PT1", "PT2", "Presentation", "Final"};
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

// ============================================================================
// PART 2: CÁC HÀM XỬ LÝ FILE & LOGIC ĐIỂM (KHÔNG ĐỔI)
// ============================================================================
void docFilemonhoc() {
    FILE *fp = fopen("monhoc.txt", "r");
    if (!fp) {
        fp = fopen("monhoc.txt", "w");
        if (fp) fclose(fp);
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

bool daCoDiem(SinhVien sv) {
    return sv.lab1 >= 0 && sv.lab2 >= 0 &&
           sv.pt1 >= 0 && sv.pt2 >= 0 &&
           sv.presentation >= 0 && sv.finalTest >= 0;
}

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
                ds[i].maSV, ds[i].lop, ds[i].tenSV, 
                ds[i].lab1, ds[i].lab2, ds[i].pt1, ds[i].pt2, 
                ds[i].presentation, ds[i].finalTest);
    }
    fclose(fp);
}
const char* layTenCuoi(const char *hoTen) {
    const char *p = strrchr(hoTen, ' ');
    if (p) return p + 1;
    return hoTen;
}

bool svCoTrongFile(char tenFile[], char mssv[]) {
    FILE *fp = fopen(tenFile, "r");
    if (!fp) return false;
    char line[256];
    if (fgets(line, sizeof(line), fp)) {
        // Bo qua dong dau (cau hinh)
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

int tatCaDaChot() {
    int ci;
    for (ci = 0; ci < 6; ci++) if (!colLocked[ci]) return 0;
    return 1;
}

bool docFile(char tenFile[]) {
    strcpy(fileHPHienTai, tenFile);
    FILE *fp = fopen(tenFile, "r");
    if (!fp) return false;

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
                           ds[n].maSV, ds[n].lop, ds[n].tenSV, 
                           &ds[n].lab1, &ds[n].lab2, &ds[n].pt1, &ds[n].pt2, 
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

// ============================================================================
// PART 3: DUY NHẤT MỘT HÀM MAIN ĐIỀU KHIỂN ĐỒ HỌA IMGUI (C++)
// ============================================================================
int main() {
    // 1. Khởi tạo dữ liệu từ file
    docFilemonhoc();
    docFileSinhVien();

    // 2. Cấu hình hệ thống cửa sổ đồ họa GLFW & OpenGL
    if (!glfwInit()) return 1;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "PBL1: He thong Quan ly Diem Sinh vien", NULL, NULL);
    if (window == NULL) return 1;
    glfwMakeContextCurrent(window);

#ifdef _WIN32
    HWND hwnd = glfwGetWin32Window(window);
    HICON hIcon = LoadIcon(GetModuleHandle(NULL), "IDI_ICON1");
    if (hIcon) {
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    }
#endif

    glfwSwapInterval(1); 

    // 3. Thiết lập môi trường ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark(); 

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Các biến trạng thái hiển thị UI
    int mon_selected_idx = -1;
    int hp_selected_idx = -1;
    char search_mssv[20] = "";
    bool show_student_detail = false;
    int detail_sv_idx = -1;

    // Biến tạm form nhập liệu
    char add_mssv[20] = "";
    char add_lop[20] = "";
    char add_ten[50] = "";

    // Thêm sinh viên vào học phần
    char add_hp_mssv[20] = "";
    bool show_add_hp_error = false;
    char add_hp_error_msg[128] = "";

    // Thông báo khi kick sinh viên
    bool show_kick_error = false;
    char kick_error_msg[128] = "";

    // Xóa sinh viên khỏi hệ thống
    bool show_delete_system_modal = false;
    char delete_system_mssv[20] = "";
    bool show_delete_system_error = false;
    char delete_system_error_msg[128] = "";

    // Sắp xếp
    int sort_criteria = 0; // 0: MSSV, 1: Tên, 2: Lớp, 3: ĐTB
    int sort_direction = 0; // 0: Tăng dần, 1: Giảm dần

    // Thêm môn học mới
    bool show_add_mon_modal = false;
    char add_mon_ten[30] = "";
    int add_mon_so_hp = 1;
    int add_mon_max_sv = 40;
    float add_mon_w_lab = 0.1f;
    float add_mon_w_pt = 0.1f;
    float add_mon_w_pre = 0.2f;
    float add_mon_w_final = 0.4f;
    char add_mon_ma_hp[MAX_HP][20];
    for (int i = 0; i < MAX_HP; i++) add_mon_ma_hp[i][0] = '\0';
    bool show_add_mon_error = false;
    char add_mon_error_msg[128] = "";

    // 4. VÒNG LẶP RENDER CHÍNH
    int display_w = 1280, display_h = 720;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glfwGetFramebufferSize(window, &display_w, &display_h);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --------------------------------------------------------------------
        // CỬA SỔ TRÁI: THANH ĐIỀU HƯỚNG
        // --------------------------------------------------------------------
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(350, display_h - 20), ImGuiCond_Always);
        ImGui::Begin("Thanh Dieu Huong", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "PBL1: QUAN LY DIEM SINH VIEN");
        ImGui::Separator();

        ImGui::Text("1. Chon Mon Hoc:");
        if (ImGui::BeginListBox("##MonHocList", ImVec2(-FLT_MIN, 150))) {
            for (int i = 0; i < soMon; i++) {
                const bool is_selected = (mon_selected_idx == i);
                if (ImGui::Selectable(dsMonHoc[i].tenMon, is_selected)) {
                    mon_selected_idx = i;
                    hp_selected_idx = -1; 
                }
            }
            ImGui::EndListBox();
        }
        if (ImGui::Button("Them Mon Hoc Moi", ImVec2(-FLT_MIN, 0))) {
            show_add_mon_modal = true;
        }

        if (mon_selected_idx != -1) {
            ImGui::Text("2. Chon Hoc Phan:");
            MonHoc& currentMon = dsMonHoc[mon_selected_idx];
            if (ImGui::BeginListBox("##HocPhanList", ImVec2(-FLT_MIN, 150))) {
                for (int i = 0; i < currentMon.soHocPhan; i++) {
                    const bool is_selected = (hp_selected_idx == i);
                    if (ImGui::Selectable(currentMon.dsHocPhan[i].maHP, is_selected)) {
                        hp_selected_idx = i;
                        docFile(currentMon.dsHocPhan[i].fileHP);
                    }
                }
                ImGui::EndListBox();
            }
        }

        if (hp_selected_idx != -1) {
            ImGui::Separator();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "THEM SINH VIEN VAO HOC PHAN");
            ImGui::InputText("MSSV Them", add_hp_mssv, IM_ARRAYSIZE(add_hp_mssv));
            if (ImGui::Button("Them vao Lop Hoc Phan", ImVec2(-FLT_MIN, 0))) {
                if (strlen(add_hp_mssv) > 0) {
                    int idx = timSVTheoMSSV(add_hp_mssv);
                    if (idx < 0) {
                        show_add_hp_error = true;
                        strcpy(add_hp_error_msg, "MSSV khong ton tai trong danh sach tong!");
                    } else {
                        // Kiem tra da co trong lop hien tai chua
                        bool found = false;
                        for (int i = 0; i < n; i++) {
                            if (strcmp(ds[i].maSV, add_hp_mssv) == 0) {
                                found = true;
                                break;
                            }
                        }
                        if (found) {
                            show_add_hp_error = true;
                            strcpy(add_hp_error_msg, "Sinh vien da co trong lop hoc phan nay!");
                        } else {
                            // Kiem tra xem co dang hoc phan khac cua cung mon khong
                            bool registeredInOther = false;
                            MonHoc& currentMon = dsMonHoc[mon_selected_idx];
                            for (int checkHP = 0; checkHP < currentMon.soHocPhan; checkHP++) {
                                if (checkHP == hp_selected_idx) continue;
                                if (svCoTrongFile(currentMon.dsHocPhan[checkHP].fileHP, add_hp_mssv)) {
                                    show_add_hp_error = true;
                                    sprintf(add_hp_error_msg, "Vi pham! Da hoc lop %s cua mon nay!", currentMon.dsHocPhan[checkHP].maHP);
                                    registeredInOther = true;
                                    break;
                                }
                            }
                            if (!registeredInOther) {
                                if (n >= soSVToiDa) {
                                    show_add_hp_error = true;
                                    sprintf(add_hp_error_msg, "Lop hoc phan da day (%d/%d)!", n, soSVToiDa);
                                } else {
                                    // Them vao
                                    strcpy(ds[n].maSV, add_hp_mssv);
                                    strcpy(ds[n].lop, dsSV[idx].lop);
                                    strcpy(ds[n].tenSV, dsSV[idx].tenSV);
                                    ds[n].lab1 = -1.0f;
                                    ds[n].lab2 = -1.0f;
                                    ds[n].pt1 = -1.0f;
                                    ds[n].pt2 = -1.0f;
                                    ds[n].presentation = -1.0f;
                                    ds[n].finalTest = -1.0f;
                                    ds[n].dtb = -1.0f;
                                    strcpy(ds[n].diemChu, " ");
                                    n++;
                                    ghiFile(currentMon.dsHocPhan[hp_selected_idx].fileHP);
                                    add_hp_mssv[0] = '\0';
                                }
                            }
                        }
                    }
                }
            }
        }

        if (show_add_hp_error) {
            ImGui::OpenPopup("LoiThemHP");
            show_add_hp_error = false;
        }
        if (ImGui::BeginPopupModal("LoiThemHP", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "LOI THEM VAO HOC PHAN:");
            ImGui::Text("%s", add_hp_error_msg);
            if (ImGui::Button("Dong", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

        ImGui::Separator();
        
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "THEM SINH VIEN MOI (HE THONG)");
        ImGui::InputText("MSSV", add_mssv, IM_ARRAYSIZE(add_mssv));
        ImGui::InputText("Lop", add_lop, IM_ARRAYSIZE(add_lop));
        ImGui::InputText("Ho Ten", add_ten, IM_ARRAYSIZE(add_ten));
        
        if (ImGui::Button("Them vao danh sach tong", ImVec2(-FLT_MIN, 0))) {
            if (strlen(add_mssv) > 0 && strlen(add_ten) > 0) {
                if (timSVTheoMSSV(add_mssv) >= 0) {
                    ImGui::OpenPopup("LoiTrungMSSV");
                } else {
                    strcpy(dsSV[nSV].maSV, add_mssv);
                    strcpy(dsSV[nSV].lop, add_lop);
                    strcpy(dsSV[nSV].tenSV, add_ten);
                    nSV++;
                    ghiFileSinhVien(); 
                    add_mssv[0] = '\0'; add_lop[0] = '\0'; add_ten[0] = '\0';
                }
            }
        }
        
        if (ImGui::Button("Xoa khoi danh sach tong (He thong)", ImVec2(-FLT_MIN, 0))) {
            show_delete_system_modal = true;
        }

        if (ImGui::BeginPopupModal("LoiTrungMSSV", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Ma so sinh vien nay da ton tai trong he thong!");
            if (ImGui::Button("Dong", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

        if (show_delete_system_modal) {
            ImGui::OpenPopup("XoaSVHeThong");
            show_delete_system_modal = false;
        }
        if (ImGui::BeginPopupModal("XoaSVHeThong", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Nhap MSSV can xoa khoi he thong:");
            ImGui::InputText("MSSV Xoa", delete_system_mssv, IM_ARRAYSIZE(delete_system_mssv));
            
            if (ImGui::Button("Xac nhan Xoa", ImVec2(120, 0))) {
                if (strlen(delete_system_mssv) > 0) {
                    int idx = timSVTheoMSSV(delete_system_mssv);
                    if (idx < 0) {
                        show_delete_system_error = true;
                        strcpy(delete_system_error_msg, "MSSV khong ton tai!");
                    } else {
                        // Xóa khỏi danh sách sinh viên tổng
                        for (int i = idx; i < nSV - 1; i++) {
                            dsSV[i] = dsSV[i + 1];
                        }
                        nSV--;
                        ghiFileSinhVien();

                        // Quét tất cả các file học phần của tất cả các môn học
                        for (int f = 0; f < soMon; f++) {
                            for (int c = 0; c < dsMonHoc[f].soHocPhan; c++) {
                                char fileHP[50];
                                strcpy(fileHP, dsMonHoc[f].dsHocPhan[c].fileHP);
                                
                                FILE *fpTemp = fopen(fileHP, "r");
                                if (fpTemp) {
                                    float temp_wLab, temp_wPT, temp_wPre, temp_wFinal;
                                    int temp_colLocked[6];
                                    int temp_soSVToiDa;
                                    SinhVien temp_ds[MAX];
                                    int temp_n = 0;

                                    char line[256];
                                    if (fgets(line, sizeof(line), fpTemp)) {
                                        int tc = 0;
                                        int parsed = sscanf(line, "%f %f %f %f %d %d %d %d %d %d %d", 
                                                        &temp_wLab, &temp_wPT, &temp_wPre, &temp_wFinal, 
                                                        &temp_colLocked[0], &temp_colLocked[1], &temp_colLocked[2], &temp_colLocked[3], &temp_colLocked[4], &temp_colLocked[5],
                                                        &tc);
                                        temp_soSVToiDa = (parsed >= 11) ? tc : 0;
                                    }

                                    while (fgets(line, sizeof(line), fpTemp) && temp_n < MAX) {
                                        if (strlen(line) < 5) continue;
                                        int check = sscanf(line, "%19s %19s %[^0-9.-] %f %f %f %f %f %f", 
                                                           temp_ds[temp_n].maSV, temp_ds[temp_n].lop, temp_ds[temp_n].tenSV, 
                                                           &temp_ds[temp_n].lab1, &temp_ds[temp_n].lab2, &temp_ds[temp_n].pt1, &temp_ds[temp_n].pt2, 
                                                           &temp_ds[temp_n].presentation, &temp_ds[temp_n].finalTest);

                                        if (check >= 2) { 
                                            int len = strlen(temp_ds[temp_n].tenSV);
                                            while(len > 0 && (temp_ds[temp_n].tenSV[len-1] == ' ' || temp_ds[temp_n].tenSV[len-1] == '\t' || 
                                                              temp_ds[temp_n].tenSV[len-1] == '\r' || temp_ds[temp_n].tenSV[len-1] == '\n')) {
                                                temp_ds[temp_n].tenSV[--len] = '\0';
                                            }
                                            temp_n++;
                                        }
                                    }
                                    fclose(fpTemp);

                                    // Xóa sinh viên nếu có trong lớp này
                                    int sectionIdx = -1;
                                    for (int i = 0; i < temp_n; i++) {
                                        if (strcmp(temp_ds[i].maSV, delete_system_mssv) == 0) {
                                            sectionIdx = i;
                                            break;
                                        }
                                    }
                                    if (sectionIdx >= 0) {
                                        for (int i = sectionIdx; i < temp_n - 1; i++) {
                                            temp_ds[i] = temp_ds[i + 1];
                                        }
                                        temp_n--;
                                        
                                        // Ghi đè lại file HP
                                        FILE *fpWrite = fopen(fileHP, "w");
                                        if (fpWrite) {
                                            fprintf(fpWrite, "%f %f %f %f %d %d %d %d %d %d %d\n", 
                                                        temp_wLab, temp_wPT, temp_wPre, temp_wFinal,
                                                        temp_colLocked[0], temp_colLocked[1], temp_colLocked[2], temp_colLocked[3], temp_colLocked[4], temp_colLocked[5],
                                                        temp_soSVToiDa);
                                            for (int i = 0; i < temp_n; i++) {
                                                fprintf(fpWrite, "%s %s %s %f %f %f %f %f %f\n", 
                                                        temp_ds[i].maSV, temp_ds[i].lop, temp_ds[i].tenSV, 
                                                        temp_ds[i].lab1, temp_ds[i].lab2, temp_ds[i].pt1, temp_ds[i].pt2, 
                                                        temp_ds[i].presentation, temp_ds[i].finalTest);
                                            }
                                            fclose(fpWrite);
                                        }
                                    }
                                }
                            }
                        }

                        // Load lại dữ liệu nếu lớp đang được chọn chứa sinh viên vừa xóa
                        if (hp_selected_idx != -1) {
                            docFile(fileHPHienTai);
                        }

                        delete_system_mssv[0] = '\0';
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Huy", ImVec2(120, 0))) {
                delete_system_mssv[0] = '\0';
                ImGui::CloseCurrentPopup();
            }

            if (show_delete_system_error) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", delete_system_error_msg);
                show_delete_system_error = false;
            }

            ImGui::EndPopup();
        }

        ImGui::End();

        // --------------------------------------------------------------------
        // CỬA SỔ PHẢI: BẢNG LƯỚI ĐIỂM CHI TIẾT
        // --------------------------------------------------------------------
        ImGui::SetNextWindowPos(ImVec2(370, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(display_w - 380, display_h - 20), ImGuiCond_Always);
        ImGui::Begin("Bang Diem Chi Tiet", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        if (hp_selected_idx == -1) {
            ImGui::Text("Vui long chon Mon hoc va Hoc phan ben trai de xem thong tin bang diem.");
        } else {
            ImGui::Text("Hoc phan hien tai: %s | File: %s", fileHPHienTai, fileHPHienTai);
            ImGui::Text("Trong so: Lab=%.2f, PT=%.2f, Pre=%.2f, Final=%.2f | Si so: %d/%d", 
                         wLab, wPT, wPre, wFinal, n, soSVToiDa);
            ImGui::Separator();

            ImGui::InputText("Tim kiem MSSV", search_mssv, IM_ARRAYSIZE(search_mssv));
            ImGui::SameLine();
            if (ImGui::Button("Xem chi tiet") && strlen(search_mssv) > 0) {
                for(int i=0; i<n; i++) {
                    if(strcmp(ds[i].maSV, search_mssv) == 0) {
                        detail_sv_idx = i;
                        show_student_detail = true;
                        break;
                    }
                }
            }
            
            ImGui::Spacing();

            ImGui::Text("Sap xep:");
            ImGui::SameLine();
            ImGui::PushItemWidth(150);
            const char* criteria_names[] = { "MSSV", "Ten sinh vien", "Lop", "Diem trung binh" };
            ImGui::Combo("##SortCriteria", &sort_criteria, criteria_names, IM_ARRAYSIZE(criteria_names));
            ImGui::PopItemWidth();
            ImGui::SameLine();
            ImGui::PushItemWidth(120);
            const char* direction_names[] = { "Tang dan", "Giam dan" };
            ImGui::Combo("##SortDirection", &sort_direction, direction_names, IM_ARRAYSIZE(direction_names));
            ImGui::PopItemWidth();
            ImGui::SameLine();
            if (ImGui::Button("Sap xep")) {
                for (int i = 0; i < n - 1; i++) {
                    for (int j = i + 1; j < n; j++) {
                        bool canDoiCho = false;
                        switch (sort_criteria) {
                            case 0:
                                if (sort_direction == 0 && strcmp(ds[i].maSV, ds[j].maSV) > 0) canDoiCho = true;
                                if (sort_direction == 1 && strcmp(ds[i].maSV, ds[j].maSV) < 0) canDoiCho = true;
                                break;
                            case 1: {
                                int cmpTen = strcmp(layTenCuoi(ds[i].tenSV), layTenCuoi(ds[j].tenSV));
                                if (cmpTen == 0) cmpTen = strcmp(ds[i].tenSV, ds[j].tenSV);
                                if (sort_direction == 0 && cmpTen > 0) canDoiCho = true;
                                if (sort_direction == 1 && cmpTen < 0) canDoiCho = true;
                                break;
                            }
                            case 2:
                                if (sort_direction == 0 && strcmp(ds[i].lop, ds[j].lop) > 0) canDoiCho = true;
                                if (sort_direction == 1 && strcmp(ds[i].lop, ds[j].lop) < 0) canDoiCho = true;
                                break;
                            case 3:
                                if (sort_direction == 0 && ds[i].dtb > ds[j].dtb) canDoiCho = true;
                                if (sort_direction == 1 && ds[i].dtb < ds[j].dtb) canDoiCho = true;
                                break;
                        }
                        if (canDoiCho) {
                            SinhVien t = ds[i];
                            ds[i] = ds[j];
                            ds[j] = t;
                        }
                    }
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Luu thu tu sap xep")) {
                ghiFile(fileHPHienTai);
            }

            ImGui::Spacing();

            float table_height = display_h - 320.0f;
            if (table_height < 200.0f) table_height = 200.0f;

            if (ImGui::BeginTable("StudentTable", 12, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY, ImVec2(0, table_height))) {
                ImGui::TableSetupColumn("STT", ImGuiTableColumnFlags_WidthFixed, 30.0f);
                ImGui::TableSetupColumn("MSSV", ImGuiTableColumnFlags_WidthFixed, 90.0f);
                ImGui::TableSetupColumn("Ho va Ten", ImGuiTableColumnFlags_WidthFixed, 180.0f);
                ImGui::TableSetupColumn("Lab 1", ImGuiTableColumnFlags_WidthFixed, 50.0f);
                ImGui::TableSetupColumn("Lab 2", ImGuiTableColumnFlags_WidthFixed, 50.0f);
                ImGui::TableSetupColumn("PT 1", ImGuiTableColumnFlags_WidthFixed, 50.0f);
                ImGui::TableSetupColumn("PT 2", ImGuiTableColumnFlags_WidthFixed, 50.0f);
                ImGui::TableSetupColumn("Pre", ImGuiTableColumnFlags_WidthFixed, 50.0f);
                ImGui::TableSetupColumn("Final", ImGuiTableColumnFlags_WidthFixed, 50.0f);
                ImGui::TableSetupColumn("DTB", ImGuiTableColumnFlags_WidthFixed, 50.0f);
                ImGui::TableSetupColumn("Chu", ImGuiTableColumnFlags_WidthFixed, 40.0f);
                ImGui::TableSetupColumn("Thao tac", ImGuiTableColumnFlags_WidthFixed, 60.0f);
                ImGui::TableHeadersRow();

                for (int i = 0; i < n; i++) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0); ImGui::Text("%d", i + 1);
                    ImGui::TableSetColumnIndex(1); ImGui::Text("%s", ds[i].maSV);
                    ImGui::TableSetColumnIndex(2); ImGui::Text("%s", ds[i].tenSV);

                    // Sửa đổi ID tĩnh thành ID động dạng ID_##i để tránh xung đột dữ liệu ImGui giữa các dòng
                    // Lab 1
                    ImGui::TableSetColumnIndex(3);
                    if (colLocked[0]) {
                        if (ds[i].lab1 < 0) ImGui::Text(" ");
                        else ImGui::Text("%.1f", ds[i].lab1); 
                    } else {
                        ImGui::PushItemWidth(-FLT_MIN);
                        char id[20]; sprintf(id, "##L1_%d", i);
                        char buf[16];
                        if (ds[i].lab1 < 0) strcpy(buf, "");
                        else sprintf(buf, "%.1f", ds[i].lab1);
                        if (ImGui::InputText(id, buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal)) {
                            if (strlen(buf) == 0) ds[i].lab1 = -1.0f;
                            else ds[i].lab1 = atof(buf);
                            tinhDiem(&ds[i]);
                        }
                        ImGui::PopItemWidth();
                    }

                    // Lab 2
                    ImGui::TableSetColumnIndex(4);
                    if (colLocked[1]) {
                        if (ds[i].lab2 < 0) ImGui::Text(" ");
                        else ImGui::Text("%.1f", ds[i].lab2);
                    } else {
                        ImGui::PushItemWidth(-FLT_MIN);
                        char id[20]; sprintf(id, "##L2_%d", i);
                        char buf[16];
                        if (ds[i].lab2 < 0) strcpy(buf, "");
                        else sprintf(buf, "%.1f", ds[i].lab2);
                        if (ImGui::InputText(id, buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal)) {
                            if (strlen(buf) == 0) ds[i].lab2 = -1.0f;
                            else ds[i].lab2 = atof(buf);
                            tinhDiem(&ds[i]);
                        }
                        ImGui::PopItemWidth();
                    }

                    // PT 1
                    ImGui::TableSetColumnIndex(5);
                    if (colLocked[2]) {
                        if (ds[i].pt1 < 0) ImGui::Text(" ");
                        else ImGui::Text("%.1f", ds[i].pt1);
                    } else {
                        ImGui::PushItemWidth(-FLT_MIN);
                        char id[20]; sprintf(id, "##P1_%d", i);
                        char buf[16];
                        if (ds[i].pt1 < 0) strcpy(buf, "");
                        else sprintf(buf, "%.1f", ds[i].pt1);
                        if (ImGui::InputText(id, buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal)) {
                            if (strlen(buf) == 0) ds[i].pt1 = -1.0f;
                            else ds[i].pt1 = atof(buf);
                            tinhDiem(&ds[i]);
                        }
                        ImGui::PopItemWidth();
                    }

                    // PT 2
                    ImGui::TableSetColumnIndex(6);
                    if (colLocked[3]) {
                        if (ds[i].pt2 < 0) ImGui::Text(" ");
                        else ImGui::Text("%.1f", ds[i].pt2);
                    } else {
                        ImGui::PushItemWidth(-FLT_MIN);
                        char id[20]; sprintf(id, "##P2_%d", i);
                        char buf[16];
                        if (ds[i].pt2 < 0) strcpy(buf, "");
                        else sprintf(buf, "%.1f", ds[i].pt2);
                        if (ImGui::InputText(id, buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal)) {
                            if (strlen(buf) == 0) ds[i].pt2 = -1.0f;
                            else ds[i].pt2 = atof(buf);
                            tinhDiem(&ds[i]);
                        }
                        ImGui::PopItemWidth();
                    }

                    // Presentation
                    ImGui::TableSetColumnIndex(7);
                    if (colLocked[4]) {
                        if (ds[i].presentation < 0) ImGui::Text(" ");
                        else ImGui::Text("%.1f", ds[i].presentation);
                    } else {
                        ImGui::PushItemWidth(-FLT_MIN);
                        char id[20]; sprintf(id, "##Pre_%d", i);
                        char buf[16];
                        if (ds[i].presentation < 0) strcpy(buf, "");
                        else sprintf(buf, "%.1f", ds[i].presentation);
                        if (ImGui::InputText(id, buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal)) {
                            if (strlen(buf) == 0) ds[i].presentation = -1.0f;
                            else ds[i].presentation = atof(buf);
                            tinhDiem(&ds[i]);
                        }
                        ImGui::PopItemWidth();
                    }

                    // Final Test
                    ImGui::TableSetColumnIndex(8);
                    if (colLocked[5]) {
                        if (ds[i].finalTest < 0) ImGui::Text(" ");
                        else ImGui::Text("%.1f", ds[i].finalTest);
                    } else {
                        ImGui::PushItemWidth(-FLT_MIN);
                        char id[20]; sprintf(id, "##Fin_%d", i);
                        char buf[16];
                        if (ds[i].finalTest < 0) strcpy(buf, "");
                        else sprintf(buf, "%.1f", ds[i].finalTest);
                        if (ImGui::InputText(id, buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal)) {
                            if (strlen(buf) == 0) ds[i].finalTest = -1.0f;
                            else ds[i].finalTest = atof(buf);
                            tinhDiem(&ds[i]);
                        }
                        ImGui::PopItemWidth();
                    }

                    // DTB
                    ImGui::TableSetColumnIndex(9); 
                    if (!daCoDiem(ds[i])) ImGui::Text(" ");
                    else ImGui::TextColored(ImVec4(0,1,0,1), "%.2f", ds[i].dtb);

                    // Chu
                    ImGui::TableSetColumnIndex(10); ImGui::Text("%s", ds[i].diemChu);

                    ImGui::TableSetColumnIndex(11);
                    char btn_id[32];
                    sprintf(btn_id, "Loai##%d", i);
                    if (ImGui::Button(btn_id)) {
                        if (ds[i].finalTest >= 0) {
                            show_kick_error = true;
                            sprintf(kick_error_msg, "Khong the loai! %s da hoan thanh diem cuoi ky.", ds[i].tenSV);
                        } else {
                            for (int k = i; k < n - 1; k++) {
                                ds[k] = ds[k + 1];
                            }
                            n--;
                            ghiFile(fileHPHienTai);
                        }
                    }
                }
                ImGui::EndTable();
            }

            ImGui::Spacing();

            ImGui::Text("Chot tung cot diem (Yeu cau phai co du diem):");
            for (int ci = 0; ci < 6; ci++) {
                char btn_lbl[32];
                if (colLocked[ci]) {
                    sprintf(btn_lbl, "%s [KHOA]", tenCotGlobal[ci]);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.2f, 0.2f, 1.0f));
                    ImGui::Button(btn_lbl);
                    ImGui::PopStyleColor();
                } else {
                    sprintf(btn_lbl, "%s [MO]", tenCotGlobal[ci]);
                    if (ImGui::Button(btn_lbl)) {
                        if (cotCoDuDiem(ci)) {
                            colLocked[ci] = 1;
                            ghiFile(fileHPHienTai);
                        } else {
                            ImGui::OpenPopup("LoiChotCot");
                        }
                    }
                }
                if (ci < 5) ImGui::SameLine();
            }

            if (ImGui::BeginPopupModal("LoiChotCot", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "LOI CHOT COT DIEM:");
                ImGui::Text("Cot nay con sinh vien chua co diem! Khong the chot.");
                if (ImGui::Button("Dong", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }

            ImGui::Spacing();
            
            if (ImGui::Button("Luu Thay Doi Diem", ImVec2(200, 40))) {
                ghiFile(fileHPHienTai);
            }
            ImGui::SameLine();
            if (ImGui::Button("Chot Toan Bo Cot", ImVec2(200, 40))) {
                for(int ci=0; ci<6; ci++) {
                    if (cotCoDuDiem(ci)) colLocked[ci] = 1;
                }
                ghiFile(fileHPHienTai);
            }
        }
        ImGui::End();

        // --------------------------------------------------------------------
        // CỬA SỔ PHỤ: POPUP PHIẾU ĐIỂM
        // --------------------------------------------------------------------
        if (show_student_detail && detail_sv_idx != -1) {
            ImGui::Begin("Bang Diem Chi Tiet Sinh Vien", &show_student_detail, ImGuiWindowFlags_AlwaysAutoResize);
            SinhVien& sv = ds[detail_sv_idx];
            ImGui::Text("MSSV: %s", sv.maSV);
            ImGui::Text("Lop: %s", sv.lop);
            ImGui::Text("Ho ten: %s", sv.tenSV);
            ImGui::Separator();
            char sLab1[16], sLab2[16], sPT1[16], sPT2[16], sPre[16], sFinal[16], sDTB[16];
            if (sv.lab1 < 0) strcpy(sLab1, "Chua co"); else sprintf(sLab1, "%.1f", sv.lab1);
            if (sv.lab2 < 0) strcpy(sLab2, "Chua co"); else sprintf(sLab2, "%.1f", sv.lab2);
            if (sv.pt1 < 0) strcpy(sPT1, "Chua co"); else sprintf(sPT1, "%.1f", sv.pt1);
            if (sv.pt2 < 0) strcpy(sPT2, "Chua co"); else sprintf(sPT2, "%.1f", sv.pt2);
            if (sv.presentation < 0) strcpy(sPre, "Chua co"); else sprintf(sPre, "%.1f", sv.presentation);
            if (sv.finalTest < 0) strcpy(sFinal, "Chua co"); else sprintf(sFinal, "%.1f", sv.finalTest);
            if (!daCoDiem(sv)) strcpy(sDTB, "Chua co"); else sprintf(sDTB, "%.2f", sv.dtb);

            ImGui::Text("Diem Lab 1: %s | Lab 2: %s", sLab1, sLab2);
            ImGui::Text("Diem PT 1:  %s | PT 2:  %s", sPT1, sPT2);
            ImGui::Text("Thuyet trinh: %s", sPre);
            ImGui::Text("Thi cuoi ky:  %s", sFinal);
            ImGui::Separator();
            ImGui::TextColored(ImVec4(1,0,0,1), "DIEM TRUNG BINH: %s", sDTB);
            ImGui::Text("XEP LOAI: %s", sv.diemChu);
            if (ImGui::Button("Dong")) { show_student_detail = false; }
            ImGui::End();
        }

        if (show_kick_error) {
            ImGui::OpenPopup("LoiKickSV");
            show_kick_error = false;
        }
        if (ImGui::BeginPopupModal("LoiKickSV", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "LOI LOAI SINH VIEN KHOI HOC PHAN:");
            ImGui::Text("%s", kick_error_msg);
            if (ImGui::Button("Dong", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

        if (show_add_mon_modal) {
            ImGui::OpenPopup("ThemMonHocModal");
            show_add_mon_modal = false;
        }
        if (ImGui::BeginPopupModal("ThemMonHocModal", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "THEM MON HOC MOI");
            ImGui::InputText("Ten mon hoc", add_mon_ten, IM_ARRAYSIZE(add_mon_ten));
            ImGui::InputInt("So luong SV toi da", &add_mon_max_sv);
            ImGui::InputInt("So luong hoc phan (1-20)", &add_mon_so_hp);
            if (add_mon_so_hp < 1) add_mon_so_hp = 1;
            if (add_mon_so_hp > MAX_HP) add_mon_so_hp = MAX_HP;

            ImGui::Separator();
            ImGui::Text("Trong so diem (Lab x 2 + PT x 2 + Pre + Final = 1.0):");
            ImGui::InputFloat("Heso Lab", &add_mon_w_lab, 0.01f, 0.05f, "%.2f");
            ImGui::InputFloat("Heso PT", &add_mon_w_pt, 0.01f, 0.05f, "%.2f");
            ImGui::InputFloat("Heso Pre", &add_mon_w_pre, 0.01f, 0.05f, "%.2f");
            ImGui::InputFloat("Heso Final", &add_mon_w_final, 0.01f, 0.05f, "%.2f");

            float tong_ts = add_mon_w_lab * 2 + add_mon_w_pt * 2 + add_mon_w_pre + add_mon_w_final;
            ImGui::Text("Tong trong so hien tai: %.2f", tong_ts);

            ImGui::Separator();
            ImGui::Text("Nhap ma cho tung hoc phan:");
            for (int i = 0; i < add_mon_so_hp; i++) {
                char label[32];
                sprintf(label, "Ma hoc phan %d", i + 1);
                ImGui::InputText(label, add_mon_ma_hp[i], sizeof(add_mon_ma_hp[i]));
            }

            if (ImGui::Button("Xac nhan", ImVec2(120, 0))) {
                if (strlen(add_mon_ten) == 0) {
                    show_add_mon_error = true;
                    strcpy(add_mon_error_msg, "Ten mon hoc khong duoc de trong!");
                } else if (add_mon_max_sv < 1 || add_mon_max_sv > MAX) {
                    show_add_mon_error = true;
                    sprintf(add_mon_error_msg, "Si so toi da phai tu 1 den %d!", MAX);
                } else if (tong_ts < 0.99f || tong_ts > 1.01f) {
                    show_add_mon_error = true;
                    strcpy(add_mon_error_msg, "Tong trong so phai bang 1.0!");
                } else {
                    bool dup_mon = false;
                    docFilemonhoc();
                    for (int i = 0; i < soMon; i++) {
                        if (strcmp(dsMonHoc[i].tenMon, add_mon_ten) == 0) {
                            dup_mon = true;
                            break;
                        }
                    }
                    if (dup_mon) {
                        show_add_mon_error = true;
                        strcpy(add_mon_error_msg, "Mon hoc nay da ton tai!");
                    } else {
                        bool dup_ma = false;
                        for (int i = 0; i < add_mon_so_hp; i++) {
                            if (strlen(add_mon_ma_hp[i]) == 0) {
                                show_add_mon_error = true;
                                strcpy(add_mon_error_msg, "Ma hoc phan khong duoc de trong!");
                                dup_ma = true;
                                break;
                            }
                            for (int j = 0; j < i; j++) {
                                if (strcmp(add_mon_ma_hp[i], add_mon_ma_hp[j]) == 0) {
                                    show_add_mon_error = true;
                                    strcpy(add_mon_error_msg, "Cac ma hoc phan khong duoc trung nhau!");
                                    dup_ma = true;
                                    break;
                                }
                            }
                            if (dup_ma) break;
                        }

                        if (!dup_ma) {
                            char tenLower[30];
                            int lenLower = 0;
                            for (int i = 0; add_mon_ten[i]; i++) {
                                char cChar = add_mon_ten[i];
                                if (cChar != ' ' && cChar != '\t') {
                                    if (cChar >= 'A' && cChar <= 'Z') tenLower[lenLower++] = cChar + 32;
                                    else tenLower[lenLower++] = cChar;
                                }
                            }
                            tenLower[lenLower] = '\0';

                            MonHoc newMon;
                            strcpy(newMon.tenMon, add_mon_ten);
                            newMon.soHocPhan = add_mon_so_hp;

                            for (int i = 0; i < add_mon_so_hp; i++) {
                                char maHPLower[20];
                                int lenHP = 0;
                                for (int j = 0; add_mon_ma_hp[i][j]; j++) {
                                    char cChar = add_mon_ma_hp[i][j];
                                    if (cChar != ' ' && cChar != '\t') {
                                        if (cChar >= 'A' && cChar <= 'Z') maHPLower[lenHP++] = cChar + 32;
                                        else maHPLower[lenHP++] = cChar;
                                    }
                                }
                                maHPLower[lenHP] = '\0';

                                char duongDan[60];
                                sprintf(duongDan, "Res/%s_%s.dat", tenLower, maHPLower);

                                FILE *fp = fopen(duongDan, "w");
                                if (fp) {
                                    fprintf(fp, "%f %f %f %f 0 0 0 0 0 0 %d\n", add_mon_w_lab, add_mon_w_pt, add_mon_w_pre, add_mon_w_final, add_mon_max_sv);
                                    fclose(fp);
                                }

                                strcpy(newMon.dsHocPhan[i].maHP, add_mon_ma_hp[i]);
                                strcpy(newMon.dsHocPhan[i].fileHP, duongDan);
                            }

                            dsMonHoc[soMon] = newMon;
                            soMon++;
                            ghiFilemonhoc();

                            add_mon_ten[0] = '\0';
                            add_mon_so_hp = 1;
                            add_mon_max_sv = 40;
                            add_mon_w_lab = 0.1f;
                            add_mon_w_pt = 0.1f;
                            add_mon_w_pre = 0.2f;
                            add_mon_w_final = 0.4f;
                            for (int i = 0; i < MAX_HP; i++) add_mon_ma_hp[i][0] = '\0';
                            ImGui::CloseCurrentPopup();
                        }
                    }
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Huy", ImVec2(120, 0))) {
                add_mon_ten[0] = '\0';
                for (int i = 0; i < MAX_HP; i++) add_mon_ma_hp[i][0] = '\0';
                ImGui::CloseCurrentPopup();
            }

            if (show_add_mon_error) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", add_mon_error_msg);
                show_add_mon_error = false;
            }

            ImGui::EndPopup();
        }

        // Render cấu trúc giao diện đồ họa
        ImGui::Render();
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // 5. Thu dọn tài nguyên hệ thống
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}