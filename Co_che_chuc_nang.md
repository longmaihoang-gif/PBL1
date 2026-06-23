# Tài Liệu Cơ Chế Hoạt Động Các Chức Năng Chính (PBL1_Code.c)

Tài liệu này giải thích chi tiết cơ chế hoạt động, luồng xử lý và vai trò hệ thống của các hàm chức năng chính tương ứng với Menu lựa chọn trong chương trình Quản lý điểm Sinh viên.

---

## 1. Chức năng 1: Nhập điểm (`nhap`)
*   **Mục tiêu**: Cho phép giảng viên nhập điểm cho toàn bộ lớp học phần ở một cột cụ thể hoặc nhập liên tục tất cả các cột điểm.
*   **Luồng xử lý**:
    1.  **Kiểm tra điều kiện**: Gọi hàm `tatCaDaChot()` để kiểm tra xem toàn bộ các cột điểm đã bị khóa hay chưa. Nếu đã khóa toàn bộ, thông báo lỗi và thoát.
    2.  **Lựa chọn cột**: Người dùng chọn một trong các cột từ 1 đến 6 (tương ứng với các điểm Lab, PT, Presentation, Final) hoặc chọn chế độ 7 (nhập liên tục tất cả các cột).
    3.  **Vòng lặp nhập điểm**: Duyệt qua danh sách sinh viên `ds`. Với mỗi sinh viên:
        *   Kiểm tra nếu cột điểm hiện tại đã bị khóa (`colLocked[idx] == 1`), chương trình sẽ bỏ qua sinh viên đó hoặc bỏ qua cả cột điểm.
        *   Nếu cột điểm chưa khóa, sao lưu giá trị cũ (`backup = *diemPtr`).
        *   Gọi hàm hỗ trợ `nhapTheoCot()`. Nếu giá trị điểm nhận về là `-999` (người dùng nhập `-2`), chương trình thực hiện khôi phục điểm cũ từ biến `backup`, đổi cờ dừng sớm `dungSom = 1` và thoát khỏi vòng lặp.
        *   Gọi hàm `tinhDiem()` để cập nhật điểm trung bình và xếp loại điểm chữ mới.
    4.  **Xác nhận lưu**: Sau khi hoàn thành nhập điểm, chương trình hỏi người dùng có muốn ghi đè kết quả xuống file học phần (.dat) hay không. Nếu có, thực hiện gọi hàm `ghiFile()`.

---

## 2. Chức năng 2: Xem danh sách (`xemDanhSach`)
*   **Mục tiêu**: Xuất bảng điểm chi tiết của lớp học phần hiện tại hoặc bảng danh sách tổng sinh viên trong hệ thống ra màn hình Console dưới dạng bảng được định dạng thẳng hàng.
*   **Luồng xử lý**:
    1.  **Xem danh sách tổng**: Đọc thông tin từ file `sinhvien.txt` lên mảng `dsSV` và hiển thị bảng gồm: MSSV, Lớp, Họ và tên.
    2.  **Xem bảng điểm học phần**:
        *   Người dùng chọn học phần cần xem. Chương trình gọi `docFile()` để nạp dữ liệu.
        *   Dựa trên biến cấu hình `cheDoTong`, chương trình hiển thị bảng điểm đầy đủ (MSSV, Lớp, Họ tên, 6 cột điểm thành phần, Điểm trung bình, Điểm chữ) hoặc bảng điểm rút gọn.
        *   Trong quá trình duyệt hiển thị, chương trình thực hiện thống kê số lượng sinh viên đạt các mức điểm chữ A, B, C, D, F.
        *   Tính toán và in ra tỷ lệ xếp loại phần trăm (`A%`, `B%`, `C%`, `D%`, `F%`) trên tổng số sinh viên đã có đủ điểm.

---

## 3. Chức năng 3: Sửa điểm (`sua`)
*   **Mục tiêu**: Hỗ trợ tìm kiếm sinh viên theo mã số (MSSV) và chỉnh sửa điểm số ở các cột điểm chưa bị khóa.
*   **Luồng xử lý**:
    1.  **Kiểm tra tổng quát**: Khóa thao tác nếu toàn bộ 6 cột điểm đã chốt.
    2.  **Tìm kiếm**: Nhập MSSV cần sửa. Duyệt mảng `ds` để tìm sinh viên trùng khớp.
    3.  **Menu sửa đổi**: Khi tìm thấy sinh viên, hiển thị menu chi tiết cho phép chọn sửa:
        *   *Sửa Lab 1 & 2*: Kiểm tra trạng thái khóa của từng cột Lab. Cho phép sửa Lab chưa bị khóa.
        *   *Sửa PT 1 & 2*: Kiểm tra trạng thái khóa của từng cột PT.
        *   *Sửa Presentation* hoặc *Final Test*: Kiểm tra trạng thái chốt tương ứng.
        *   *Sửa toàn bộ điểm*: Cho phép nhập lại tất cả các cột điểm chưa bị khóa trong một lượt.
    4.  **Cập nhật**: Giá trị điểm mới nhập vào sẽ đi qua hàm `clampDiem()` để chuẩn hóa về khoảng `[0.0, 10.0]`. Sau đó, chương trình tự động gọi `tinhDiem()` để cập nhật điểm trung bình và điểm chữ mới.

