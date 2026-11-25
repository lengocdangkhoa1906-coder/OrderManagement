#include "OrderBST.h"
#include "OrderStatusHandler.h"
#include "crud.h"
#include "hashtable.h"
#include "statistics.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

namespace {
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
    printResult("Chan ma rong", !hash.insert(Order{}));
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

void testOrderIntegration() {
    HashTable hash;
    OrderQueue queue;
    const string day = "2026-11-18";
    const bool created = createOrder(hash, queue, "DH1", "An", day, 100) &&
                         createOrder(hash, queue, "DH2", "Binh", day, 200) &&
                         createOrder(hash, queue, "DH3", "Chi", "2026-11-19", 300);
    printResult("Tao don va hang doi FIFO dung thu tu", created && queue.peek() == hash.find("DH1"));
    if (!created) return;

    OrderStatusHandler handler;
    const auto removeFromQueue = [&queue](const string& id) { return queue.removeById(id); };
    Order* first = hash.find("DH1");
    Order* second = hash.find("DH2");
    printResult("Chan hoan thanh don chua duyet", !handler.hoanThanh(first));
    printResult("Duyet dau hang va cap nhat don trong HashTable",
                handler.pheDuyet(queue.peek(), removeFromQueue) &&
                first->getStatus() == OrderStatus::APPROVED && queue.peek() == second);
    printResult("Huy don dang cho va go khoi hang doi",
                handler.huyDon(second, removeFromQueue) &&
                second->getStatus() == OrderStatus::CANCELLED && !queue.contains("DH2"));
    printResult("Hoan thanh don da duyet", handler.hoanThanh(first) &&
                first->getStatus() == OrderStatus::COMPLETED);
    printResult("Xoa don dang cho o ca HashTable va Queue",
                deleteOrder(hash, queue, "DH3") && hash.find("DH3") == nullptr && queue.isEmpty());

    BSTDonHang tree;
    for (Order* order : hash.allOrders()) tree.insert(order);
    printResult("BST tim du don cung ngay va bo qua don da xoa",
                tree.search(day).size() == 2 && tree.search("2026-11-19").empty());
    printResult("BST tim khoang ngay gom hai dau va chan ngay sai",
                tree.searchByDateRange(day, day).size() == 2 && tree.search("2026-02-29").empty());
}

string readText(const char* prompt) {
    cout << prompt;
    string text;
    getline(cin, text);
    return text;
}

int readChoice(const char* prompt) {
    istringstream input(readText(prompt));
    int choice;
    if (!(input >> choice) || !(input >> ws).eof()) return -1;
    return choice;
}

void manageOrders(HashTable& hash, OrderQueue& queue) {
    switch (readChoice("1. Xem tat ca | 2. Tim theo ma | 3. Xoa theo ma | 0. Quay lai\nChon: ")) {
        case 0:
            return;
        case 1:
            listOrders(hash);
            return;
        case 2:
            viewOrder(hash, readText("Nhap ma don: "));
            return;
        case 3:
            if (deleteOrder(hash, queue, readText("Nhap ma don can xoa: "))) {
                cout << "Da xoa don hang.\n";
            }
            return;
        default:
            cout << "Lua chon khong hop le.\n";
    }
}

void processOrders(HashTable& hash, OrderQueue& queue) {
    OrderStatusHandler handler;
    const auto removeFromQueue = [&queue](const string& id) { return queue.removeById(id); };
    switch (readChoice("1. Duyet don dau hang | 2. Huy theo ma | 3. Hoan thanh theo ma | 0. Quay lai\nChon: ")) {
        case 0:
            return;
        case 1:
            if (Order* order = queue.peek()) {
                handler.pheDuyet(order, removeFromQueue);
            } else {
                cout << "Khong co don cho phe duyet.\n";
            }
            return;
        case 2:
            handler.huyDon(hash.find(readText("Nhap ma don can huy: ")), removeFromQueue);
            return;
        case 3:
            handler.hoanThanh(hash.find(readText("Nhap ma don can hoan thanh: ")));
            return;
        default:
            cout << "Lua chon khong hop le.\n";
    }
}

void findOrdersByDate(HashTable& hash, bool range) {
    const string from = readText(range ? "Tu ngay (YYYY-MM-DD): " : "Nhap ngay (YYYY-MM-DD): ");
    const string to = range ? readText("Den ngay (YYYY-MM-DD): ") : from;
    if (!Order::isValidDate(from) || !Order::isValidDate(to) || from > to) {
        cout << "Ngay hoac khoang ngay khong hop le.\n";
        return;
    }

    // Chi tao chi muc tu cac don hien co de Queue/BST khong giu don da xoa.
    BSTDonHang tree;
    for (Order* order : hash.allOrders()) tree.insert(order);
    const vector<Order*> matches = range ? tree.searchByDateRange(from, to) : tree.search(from);
    if (matches.empty()) {
        cout << "Khong tim thay don trong ngay da chon.\n";
        return;
    }
    for (const Order* order : matches) order->display();
}

void filterOrders(HashTable& hash) {
    switch (readChoice("1. Theo ngay | 2. Theo khoang ngay | 3. Theo trang thai | 0. Quay lai\nChon: ")) {
        case 0:
            return;
        case 1:
            findOrdersByDate(hash, false);
            return;
        case 2:
            findOrdersByDate(hash, true);
            return;
        case 3: {
            const OrderStatus statuses[] = {OrderStatus::PENDING, OrderStatus::APPROVED,
                                            OrderStatus::CANCELLED, OrderStatus::COMPLETED};
            const int choice = readChoice("1. Cho xu ly | 2. Da duyet | 3. Da huy | 4. Hoan thanh\nChon: ");
            if (choice >= 1 && choice <= 4) {
                showFilterByStatus(hash, statuses[choice - 1]);
                return;
            }
            break;
        }
        default:
            break;
    }
    cout << "Lua chon khong hop le.\n";
}

void sortOrdersByDateMenu(HashTable& hash) {
    const int direction = readChoice("1. Cu nhat truoc | 2. Moi nhat truoc | 0. Quay lai\nChon: ");
    if (direction == 0) return;
    if (direction != 1 && direction != 2) {
        cout << "Lua chon khong hop le.\n";
        return;
    }

    const vector<Order*> stored = hash.allOrders();
    if (stored.empty()) {
        cout << "Chua co don de sap xep.\n";
        return;
    }
    vector<Order> sorted;
    sorted.reserve(stored.size());
    for (const Order* order : stored) sorted.push_back(*order);
    mergeSortOrdersByDate(sorted, direction == 1);
    for (const Order& order : sorted) order.display();
}

void showMenu() {
    cout << "\n========== QUAN LY DON DAT HANG ==========\n";
    cout << "1. Tao don\n";
    cout << "2. Xem, tim theo ma hoac xoa don\n";
    cout << "3. Phe duyet, huy, hoan thanh\n";
    cout << "4. Tim theo ngay / loc trang thai\n";
    cout << "5. Thong ke / Top 3\n";
    cout << "6. Sap xep theo ngay\n";
    cout << "7. Xem hang doi cho phe duyet\n";
    cout << "9. Chay kiem thu\n";
    cout << "0. Thoat\n";
}
} // namespace

int main() {
    HashTable hash;
    OrderQueue queue;
    while (true) {
        showMenu();
        const int choice = readChoice("Chon: ");
        if (!cin || choice == 0) break;
        switch (choice) {
            case 1: inputAndCreateOrder(hash, queue); break;
            case 2: manageOrders(hash, queue); break;
            case 3: processOrders(hash, queue); break;
            case 4: filterOrders(hash); break;
            case 5: showStatistics(hash); break;
            case 6: sortOrdersByDateMenu(hash); break;
            case 7: queue.display(); break;
            case 9:
                testHashTable();
                testCRUD();
                testStatistics();
                testOrderIntegration();
                break;
            default: cout << "Lua chon khong hop le.\n";
        }
    }
    cout << "Tam biet!\n";
    return 0;
}
