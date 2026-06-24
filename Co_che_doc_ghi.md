# Tài Liệu Cơ Chế Hoạt Động Các Hàm Đọc/Ghi File (PBL1_Code.c)

Tài liệu này phân tích chi tiết cơ chế hoạt động, định dạng cấu trúc tệp dữ liệu, phương pháp phân tích chuỗi (parsing) và luồng ghi nhận thông tin của các hàm đọc/ghi file trong chương trình Quản lý điểm Sinh viên.

---

## 1. Cơ chế Quản lý Danh sách Môn học (`monhoc.txt`)

### A. Hàm đọc dữ liệu môn học (`docFilemonhoc`)
*   **Chế độ mở file**: Mở file `"monhoc.txt"` ở chế độ đọc (`"r"`). Nếu tệp chưa tồn tại, hàm tự động tạo tệp mới ở chế độ ghi (`"w"`), đóng lại ngay và thiết lập số môn học `soMon = 0`.
*   **Định dạng dòng dữ liệu**: Mỗi dòng lưu thông tin của một môn học gồm:
    `[Tên môn học]\t[Số học phần]\t[Mã HP 1]\t[File HP 1]\t[Mã HP 2]\t[File HP 2]...` (phân cách bằng phím Tab `\t`).
*   **Cơ chế phân tích chuỗi (Parsing)**:
    1.  Dùng `fgets` đọc từng dòng văn bản (tối đa 1024 ký tự).
    2.  Dùng `strcspn(line, "\r\n")` để tìm vị trí ký tự xuống dòng và thay thế bằng ký tự kết thúc chuỗi `\0` để loại bỏ ký tự xuống dòng thừa.
    3.  Dùng hàm tách chuỗi `strtok(line, "\t")` để lấy token đầu tiên làm **Tên môn học**.
    4.  Gọi `strtok(NULL, "\t")` lấy token thứ hai, dùng `atoi()` chuyển đổi thành số nguyên để xác định **Số lượng học phần**.
    5.  Chạy vòng lặp duyệt qua số lượng học phần, mỗi lượt lặp thực hiện gọi liên tiếp hai lần `strtok(NULL, "\t")` để lần lượt trích xuất **Mã học phần** và **Đường dẫn tệp cấu hình điểm**.

### B. Hàm ghi dữ liệu môn học (`ghiFilemonhoc`)
*   **Chế độ mở file**: Ghi đè file `"monhoc.txt"` ở chế độ ghi (`"w"`).
*   **Quy trình ghi**:
    *   Duyệt qua mảng cấu trúc môn học `dsMonHoc` từ chỉ số `0` đến `soMon - 1`.
    *   Sử dụng `fprintf` để ghi Tên môn học và Số học phần: `fprintf(fp, "%s\t%d", dsMonHoc[i].tenMon, limit);`.
    *   Duyệt qua danh sách học phần của môn đó để ghi tiếp mã lớp học phần và tệp lưu điểm tương ứng phân cách bởi ký tự `\t`: `fprintf(fp, "\t%s\t%s", ...);`.
    *   Kết thúc dòng của mỗi môn học bằng ký tự xuống dòng `\n`.

---

## 2. Cơ chế Quản lý Danh sách Sinh viên Tổng (`sinhvien.txt`)

### A. Hàm đọc danh sách sinh viên (`docFileSinhVien`)
*   **Chế độ mở file**: Mở file `"sinhvien.txt"` ở chế độ đọc (`"r"`).
*   **Định dạng dòng dữ liệu**: Mỗi dòng biểu diễn thông tin một sinh viên dưới dạng:
    `[MSSV]\t[Lớp]\t[Họ và tên]`
*   **Cơ chế phân tích chuỗi (Parsing)**:
    *   Đọc dòng bằng `fgets`, loại bỏ ký tự xuống dòng.
    *   Hàm không sử dụng `strtok` mà định vị ký tự Tab `\t` trực tiếp bằng hàm tìm kiếm ký tự `strchr`.
    *   Tìm Tab thứ nhất: `p1 = strchr(line, '\t')`. Thay thế ký tự Tab bằng `\0` để cắt chuỗi lấy **MSSV** (`line`).
    *   Tìm Tab thứ hai từ vị trí sau Tab thứ nhất: `p2 = strchr(p1 + 1, '\t')`. Thay thế Tab thứ hai bằng `\0` để cắt chuỗi lấy **Lớp** (`p1 + 1`).
    *   Phần còn lại sau Tab thứ hai chính là **Họ và tên** (`p2 + 1`).
    *   Sao chép các chuỗi con thu được vào mảng cấu trúc `dsSV` và tăng biến đếm sĩ số tổng `nSV`.