---

## 4. Chức năng 4: Sắp xếp (`Sapxep`)
*   **Mục tiêu**: Thay đổi thứ tự hiển thị của danh sách sinh viên trong lớp học phần dựa trên các tiêu chí cụ thể và cho phép lưu thứ tự mới vào file.
*   **Luồng xử lý**:
    1.  **Thiết lập tiêu chí**: Người dùng chọn tiêu chí sắp xếp (1: MSSV, 2: Tên sinh viên, 3: Lớp, 4: Điểm trung bình) và kiểu sắp xếp (1: Tăng dần, 2: Giảm dần).
    2.  **Lọc học phần**: Người dùng chọn học phần cụ thể cần sắp xếp. Chương trình gọi `docFile()` để nạp dữ liệu lên RAM.
    3.  **Thuật toán sắp xếp**: Sử dụng hai vòng lặp lồng nhau (thuật toán Bubble Sort) để so sánh cặp phần tử `i` và `j`:
        *   Sử dụng hàm `strcmp` đối với chuỗi (MSSV, Lớp, Tên). Đối với tên, gọi hàm `layTenCuoi` để lấy từ cuối cùng so sánh trước; nếu từ cuối giống nhau thì so sánh toàn bộ chuỗi họ tên.
        *   So sánh toán tử `>` hoặc `<` đối với số thực (Điểm trung bình).
        *   Nếu thỏa mãn điều kiện sắp xếp, tiến hành hoán vị vị trí hai cấu trúc `SinhVien` trong mảng `ds`.
    4.  **Hiển thị và lưu trữ**: In danh sách sau khi sắp xếp ra màn hình và hỏi người dùng có muốn ghi đè thứ tự mới này vào file học phần (.dat) hay không.

---

## 5. Chức năng 5: Xem điểm chi tiết (`xemDiemChiTiet`)
*   **Mục tiêu**: Tìm kiếm và kết xuất bảng điểm thành phần chi tiết của một sinh viên cụ thể dưới dạng phiếu điểm đóng khung trực quan.
*   **Luồng xử lý**:
    1.  Nhập MSSV cần tra cứu.
    2.  Duyệt danh sách sinh viên trong học phần hiện tại.
    3.  Nếu tìm thấy sinh viên:
        *   Kiểm tra xem sinh viên đã có đủ điểm chưa bằng cách gọi hàm `daCoDiem()`.
        *   Chuyển đổi các điểm số từ kiểu số thực sang định dạng chuỗi hiển thị. Đối với các cột điểm chưa có (giá trị âm), hiển thị khoảng trắng để tránh in ra giá trị âm.
        *   In ra màn hình Console khung phiếu điểm chi tiết chứa đầy đủ thông tin: MSSV, Họ tên, Lớp, điểm chi tiết kèm theo phần trăm trọng số của từng đầu điểm, Điểm trung bình và Điểm chữ tương ứng.
    4.  Nếu không tìm thấy, in thông báo lỗi. Cho phép bấm phím `c` để tiếp tục tra cứu sinh viên khác.

---

## 6. Chức năng 6: Chốt cột điểm (`Chotcotdiem`)
*   **Mục tiêu**: Khóa vĩnh viễn các cột điểm đã nhập đủ điểm cho toàn bộ lớp học phần nhằm bảo mật thông tin, ngăn chặn hành vi sửa điểm ngoài ý muốn.
*   **Luồng xử lý**:
    1.  Hiển thị danh sách 6 cột điểm kèm theo trạng thái hiện tại (ĐÃ CHỐT hoặc CHƯA CHỐT).
    2.  Người dùng chọn chốt một cột điểm cụ thể (từ 1 đến 6) hoặc chốt tất cả các cột chưa chốt (lựa chọn 7).
    3.  **Kiểm tra tính hợp lệ**:
        *   Nếu cột điểm đó đã chốt từ trước, thông báo lỗi.
        *   Gọi hàm `cotCoDuDiem()` để kiểm tra xem cột điểm cần chốt đã được nhập điểm đầy đủ cho toàn bộ sinh viên trong lớp hay chưa. Nếu còn sinh viên chưa có điểm hoặc lớp chưa có sinh viên, từ chối chốt.
    4.  **Khóa điểm**: Nếu đủ điều kiện, chương trình yêu cầu người dùng xác nhận lại (y/n) vì thao tác chốt điểm không thể hoàn tác. Nếu người dùng chọn `y`, gán `colLocked[idx] = 1` và gọi `ghiFile()` để lưu trạng thái khóa xuống file dữ liệu.

