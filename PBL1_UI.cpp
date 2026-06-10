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

#define MAX 100
#define MAX_MON 100
#define MIN_TC 3

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
    int soTinChiTichLuy;
} SinhVien;

SinhVien ds[MAX];
int n = 0;
int colLocked[6] = {0, 0, 0, 0, 0, 0};
char *tenCotGlobal[] = {"Lab1", "Lab2", "PT1", "PT2", "Presentation", "Final"};
float wLab, wPT, wPre, wFinal;
int soSVToiDa = 0;

typedef struct {
    char maHP[20];
    char fileHP[50];
} HocPhan;

typedef struct {
    char tenMon[30];
    int soHocPhan;
    HocPhan dsHocPhan[10];
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
        dsMonHoc[soMon].soHocPhan = atoi(token);
        int i;
        for (i = 0; i < dsMonHoc[soMon].soHocPhan && i < 10; i++) {
            token = strtok(NULL, "\t");
            if (!token) break;
            strncpy(dsMonHoc[soMon].dsHocPhan[i].maHP, token, 19);
            dsMonHoc[soMon].dsHocPhan[i].maHP[19] = '\0';
            token = strtok(NULL, "\t");
            if (!token) break;
            strncpy(dsMonHoc[soMon].dsHocPhan[i].fileHP, token, 49);
            dsMonHoc[soMon].dsHocPhan[i].fileHP[49] = '\0';
        }
        soMon++;
    }
    fclose(fp);
}

void ghiFilemonhoc() {
    FILE *fp = fopen("monhoc.txt", "w");
    if (!fp) return;
    int i, j;
    for (i = 0; i < soMon; i++) {
        fprintf(fp, "%s\t%d", dsMonHoc[i].tenMon, dsMonHoc[i].soHocPhan);
        for (j = 0; j < dsMonHoc[i].soHocPhan; j++) {
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

    // 4. VÒNG LẶP RENDER CHÍNH
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --------------------------------------------------------------------
        // CỬA SỔ TRÁI: THANH ĐIỀU HƯỚNG
        // --------------------------------------------------------------------
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(350, 680), ImGuiCond_Always);
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

        if (ImGui::BeginPopupModal("LoiTrungMSSV", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Ma so sinh vien nay da ton tai trong he thong!");
            if (ImGui::Button("Dong", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

        ImGui::End();

        // --------------------------------------------------------------------
        // CỬA SỔ PHẢI: BẢNG LƯỚI ĐIỂM CHI TIẾT
        // --------------------------------------------------------------------
        ImGui::SetNextWindowPos(ImVec2(370, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(890, 680), ImGuiCond_Always);
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

            if (ImGui::BeginTable("StudentTable", 11, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY, ImVec2(0, 450))) {
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
                ImGui::TableHeadersRow();

                for (int i = 0; i < n; i++) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0); ImGui::Text("%d", i + 1);
                    ImGui::TableSetColumnIndex(1); ImGui::Text("%s", ds[i].maSV);
                    ImGui::TableSetColumnIndex(2); ImGui::Text("%s", ds[i].tenSV);

                    // Sửa đổi ID tĩnh thành ID động dạng ID_##i để tránh xung đột dữ liệu ImGui giữa các dòng
                    ImGui::TableSetColumnIndex(3);
                    if (colLocked[0]) ImGui::Text("%.1f", ds[i].lab1); 
                    else { ImGui::PushItemWidth(-FLT_MIN); char id[20]; sprintf(id, "##L1_%d", i); if (ImGui::InputFloat(id, &ds[i].lab1, 0.0f, 0.0f, "%.1f")) tinhDiem(&ds[i]); ImGui::PopItemWidth(); }

                    ImGui::TableSetColumnIndex(4);
                    if (colLocked[1]) ImGui::Text("%.1f", ds[i].lab2); 
                    else { ImGui::PushItemWidth(-FLT_MIN); char id[20]; sprintf(id, "##L2_%d", i); if (ImGui::InputFloat(id, &ds[i].lab2, 0.0f, 0.0f, "%.1f")) tinhDiem(&ds[i]); ImGui::PopItemWidth(); }

                    ImGui::TableSetColumnIndex(5);
                    if (colLocked[2]) ImGui::Text("%.1f", ds[i].pt1); 
                    else { ImGui::PushItemWidth(-FLT_MIN); char id[20]; sprintf(id, "##P1_%d", i); if (ImGui::InputFloat(id, &ds[i].pt1, 0.0f, 0.0f, "%.1f")) tinhDiem(&ds[i]); ImGui::PopItemWidth(); }

                    ImGui::TableSetColumnIndex(6);
                    if (colLocked[3]) ImGui::Text("%.1f", ds[i].pt2); 
                    else { ImGui::PushItemWidth(-FLT_MIN); char id[20]; sprintf(id, "##P2_%d", i); if (ImGui::InputFloat(id, &ds[i].pt2, 0.0f, 0.0f, "%.1f")) tinhDiem(&ds[i]); ImGui::PopItemWidth(); }

                    ImGui::TableSetColumnIndex(7);
                    if (colLocked[4]) ImGui::Text("%.1f", ds[i].presentation); 
                    else { ImGui::PushItemWidth(-FLT_MIN); char id[20]; sprintf(id, "##Pre_%d", i); if (ImGui::InputFloat(id, &ds[i].presentation, 0.0f, 0.0f, "%.1f")) tinhDiem(&ds[i]); ImGui::PopItemWidth(); }

                    ImGui::TableSetColumnIndex(8);
                    if (colLocked[5]) ImGui::Text("%.1f", ds[i].finalTest); 
                    else { ImGui::PushItemWidth(-FLT_MIN); char id[20]; sprintf(id, "##Fin_%d", i); if (ImGui::InputFloat(id, &ds[i].finalTest, 0.0f, 0.0f, "%.1f")) tinhDiem(&ds[i]); ImGui::PopItemWidth(); }

                    ImGui::TableSetColumnIndex(9); ImGui::TextColored(ImVec4(0,1,0,1), "%.2f", ds[i].dtb);
                    ImGui::TableSetColumnIndex(10); ImGui::Text("%s", ds[i].diemChu);
                }
                ImGui::EndTable();
            }

            ImGui::Spacing();
            
            if (ImGui::Button("Luu Thay Doi Diem", ImVec2(200, 40))) {
                ghiFile(fileHPHienTai);
            }
            ImGui::SameLine();
            if (ImGui::Button("Chot Toan Bo Cot", ImVec2(200, 40))) {
                for(int ci=0; ci<6; ci++) colLocked[ci] = 1;
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
            ImGui::Text("Diem Lab 1: %.1f | Lab 2: %.1f", sv.lab1, sv.lab2);
            ImGui::Text("Diem PT 1:  %.1f | PT 2:  %.1f", sv.pt1, sv.pt2);
            ImGui::Text("Thuyet trinh: %.1f", sv.presentation);
            ImGui::Text("Thi cuoi ky:  %.1f", sv.finalTest);
            ImGui::Separator();
            ImGui::TextColored(ImVec4(1,0,0,1), "DIEM TRUNG BINH: %.2f", sv.dtb);
            ImGui::Text("XEP LOAI: %s", sv.diemChu);
            if (ImGui::Button("Dong")) { show_student_detail = false; }
            ImGui::End();
        }

        // Render cấu trúc giao diện đồ họa
        ImGui::Render();
        int display_w, display_h;
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