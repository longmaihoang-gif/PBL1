# Tài Liệu Cơ Chế Hoạt Động Các Hàm Phụ Trợ (PBL1_Code.c)

Tài liệu này giải thích chi tiết cơ chế hoạt động, tham số, giá trị trả về và vai trò hệ thống của các hàm phụ trợ (hành động hỗ trợ / utility skills) trong chương trình Quản lý điểm Sinh viên.

---

## 1. Hàm `layTenCuoi`
*   **Mục tiêu**: Trích xuất phần tên chính từ chuỗi họ và tên đầy đủ của sinh viên để phục vụ sắp xếp theo thứ tự bảng chữ cái.
*   **Nguyên lý hoạt động**:
    *   Sử dụng hàm thư viện `strrchr(hoTen, ' ')` để tìm kiếm ký tự khoảng trắng `' '` xuất hiện cuối cùng trong chuỗi.
    *   Hàm `strrchr` trả về con trỏ trỏ tới vị trí của khoảng trắng đó. Nếu tìm thấy, con trỏ được dịch lên 1 vị trí (`p + 1`) để trỏ đến ký tự đầu tiên của phần tên chính (bỏ qua khoảng trắng).
    *   Nếu không tìm thấy khoảng trắng (tên chỉ gồm 1 từ như "An", "Bình"), hàm trả về toàn bộ chuỗi ban đầu.
*   **Ví dụ**: `"Nguyễn Văn An"` -> `strrchr` tìm thấy khoảng trắng trước chữ `"An"`, dịch sang phải 1 ký tự và trả về `"An"`.

---

## 2. Hàm `daCoDiem`
*   **Mục tiêu**: Xác định xem một sinh viên đã hoàn thành đầy đủ tất cả 6 đầu điểm thành phần hay chưa.
*   **Nguyên lý hoạt động**:
    *   Hàm nhận vào một cấu trúc `SinhVien` và kiểm tra tất cả các cột điểm: `lab1`, `lab2`, `pt1`, `pt2`, `presentation`, `finalTest`.
    *   Nếu bất kỳ cột điểm nào có giá trị âm (`< 0` - trạng thái mặc định `-1` khi sinh viên mới được thêm vào lớp hoặc chưa nhập điểm), hàm trả về `false`.
    *   Ngược lại, nếu tất cả các điểm đều lớn hơn hoặc bằng `0`, hàm trả về `true`.
*   **Ý nghĩa**: Giúp phân biệt trạng thái hiển thị điểm trung bình và xếp loại học lực (chỉ tính toán và xếp loại khi đã nhập đủ cả 6 đầu điểm thành phần).

---

## 3. Hàm `tatCaDaChot`
*   **Mục tiêu**: Kiểm tra trạng thái khóa (chốt) điểm của toàn bộ lớp học phần hiện tại.
*   **Nguyên lý hoạt động**:
    *   Duyệt qua mảng trạng thái khóa cột toàn cục `colLocked` gồm 6 phần tử (tương ứng với 6 cột điểm).
    *   Nếu phát hiện bất kỳ phần tử nào bằng `0` (chưa chốt), hàm ngay lập tức trả về `0` (False).
    *   Nếu duyệt qua toàn bộ mảng và tất cả đều bằng `1`, hàm trả về `1` (True).
*   **Ý nghĩa**: Dùng để khóa tính năng nhập hoặc sửa điểm của lớp học phần nếu toàn bộ các cột điểm đã được xác nhận chốt.

---

## 4. Hàm `cotCoDuDiem`
*   **Mục tiêu**: Xác định xem một cột điểm cụ thể đã được nhập điểm đầy đủ cho toàn bộ danh sách sinh viên trong lớp học phần hay chưa.
*   **Nguyên lý hoạt động**:
    *   Nhận vào chỉ số cột `colIndex` (từ 0 đến 5).
    *   Nếu sĩ số lớp hiện tại bằng 0, hàm trả về `0`.
    *   Duyệt qua toàn bộ danh sách sinh viên `ds`. Dựa vào `colIndex` để truy xuất điểm tương ứng (0: `lab1`, 1: `lab2`, 2: `pt1`, 3: `pt2`, 4: `presentation`, 5: `finalTest`).
    *   Nếu phát hiện bất kỳ sinh viên nào có điểm âm (`val < 0`), hàm lập tức trả về `0`.
    *   Nếu mọi sinh viên đều có điểm hợp lệ, hàm trả về `1`.