---

## 7. Chức năng 7: Quản lý sinh viên (`quanLySinhVien`)
*   **Mục tiêu**: Quản lý thông tin hồ sơ và tình trạng đăng ký lớp học phần của sinh viên thông qua menu con.
*   **Luồng xử lý**:
    1.  **Thêm sinh viên mới (`themSVMoi`)**: Đọc file `sinhvien.txt`. Nhập MSSV mới, nếu chưa tồn tại thì nhập tiếp Họ tên và Lớp, sau đó ghi thêm thông tin sinh viên vào cuối file `sinhvien.txt`.
    2.  **Thêm sinh viên vào học phần (`themSVVaoMonHoc`)**:
        *   Người dùng chọn học phần cần thêm sinh viên.
        *   Chương trình kiểm tra xem học phần đó đã chốt cột điểm nào chưa. Nếu đã có ít nhất một cột điểm bị chốt, từ chối thêm sinh viên mới.
        *   Nhập MSSV cần thêm. Kiểm tra MSSV có nằm trong file sinh viên tổng `sinhvien.txt` không; kiểm tra sinh viên đã đăng ký lớp học phần này chưa.
        *   Duyệt qua tất cả các lớp học phần khác của cùng môn học đó, gọi hàm `svCoTrongFile()` để đảm bảo sinh viên chưa đăng ký học phần nào khác của môn học này.
        *   Nếu đạt mọi điều kiện và chưa vượt quá sĩ số tối đa (`soSVToiDa`), ghi thêm sinh viên vào mảng `ds` với các điểm khởi tạo ban đầu bằng `-1` (chưa có điểm) và lưu lại file học phần.
    3.  **Loại bỏ sinh viên (`loaiBoSinhVien`)**:
        *   *Loại khỏi học phần*: Tìm sinh viên trong lớp học phần. Nếu sinh viên đã có điểm thi cuối kỳ (`finalTest >= 0`), chương trình sẽ từ chối loại bỏ. Nếu chưa thi cuối kỳ, thực hiện dịch chuyển mảng để xóa sinh viên và ghi lại file học phần.
        *   *Loại khỏi hệ thống*: Xóa sinh viên khỏi danh sách tổng `sinhvien.txt`. Đồng thời duyệt qua tất cả các file học phần của toàn bộ các môn học trong hệ thống, thực hiện xóa sinh viên đó khỏi mọi lớp học phần đã đăng ký để đảm bảo tính đồng bộ dữ liệu.

---

## 8. Chức năng 8: Thêm môn học mới (`themMonHoc`)
*   **Mục tiêu**: Khởi tạo môn học mới, thiết lập số lượng học phần trực thuộc và cấu hình hệ thống tính điểm cho môn học đó.
*   **Luồng xử lý**:
    1.  Nhập tên môn học mới (kiểm tra không được để trống và không trùng với môn học đã có).
    2.  Nhập số lượng sinh viên tối đa cho các lớp học phần thuộc môn này.
    3.  **Thiết lập trọng số**: Nhập các trọng số điểm thành phần (`w_Lab`, `w_PT`, `w_Pre`, `w_Final`). Chương trình thực hiện kiểm tra công thức: `Tổng trọng số = w_Lab * 2 + w_PT * 2 + w_Pre + w_Final`. Yêu cầu tổng trọng số phải xấp xỉ bằng `1.0` (cho phép sai số nhỏ từ `0.99` đến `1.01`). Nếu sai lệch, bắt buộc người dùng nhập lại.
    4.  Nhập số lượng lớp học phần cần tạo.
    5.  **Khởi tạo tệp tin**: Với mỗi học phần, người dùng nhập mã lớp học phần. Chương trình tự động chuyển tên môn học và mã học phần thành chữ thường, không khoảng trắng để tạo đường dẫn tệp dạng `Res/<tên_môn>_<mã_hp>.dat`.
        *   Nếu file dữ liệu đã tồn tại trên đĩa cứng, chương trình cảnh báo và hỏi người dùng có muốn ghi đè không.
        *   Nếu đồng ý ghi đè hoặc tạo mới, chương trình tạo file và ghi dòng cấu hình đầu tiên chứa trọng số điểm mới thiết lập, trạng thái khóa ban đầu của các cột điểm (`0 0 0 0 0 0`), và sĩ số tối đa của lớp học phần.
    6.  Cập nhật thông tin môn học mới vào mảng `dsMonHoc` và ghi đè file `monhoc.txt`.
