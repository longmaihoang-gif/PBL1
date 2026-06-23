# TÀI LIỆU PHÂN TÍCH CHỨC NĂNG VÀ CƠ CHẾ HOẠT ĐỘNG - DỰ ÁN PBL1
Dự án: **Quản lý điểm sinh viên**
File mã nguồn chính: [PBL1_Code.c](PBL1_Code.c)

---

## 1. TỔNG QUAN HỆ THỐNG DỮ LIỆU
Hệ thống quản lý điểm sử dụng các file văn bản để lưu trữ trạng thái lâu dài bao gồm:
*   `monhoc.txt`: Lưu trữ danh sách môn học, số lượng học phần và đường dẫn file dữ liệu `.dat` tương ứng với mỗi học phần.
*   `sinhvien.txt`: Danh sách sinh viên tổng trong hệ thống (MSSV, Lớp, Họ tên).
*   `Res/<ten_mon>_<ma_hp>.dat`: File lưu dữ liệu điểm số riêng của từng học phần. Dòng đầu tiên chứa thông tin trọng số điểm, trạng thái khóa của các cột điểm và sĩ số tối đa của học phần. Các dòng tiếp theo lưu trữ chi tiết điểm số của từng sinh viên.

---

## 2. KIẾN TRÚC DỮ LIỆU (DATA STRUCTURES)
Hệ thống định nghĩa các cấu trúc dữ liệu sau trong [PBL1_Code.c](PBL1_Code.c):

