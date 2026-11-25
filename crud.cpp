#include "crud.h"
#include <iostream>
#include <limits>

using namespace std;

// Kiem tra nam nhuan
bool isLeapYear(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

// Kiem tra ngay hop le theo dinh dang YYYY-MM-DD
bool isValidDate(const string& date) {
    if (date.length() != 10 || date[4] != '-' || date[7] != '-') {
        return false;
    }
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (date[i] < '0' || date[i] > '9') return false;
    }

    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;

    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (isLeapYear(year)) {
        daysInMonth[2] = 29;
    }

    return day >= 1 && day <= daysInMonth[month];
}

// Sap xep danh sach con tro Order* tang dan theo ma don (Insertion Sort - Chuong 3)
void sortOrdersById(vector<Order*>& list) {
    int n = (int)list.size();
    for (int i = 1; i < n; i++) {
        Order* key = list[i];
        int j = i - 1;
        while (j >= 0 && list[j]->getId() > key->getId()) {
            list[j + 1] = list[j];
            j--;
        }
        list[j + 1] = key;
    }
}

// Tao don hang moi va dong bo sang HashTable + OrderQueue
bool createOrder(HashTable& hash, OrderQueue& queue,
    const string& id, const string& customerName,
    const string& createdAt, long long totalAmount) {
    if (id.empty() || customerName.empty()) {
        cout << "[Loi] Ma don va ten khach hang khong duoc de trong.\n";
        return false;
    }
    if (hash.find(id) != nullptr) {
        cout << "[Loi] Ma don '" << id << "' da ton tai.\n";
        return false;
    }
    if (!isValidDate(createdAt)) {
        cout << "[Loi] Ngay tao '" << createdAt << "' khong hop le (chuan YYYY-MM-DD).\n";
        return false;
    }
    if (totalAmount < 0) {
        cout << "[Loi] Tong tien phai >= 0.\n";
        return false;
    }

    Order newOrder(id, customerName, createdAt, totalAmount, OrderStatus::PENDING);
    if (!hash.insert(newOrder)) {
        cout << "[Loi] Khong the luu don hang vao he thong.\n";
        return false;
    }

    // Lay dia chi Order vua luu trong HashTable de day vao Queue cho duyet
    Order* storedPtr = hash.find(id);
    if (storedPtr != nullptr) {
        queue.enqueue(storedPtr);
    }
    return true;
}

void inputAndCreateOrder(HashTable& hash, OrderQueue& queue) {
    string id, customerName, createdAt;
    long long totalAmount;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Nhap ma don (VD: DH0001): ";
    getline(cin, id);
    cout << "Nhap ten khach hang: ";
    getline(cin, customerName);
    cout << "Nhap ngay tao (YYYY-MM-DD): ";
    getline(cin, createdAt);
    cout << "Nhap tong tien (VND): ";
    if (!(cin >> totalAmount)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[Loi] So tien nhap vao khong hop le.\n";
        return;
    }

    if (createOrder(hash, queue, id, customerName, createdAt, totalAmount)) {
        cout << "-> Tao don hang " << id << " thanh cong!\n";
    }
}

// Xoa don hang theo ma: Go khoi Queue truoc, xoa khoi HashTable sau
bool deleteOrder(HashTable& hash, OrderQueue& queue, const string& id) {
    if (hash.find(id) == nullptr) {
        cout << "[Loi] Khong tim thay don hang ma '" << id << "' de xoa.\n";
        return false;
    }

    queue.removeById(id);
    return hash.erase(id);
}

// Tim kiem nhi phan theo ma don (Binary Search - Chuong 2)
Order* binarySearchById(HashTable& hash, const string& id) {
    vector<Order*> list = hash.allOrders();
    if (list.empty()) return nullptr;

    // Buoc bat buoc: Sap xep tang dan theo ma don truoc khi tim nhi phan
    sortOrdersById(list);

    int left = 0;
    int right = (int)list.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        string midId = list[mid]->getId();

        if (midId == id) {
            return list[mid];
        }
        else if (id < midId) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }
    return nullptr;
}

void viewOrder(HashTable& hash, const string& id) {
    Order* found = binarySearchById(hash, id);
    if (found == nullptr) {
        cout << "Khong tim thay don hang co ma: " << id << "\n";
    }
    else {
        cout << "--- CHI TIET DON HANG ---\n";
        found->display();
    }
}

void listOrders(HashTable& hash) {
    vector<Order*> list = hash.allOrders();
    if (list.empty()) {
        cout << "Danh sach don hang hien dang trong.\n";
        return;
    }

    sortOrdersById(list);
    cout << "--- DANH SACH TAT CA DON HANG (" << list.size() << " don) ---\n";
    for (int i = 0; i < (int)list.size(); i++) {
        list[i]->display();
    }
}

void printTest(const char* name, bool pass) {
    cout << (pass ? "[PASS] " : "[FAIL] ") << name << "\n";
}

void testCRUD() {
    cout << "\n===== CHAY KIEM THU MODULE CRUD + BINARY SEARCH (HAO) =====\n";
    HashTable hash;
    OrderQueue queue;

    printTest("1. Binary Search tren danh sach rong tra ve nullptr", binarySearchById(hash, "DH0001") == nullptr);

    bool t2 = createOrder(hash, queue, "DH0003", "Le Van C", "2024-02-29", 300000) &&
        createOrder(hash, queue, "DH0001", "Nguyen Van A", "2026-11-18", 150000) &&
        createOrder(hash, queue, "DH0002", "Tran Thi B", "2026-11-19", 250000);
    printTest("2. Tao 3 don hop le (co ngay nam nhuan 2024-02-29) va dong bo Queue", t2 && hash.size() == 3 && queue.size() == 3);

    bool t3 = !createOrder(hash, queue, "DH0001", "Trung Ma", "2026-11-20", 100000) &&
        !createOrder(hash, queue, "DH0004", "Sai Nhuan", "2026-02-29", 100000) &&
        !createOrder(hash, queue, "DH0005", "Sai Ngay", "2026-13-40", 100000) &&
        !createOrder(hash, queue, "DH0006", "Tien Am", "2026-11-20", -5000);
    printTest("3. Chan ma trung, ngay sai, nam khong nhuan va tien am", t3 && hash.size() == 3);

    bool t4 = (binarySearchById(hash, "DH0001") != nullptr) &&
        (binarySearchById(hash, "DH0002") != nullptr) &&
        (binarySearchById(hash, "DH0003") != nullptr) &&
        (binarySearchById(hash, "DH9999") == nullptr);
    printTest("4. Binary Search tim dung vi tri dau, giua, cuoi va ma khong ton tai", t4);

    bool t5 = deleteOrder(hash, queue, "DH0002") &&
        hash.size() == 2 && queue.size() == 2 &&
        binarySearchById(hash, "DH0002") == nullptr;
    printTest("5. Xoa don DH0002 dong bo ca HashTable va OrderQueue", t5);

    printTest("6. Chan xoa ma khong ton tai", !deleteOrder(hash, queue, "DH9999"));
}