*   **Ý nghĩa**: Đây là điều kiện tiên quyết để chốt một cột điểm. Không thể chốt cột điểm nếu vẫn còn sinh viên chưa được nhập điểm trong lớp học phần đó.

---

## 5. Hàm `clampDiem`
*   **Mục tiêu**: Giới hạn điểm số nhập vào luôn nằm trong phạm vi hợp lệ `[0.0, 10.0]`.
*   **Nguyên lý hoạt động**:
    *   Nếu giá trị truyền vào nhỏ hơn `0.0f`, trả về `0.0f`.
    *   Nếu giá trị truyền vào lớn hơn `10.0f`, trả về `10.0f`.
    *   Ngược lại, giữ nguyên giá trị ban đầu.
*   **Ý nghĩa**: Ngăn ngừa lỗi tràn hoặc sai lệch dữ liệu điểm số khi người dùng vô tình nhập sai ngoài khoảng quy định.

---

## 6. Hàm `svCoTrongFile`
*   **Mục tiêu**: Kiểm tra xem sinh viên đã đăng ký học một lớp học phần nào khác của cùng môn học hay chưa (ràng buộc đăng ký chéo).
*   **Nguyên lý hoạt động**:
    *   Mở file dữ liệu học phần được chỉ định (`tenFile`) ở chế độ đọc `"r"`.
    *   Duyệt qua từng dòng của file, bỏ qua dòng đầu tiên (chứa thông tin cấu hình trọng số).
    *   Sử dụng `sscanf` để trích xuất MSSV đầu dòng và so sánh với MSSV cần kiểm tra.
    *   Nếu tìm thấy trùng khớp, đóng file và trả về `true`. Nếu duyệt hết file mà không thấy, trả về `false`.
*   **Ý nghĩa**: Thực thi quy tắc: Mỗi sinh viên chỉ được phép đăng ký duy nhất 1 lớp học phần của mỗi môn học để tránh học song song.

---

## 7. Hàm `timSVTheoMSSV`
*   **Mục tiêu**: Tra cứu vị trí của sinh viên trong danh sách tổng sinh viên của hệ thống.
*   **Nguyên lý hoạt động**:
    *   Duyệt qua mảng toàn cục `dsSV` chứa thông tin sinh viên tổng (sĩ số `nSV`).
    *   So sánh mã sinh viên của từng phần tử với MSSV cần tìm qua hàm `strcmp`.
    *   Nếu trùng khớp, trả về ngay chỉ mục (chỉ số index) kiểu `int` trong mảng.
    *   Nếu duyệt hết mảng mà không thấy, trả về `-1`.
*   **Ý nghĩa**: Giúp định vị nhanh phần tử sinh viên trên bộ nhớ RAM để lấy thông tin họ tên, lớp học khi đăng ký vào lớp học phần mới hoặc khi thực hiện xóa khỏi hệ thống.

---

## 8. Hàm `nhapTheoCot`
*   **Mục tiêu**: Hỗ trợ nhập và chuẩn hóa điểm số của một sinh viên tại một cột điểm cụ thể.
*   **Nguyên lý hoạt động**:
    *   Đọc chuỗi đầu vào từ bàn phím bằng `fgets` để tránh lỗi trôi dòng lệnh khi nhấn Enter.
    *   Loại bỏ các ký tự xuống dòng (`\r`, `\n`) ở cuối chuỗi.
    *   Nếu chuỗi rỗng (người dùng chỉ nhấn Enter), bỏ qua và giữ nguyên điểm hiện tại.
    *   Nếu chuỗi nhập vào là `"-2"`, gán giá trị đặc biệt `-999` cho điểm số của sinh viên để gửi tín hiệu dừng khẩn cấp cho hàm gọi.
    *   Ngược lại, chuyển đổi chuỗi thành số thực bằng `atof`, đi qua bộ lọc `clampDiem` và gán lại cho điểm của sinh viên.