| Tên Cấu Trúc | Chi Tiết Mã Nguồn | Chức Năng |
| :--- | :--- | :--- |
| **`SinhVien`** | [SinhVien](PBL1_Code.c#L13-L20) | Lưu trữ thông tin điểm chi tiết của một sinh viên trong học phần hiện tại bao gồm: mã sinh viên (`maSV`), lớp (`lop`), họ tên (`tenSV`), các cột điểm thành phần (`lab1`, `lab2`, `pt1`, `pt2`, `presentation`, `finalTest`), điểm trung bình (`dtb`), và xếp loại điểm chữ (`diemChu`). |
| **`HocPhan`** | [HocPhan](PBL1_Code.c#L29-L32) | Định nghĩa một học phần cụ thể của môn học bao gồm mã học phần (`maHP`) và đường dẫn file dữ liệu tương ứng (`fileHP`). |
| **`MonHoc`** | [MonHoc](PBL1_Code.c#L34-L38) | Định nghĩa thông tin môn học tổng quát gồm tên môn (`tenMon`), số lượng học phần hiện tại (`soHocPhan`) và danh sách các học phần thuộc môn học này (`dsHocPhan`). |
| **`SinhVienInfo`** | [SinhVienInfo](PBL1_Code.c#L43-L47) | Lưu trữ thông tin cơ bản của sinh viên trong danh sách tổng (không chứa thông tin điểm). |

---

## 3. DANH SÁCH CHỨC NĂNG & CƠ CHẾ HOẠT ĐỘNG
Dưới đây là chi tiết hoạt động của 8 tính năng tương ứng với các lựa chọn từ menu chính của hàm [main](PBL1_Code.c#L1413):

### 3.1. Nhập Điểm (Lựa chọn 1)
*   **Hàm chính xử lý:** [nhap](PBL1_Code.c#L339) và [nhapTheoCot](PBL1_Code.c#L303).
*   **Cách thức hoạt động:**
    1.  Hệ thống kiểm tra nếu tất cả cột điểm đều đã bị khóa bằng hàm [tatCaDaChot](PBL1_Code.c#L284), chức năng sẽ dừng và báo lỗi.
    2.  Người dùng lựa chọn nhập một cột điểm cụ thể (từ 1 đến 6) hoặc nhập liên tục tất cả các cột điểm (lựa chọn 7).
    3.  Với mỗi sinh viên trong danh sách học phần, chương trình hiển thị điểm hiện tại và yêu cầu nhập điểm mới:
        *   Để trống và nhấn **Enter**: Giữ nguyên điểm cũ (bỏ qua).
        *   Nhập `-2`: Dừng sớm quá trình nhập điểm của cột này.
        *   Nhập giá trị hợp lệ từ `0` đến `10`: Cập nhật điểm thành phần và tự động tính lại điểm trung bình bằng hàm [tinhDiem](PBL1_Code.c#L183).
    4.  Sau khi nhập xong, hệ thống hỏi người dùng có muốn lưu thay đổi vào file học phần không. Nếu có, chương trình gọi hàm [ghiFile](PBL1_Code.c#L197) để lưu trữ vĩnh viễn.

### 3.2. Xem Danh Sách (Lựa chọn 2)
*   **Hàm chính xử lý:** [xemDanhSach](PBL1_Code.c#L449) kết hợp với [docFileSinhVien](PBL1_Code.c#L684) và [chonHocPhan](PBL1_Code.c#L132).
*   **Cách thức hoạt động:** Người dùng có 2 lựa chọn con:
    1.  *Xem danh sách sinh viên tổng*: Đọc từ file `sinhvien.txt` bằng hàm [docFileSinhVien](PBL1_Code.c#L684) và in ra bảng gồm STT, MSSV, Lớp, Họ và tên.
    2.  *Xem bảng điểm học phần*: Yêu cầu chọn học phần qua hàm [chonHocPhan](PBL1_Code.c#L132), đọc file bằng hàm [docFile](PBL1_Code.c#L219) rồi in ra bảng điểm chi tiết.
        *   Nếu chưa nhập điểm thành phần, ô điểm sẽ để trống ký tự khoảng trắng `" "`.
        *   Đồng thời, hàm tự động thống kê tỷ lệ phần trăm phân loại học lực (A, B, C, D, F) dựa trên tổng số sinh viên đã có đầy đủ điểm của học phần đó.

### 3.3. Sửa Điểm (Lựa chọn 3)
*   **Hàm chính xử lý:** [sua](PBL1_Code.c#L564).
*   **Cách thức hoạt động:**
    1.  Yêu cầu nhập MSSV cần sửa.
    2.  Tìm kiếm sinh viên trong danh sách học phần hiện tại. Nếu tìm thấy, hiển thị menu con cho phép lựa chọn đầu điểm cần sửa:
        *   Sửa điểm Lab 1 & Lab 2.
        *   Sửa điểm PT 1 & PT 2.
        *   Sửa điểm Presentation.
        *   Sửa điểm Final Test.
        *   Sửa toàn bộ điểm chưa chốt.
    3.  Chương trình kiểm tra trạng thái chốt của cột điểm tương ứng thông qua mảng trạng thái `colLocked`: cột nào đã chốt sẽ hiển thị nhãn `[CHOT]` và không cho phép thay đổi dữ liệu.
    4.  Nhập dữ liệu mới xong, hệ thống gọi hàm [tinhDiem](PBL1_Code.c#L183) để cập nhật ngay Điểm trung bình và Điểm chữ, sau đó lưu lại file thông qua hàm [ghiFile](PBL1_Code.c#L197).

### 3.4. Sắp Xếp (Lựa chọn 4)
*   **Hàm chính xử lý:** [Sapxep](PBL1_Code.c#L1076) kết hợp với [layTenCuoi](PBL1_Code.c#L274).
*   **Cách thức hoạt động:**
    1.  Người dùng chọn tiêu chí sắp xếp:
        *   `1`: Mã số sinh viên (MSSV).
        *   `2`: Tên sinh viên (sử dụng hàm [layTenCuoi](PBL1_Code.c#L274) tách chữ cuối của Họ tên để so sánh bảng chữ cái; nếu trùng tên cuối sẽ tiếp tục so sánh toàn bộ Họ tên).
        *   `3`: Tên Lớp.
        *   `4`: Điểm trung bình (DTB).
    2.  Chọn kiểu sắp xếp: Tăng dần (`1`) hoặc Giảm dần (`2`).
    3.  Thuật toán Bubble Sort (Sắp xếp nổi bọt) được sử dụng để tráo đổi vị trí các phần tử trong mảng cấu trúc sinh viên `ds`.
    4.  Hiển thị danh sách kết quả sau khi sắp xếp, đồng thời hỏi người dùng có muốn ghi đè thứ tự sắp xếp mới này vào file học phần gốc hay không.

### 3.5. Xem Chi Tiết Điểm theo MSSV (Lựa chọn 5)
*   **Hàm chính xử lý:** [xemDiemChiTiet](PBL1_Code.c#L524) kết hợp với hàm kiểm tra [daCoDiem](PBL1_Code.c#L279).
*   **Cách thức hoạt động:**
    1.  Người dùng nhập MSSV cần tra cứu.
    2.  Hệ thống tìm kiếm tuyến tính trong danh sách.
    3.  Nếu tìm thấy, hệ thống sẽ in ra một phiếu liên lạc cá nhân dạng hộp văn bản vẽ bằng ký tự ascii, hiển thị đầy đủ thông tin trọng số phần trăm của từng đầu điểm, điểm số của từng cột, điểm trung bình tích lũy và học lực xếp loại (Điểm chữ).
    4.  Hỗ trợ vòng lặp cho phép nhấn phím `c` để tiếp tục tra cứu sinh viên khác mà không cần quay lại menu chính.

### 3.6. Chốt Cột Điểm (Lựa chọn 6)
*   **Hàm chính xử lý:** [Chotcotdiem](PBL1_Code.c#L1169) và hàm kiểm tra dữ liệu [cotCoDuDiem](PBL1_Code.c#L289).
*   **Cách thức hoạt động:**
    1.  In ra trạng thái hiện tại của cả 6 cột điểm (đã chốt / chưa chốt).
    2.  Cho phép chọn chốt một cột điểm cụ thể (từ 1 đến 6) hoặc chốt tất cả các cột điểm đủ điều kiện (lựa chọn 7).
    3.  **Ràng buộc nghiêm ngặt:** Hàm [cotCoDuDiem](PBL1_Code.c#L289) sẽ kiểm tra xem tất cả sinh viên trong học phần đã được nhập điểm cho cột này chưa (điểm >= 0). Nếu còn sinh viên thiếu điểm (điểm có giá trị âm mặc định là `-1`), chương trình sẽ từ chối chốt cột này để tránh lỗi tính toán.
    4.  Khi chốt thành công, biến cờ tương ứng trong mảng `colLocked` sẽ đổi trạng thái từ `0` sang `1` và ghi đè trạng thái cập nhật vào file dữ liệu `.dat`.

### 3.7. Quản Lý Sinh Viên (Lựa chọn 7)
Menu con quản lý sinh viên [quanLySinhVien](PBL1_Code.c#L1056) gồm 3 tính năng:
1.  **Thêm sinh viên mới (vào danh sách tổng):** Hàm [themSVMoi](PBL1_Code.c#L726).
    *   Yêu cầu nhập MSSV, kiểm tra xem MSSV đã tồn tại trong file `sinhvien.txt` chưa bằng hàm [timSVTheoMSSV](PBL1_Code.c#L717).
    *   Nếu chưa tồn tại, yêu cầu nhập tiếp Lớp, Họ tên rồi lưu lại vào `sinhvien.txt`.
2.  **Thêm sinh viên vào môn học:** Hàm [themSVVaoMonHoc](PBL1_Code.c#L767).
    *   Cho phép chọn môn học và chọn học phần cụ thể.
    *   Nhập MSSV cần thêm. Hệ thống tự động xác thực:
        *   Sinh viên phải tồn tại trong danh sách tổng `sinhvien.txt`.
        *   Sinh viên chưa đăng ký học phần này.
        *   **Luật kinh điển:** Sinh viên không được phép đăng ký học phần khác của cùng môn học đó (sử dụng hàm [svCoTrongFile](PBL1_Code.c#L111) để duyệt kiểm tra chéo các file dữ liệu học phần khác của môn).
        *   Học phần chưa vượt quá sĩ số tối đa (`soSVToiDa`).
    *   Nếu tất cả điều kiện thỏa mãn, thêm sinh viên mới vào cuối danh sách với tất cả điểm số khởi tạo bằng `-1`. Lưu thông tin cập nhật vào file `.dat`.
3.  **Loại bỏ sinh viên:** Hàm [loaiBoSinhVien](PBL1_Code.c#L1031).
    *   *Loại khỏi môn học:* Hàm [loaiBoSinhVienKhoiMonHoc](PBL1_Code.c#L911). Yêu cầu nhập MSSV để xóa. **Quy định chống gian lận:** Không cho phép xóa sinh viên đã hoàn thành điểm cuối kỳ (Final Test >= 0).
    *   *Loại khỏi hệ thống:* Hàm [loaiBoSinhVienKhoiHeThong](PBL1_Code.c#L965). Xóa sinh viên khỏi file `sinhvien.txt`, đồng thời duyệt qua toàn bộ môn học, học phần để tìm và xóa sinh viên đó khỏi tất cả các file học phần liên quan.

### 3.8. Thêm Môn Học Mới (Lựa chọn 8)
*   **Hàm chính xử lý:** [themMonHoc](PBL1_Code.c#L1230).
*   **Cách thức hoạt động:**
    1.  Đọc cơ sở dữ liệu môn học hiện tại bằng hàm [docFilemonhoc](PBL1_Code.c#L52).
    2.  Yêu cầu nhập tên môn học mới (kiểm tra không được để trống và không được trùng lặp môn cũ).
    3.  Nhập sĩ số tối đa của học phần.
    4.  Nhập phân bổ trọng số điểm số:
        *   Trọng số Lab (mỗi môn học gồm 2 bài Lab).
        *   Trọng số PT (mỗi môn học gồm 2 bài PT).
        *   Trọng số Presentation.
        *   Trọng số Final Test.
        *   **Ràng buộc toán học:** Tổng giá trị của công thức `(wLab * 2 + wPT * 2 + wPre + wFinal)` bắt buộc phải bằng `1.0` (dung sai cho phép sai lệch nhỏ trong khoảng 0.99 đến 1.01). Nếu không đúng, yêu cầu nhập lại từ đầu.
    5.  Nhập số lượng học phần cần tạo cho môn học đó. Với mỗi học phần, chương trình tạo file dữ liệu tương ứng đặt trong thư mục `Res/` dưới dạng `<ten_mon_viet_thuong>_<ma_hp_viet_thuong>.dat`.
    6.  Khởi tạo dòng đầu tiên của file dữ liệu học phần mới tạo với thông tin trọng số và sĩ số tối đa đã thiết lập.
    7.  Cập nhật lại danh sách môn học vào file cấu hình hệ thống `monhoc.txt` thông qua hàm [ghiFilemonhoc](PBL1_Code.c#L95).

---

## 4. BẢNG TỔNG HỢP CÁC RÀNG BUỘC LOGIC (BUSINESS RULES)

| Ràng Buộc | Phạm Vi | Cơ Chế Kiểm Soát |
| :--- | :--- | :--- |
| **Khóa điểm số** | Nhập/Sửa điểm | Mảng cờ `colLocked[6]`. Cột đã khóa không thể thay đổi dữ liệu trừ khi can thiệp vào file cấu hình. |
| **Điều kiện khóa** | Khóa cột điểm | Hàm [cotCoDuDiem](PBL1_Code.c#L289) kiểm tra không còn sinh viên nào mang điểm `-1` trên cột chỉ định. |
| **Công thức điểm** | Điểm Trung Bình | `DTB = (Lab1 + Lab2) * wLab + (PT1 + PT2) * wPT + Presentation * wPre + FinalTest * wFinal` |
| **Khung học lực** | Xếp loại điểm chữ | `DTB >= 8.5` $\to$ A; `DTB >= 7.0` $\to$ B; `DTB >= 5.5` $\to$ C; `DTB >= 4.0` $\to$ D; còn lại $\to$ F (nếu đã đủ các đầu điểm). |
| **Giới hạn đăng ký** | Thêm SV vào lớp | Mỗi sinh viên chỉ được tham gia tối đa 1 học phần đối với mỗi môn học để tránh học song song cùng một môn. |
| **Chống xóa học viên** | Loại bỏ sinh viên | Nếu sinh viên đã có điểm Final Test trong file học phần, hệ thống từ chối loại bỏ sinh viên đó khỏi lớp. |
| **Tổng trọng số** | Khởi tạo môn học | Bắt buộc kiểm tra `(wLab * 2 + wPT * 2 + wPre + wFinal) == 1.0`. |