### B. Hàm ghi danh sách sinh viên (`ghiFileSinhVien`)
*   **Chế độ mở file**: Ghi đè file `"sinhvien.txt"` ở chế độ ghi (`"w"`).
*   **Quy trình ghi**: Duyệt từ `0` đến `nSV - 1`, ghi thông tin sinh viên phân cách bằng Tab:
    `fprintf(fp, "%s\t%s\t%s\n", dsSV[i].maSV, dsSV[i].lop, dsSV[i].tenSV);`

---

## 3. Cơ chế Quản lý Điểm số Học phần (`.dat`)

Các tệp này lưu cấu hình điểm và điểm số chi tiết của từng lớp học phần (ví dụ: `Res/tin_nh25.dat`).

### A. Hàm đọc dữ liệu điểm học phần (`docFile`)
*   **Chế độ mở file**: Mở file được chỉ định ở chế độ đọc (`"r"`). Gán tên file đang làm việc vào biến toàn cục `fileHPHienTai`.
*   **Cấu trúc dữ liệu trong file**:
    *   **Dòng 1 (Cấu hình)**: Chứa trọng số điểm thành phần, trạng thái khóa của 6 cột điểm, và sĩ số tối đa của lớp học phần.
        `[wLab] [wPT] [wPre] [wFinal] [khóa_Lab1] [khóa_Lab2] [khóa_PT1] [khóa_PT2] [khóa_Pre] [khóa_Final] [sĩ_số_tối_đa]`
    *   **Các dòng tiếp theo (Dữ liệu điểm)**: Chứa thông tin và điểm số chi tiết của từng sinh viên.
        `[MSSV] [Lớp] [Họ và tên] [điểm_Lab1] [điểm_Lab2] [điểm_PT1] [điểm_PT2] [điểm_Pre] [điểm_Final]`
*   **Cơ chế phân tích chuỗi (Parsing)**:
    1.  **Phân tích dòng 1**:
        *   Dùng `fgets` đọc dòng đầu tiên.
        *   Dùng `sscanf(line, "%f %f %f %f %d %d %d %d %d %d %d", ...)` để phân tích ra 11 giá trị cấu hình tương ứng.
        *   *Tương thích ngược*: Nếu số lượng giá trị phân tích được ít hơn 4 (file không có dòng tiêu đề cấu hình), chương trình tự động gán trọng số mặc định (`wLab=0.1, wPT=0.1, wPre=0.2, wFinal=0.4`), mở khóa toàn bộ cột điểm, đặt sĩ số tối đa là 0, và gọi lệnh `rewind(fp)` để đưa con trỏ file quay lại từ đầu file để bắt đầu đọc dữ liệu sinh viên ngay từ dòng đầu tiên.
    2.  **Phân tích các dòng tiếp theo (Danh sách điểm)**:
        *   Sử dụng vòng lặp `fgets` đọc từng dòng cho đến cuối file.
        *   Gọi hàm phân tích chuỗi định dạng nâng cao:
            `sscanf(line, "%19s %19s %[^0-9.-] %f %f %f %f %f %f", ...)`
            *   `%19s`: Đọc tối đa 19 ký tự không có khoảng trắng cho MSSV và Lớp.
            *   `%[^0-9.-]`: Bộ lọc chuỗi đặc biệt. Biểu thức này yêu cầu đọc toàn bộ chuỗi ký tự cho đến khi gặp chữ số (`0-9`), dấu chấm (`.`), hoặc dấu trừ (`-`). Định dạng này giúp chương trình trích xuất chính xác chuỗi Họ và tên chứa khoảng trắng của sinh viên.
            *   Các tham số `%f` tiếp theo lần lượt trích xuất 6 cột điểm thành phần.
        *   Loại bỏ các ký tự khoảng trắng hoặc Tab thừa ở cuối chuỗi Họ và Tên sinh viên bằng cách duyệt từ cuối chuỗi về trước.
        *   Gọi hàm `tinhDiem(&ds[n])` để tính toán DTB cùng xếp loại điểm chữ ban đầu, sau đó tăng sĩ số học phần `n`.

### B. Hàm ghi dữ liệu điểm học phần (`ghiFile`)
*   **Chế độ mở file**: Ghi đè file học phần chỉ định ở chế độ ghi (`"w"`).
*   **Quy trình ghi**:
    1.  Ghi dòng cấu hình đầu tiên (trọng số điểm, trạng thái khóa của 6 cột điểm, sĩ số tối đa lớp học phần):
        `fprintf(fp, "%f %f %f %f %d %d %d %d %d %d %d\n", wLab, wPT, wPre, wFinal, colLocked[0] ... colLocked[5], soSVToiDa);`
    2.  Duyệt qua danh sách sinh viên của lớp học phần hiện tại từ `0` đến `n - 1`, ghi thông tin và các điểm số của từng sinh viên:
        `fprintf(fp, "%s %s %s %f %f %f %f %f %f\n", ds[i].maSV, ds[i].lop, ds[i].tenSV, ds[i].lab1, ds[i].lab2, ds[i].pt1, ds[i].pt2, ds[i].presentation, ds[i].finalTest);`
