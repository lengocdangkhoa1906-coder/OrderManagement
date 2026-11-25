# OrderManagement

Chương trình C++17 cho đề tài 5, Quản lý đơn đặt hàng trực tuyến. Mỗi đơn có mã, tên khách hàng, ngày tạo `YYYY-MM-DD`, tổng tiền VND và một trong bốn trạng thái: chờ duyệt, đã duyệt, đã hủy, hoàn thành.

## Biên dịch và chạy

Mở thư mục chứa `CMakeLists.txt` trong Visual Studio bằng **File → Open → Folder** với workload **Desktop development with C++**. Chọn mục tiêu `OrderManagement` để chạy. Trong VS Code, có thể dùng CMake Tools hoặc chạy từ terminal đã cấu hình trình biên dịch C++:

```sh
cmake -S . -B build
cmake --build build --config Debug
```

Trên Windows, chạy `build/Debug/OrderManagement.exe` nếu dùng bộ sinh Visual Studio, hoặc `build/OrderManagement.exe` nếu dùng Ninja. Với GCC cũng có thể biên dịch trực tiếp:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp order.cpp hashtable.cpp queue.cpp crud.cpp OrderBST.cpp OrderStatusHandler.cpp statistics.cpp -o OrderManagement
```

## Chức năng

| Mục | Thao tác |
| --- | --- |
| 1 | Tạo đơn và thêm vào cuối hàng đợi chờ duyệt |
| 2 | Xem danh sách, tìm mã bằng Binary Search hoặc xóa đơn |
| 3 | Duyệt đơn đầu hàng, hủy đơn chờ theo mã hoặc hoàn thành đơn đã duyệt |
| 4 | Tìm theo ngày hoặc khoảng ngày bằng BST và lọc theo trạng thái |
| 5 | Thống kê số đơn, tổng tiền, số lượng theo cả bốn trạng thái và Top 3 theo giá trị |
| 6 | Sắp xếp theo ngày bằng Merge Sort, cùng ngày thì xếp mã tăng dần |
| 7 | Xem hàng đợi chờ duyệt theo thứ tự FIFO |
| 9 | Chạy các bộ kiểm thử có sẵn |

Dữ liệu được quản lý trong bộ nhớ của phiên chạy. `HashTable` sở hữu các đơn. `OrderQueue`, cây tìm ngày và heap giữ con trỏ tham chiếu. Khi duyệt hoặc hủy, đơn phải được gỡ khỏi hàng đợi rồi mới đổi trạng thái. Khi xóa, chương trình gỡ khỏi hàng đợi trước khi giải phóng đơn. Cây tìm ngày được tạo từ các đơn hiện tại cho từng truy vấn, nên không giữ địa chỉ của đơn đã xóa giữa hai lần tìm.

## Cấu trúc và độ phức tạp

| Phần theo báo cáo của nhóm | Cấu trúc hoặc thuật toán | Độ phức tạp chính |
| --- | --- | --- |
| Khoa, `hashtable.*` và `main.cpp` | Bảng băm Separate Chaining và menu tích hợp | Tra cứu kỳ vọng O(1) với mã ngắn và tải thấp, xấu nhất O(n) khi va chạm |
| Phương, `order.*` | Mô hình đơn, kiểm tra đầu vào và Merge Sort | Sắp xếp O(n log n), bộ nhớ phụ O(n) |
| Đăng, `queue.*` | Hàng đợi bằng danh sách liên kết | Xem hoặc lấy đầu hàng O(1), thêm có kiểm tra trùng và xóa theo mã O(n) |
| Hào, `crud.*` | Tạo, xem, xóa và tìm nhị phân theo mã | Chuẩn bị danh sách bằng Insertion Sort O(n²) xấu nhất, bước Binary Search O(log n) |
| Thoại, `OrderBST.*` và `OrderStatusHandler.*` | BST theo ngày và chuyển trạng thái | Dựng cây O(nh), tìm chính xác O(h + k), tìm khoảng ngày O(h + v + k), với h là chiều cao, v là số nút ngày khớp và k là số đơn trả về |
| Kỳ, `statistics.*` | Đếm trạng thái, tổng tiền, lọc và Max Heap | Thống kê O(n), dựng heap bằng từng lần chèn O(n log n), lấy từng phần tử lớn nhất O(log n) |

## Kiểm thử và demo

- Chọn **9** để kiểm thử bảng băm, CRUD, thống kê, hàng đợi, chuyển trạng thái và BST. Mỗi ca in `[PASS]` hoặc `[FAIL]`.
- Tạo ba đơn với hai đơn cùng ngày. Duyệt đơn đầu hàng, hủy một đơn chờ theo mã rồi hoàn thành đơn đã duyệt. Dùng mục **7** để kiểm tra hàng đợi chỉ còn đơn đang chờ.
- Tìm theo ngày, tìm theo khoảng ngày, lọc theo trạng thái và so sánh tổng tiền với ba giá trị đã nhập. Xóa một đơn đang chờ, tìm lại mã đó và xem hàng đợi.
- Thử mã trùng, mã có khoảng trắng đầu hoặc cuối, ngày `2026-02-29`, số tiền âm và chuỗi tiền như `100abc`. Chương trình phải báo lỗi và giữ dữ liệu hợp lệ trong phiên.

Codex hỗ trợ đối chiếu đề bài, sửa lỗi tích hợp và chạy kiểm thử. Khi chuẩn bị báo cáo và vấn đáp, nhóm ghi lại kết quả chạy thực tế và giải thích phần việc của từng người.
