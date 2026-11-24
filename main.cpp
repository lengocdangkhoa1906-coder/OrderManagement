#include "hashtable.h"
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

void printResult(const char* testName, bool pass) {
    cout << (pass ? "[PASS] " : "[FAIL] ") << testName << "\n";
}

void testHashTable() {
    HashTable hash(1); // 1 bucket de co tinh tao va cham, thu Separate Chaining.

    Order a("DH0001", "An", "2026-11-18", 100000, OrderStatus::PENDING);
    Order b("DH0002", "Binh", "2026-11-18", 200000, OrderStatus::PENDING);
    Order c("DH0003", "Chi", "2026-11-18", 300000, OrderStatus::PENDING);

    printResult("Them 3 don", hash.insert(a) && hash.insert(b) && hash.insert(c));
    printResult("Dem so don = 3", hash.size() == 3);
    printResult("Chan ma trung", !hash.insert(a));
    printResult("Tim thay don DH0002", hash.find("DH0002") != nullptr);
    printResult("Khong tim thay ma la", hash.find("DH9999") == nullptr);

    Order* found = hash.find("DH0001");
    if (found != nullptr) {
        found->setStatus(OrderStatus::CANCELLED);
    }
    printResult("Sua Order qua con tro la sua du lieu goc",
                found != nullptr && hash.find("DH0001")->getStatus() == OrderStatus::CANCELLED);

    printResult("Xoa node giua trong bucket va cham", hash.erase("DH0002"));
    printResult("Sau xoa, 2 don con lai van tim duoc",
                hash.find("DH0001") != nullptr && hash.find("DH0003") != nullptr);
    printResult("Danh sach tra ve du 2 don", hash.allOrders().size() == 2);
    printResult("Xoa ma khong ton tai", !hash.erase("DH9999"));

    cout << "\nDanh sach don con lai:\n";
    vector<Order*> orders = hash.allOrders();
    for (size_t i = 0; i < orders.size(); ++i) {
        orders[i]->display();
    }
}

void showMenu() {
    cout << "\n========== QUAN LY DON DAT HANG ==========\n";
    cout << "1. Tao don (Hao - chua ghep)\n";
    cout << "2. Xoa / xem don (Hao - chua ghep)\n";
    cout << "3. Phe duyet, huy, hoan thanh (Thoai + Dang - chua ghep)\n";
    cout << "4. Tim theo ngay / loc trang thai (Thoai + Ky - chua ghep)\n";
    cout << "5. Thong ke / Top 3 (Ky - chua ghep)\n";
    cout << "6. Sap xep theo ngay (Phuong - chua ghep)\n";
    cout << "9. CHAY TEST HASH TABLE CUA KHOA\n";
    cout << "0. Thoat\n";
    cout << "Chon: ";
}

int main() {
    int choice;
    while (true) {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Vui long nhap mot so.\n";
            continue;
        }
        if (choice == 0) {
            cout << "Tam biet!\n";
            break;
        }
        if (choice == 9) {
            testHashTable();
        } else if (choice >= 1 && choice <= 6) {
            cout << "Chuc nang nay se duoc ghep tu module cua thanh vien phu trach.\n";
        } else {
            cout << "Lua chon khong hop le.\n";
        }
    }
    return 0;
}
