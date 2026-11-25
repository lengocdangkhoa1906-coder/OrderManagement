#include "statistics.h"
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

using namespace std;

// Danh sach TAT CA trang thai cua OrderStatus (de thong ke du ca trang thai co 0 don).
// Neu order.h cua ban co them/bot trang thai thi sua o day va trong statusName().
static const OrderStatus ALL_STATUS[] = {
    OrderStatus::PENDING,
    OrderStatus::APPROVED,
    OrderStatus::COMPLETED,
    OrderStatus::CANCELLED};

string statusName(OrderStatus status)
{
    switch (status)
    {
    case OrderStatus::PENDING:
        return "PENDING";
    case OrderStatus::APPROVED:
        return "APPROVED";
    case OrderStatus::COMPLETED:
        return "COMPLETED";
    case OrderStatus::CANCELLED:
        return "CANCELLED";
    }
    return "UNKNOWN";
}

// 1. Dem so don theo trang thai - O(n)
map<OrderStatus, int> countByStatus(HashTable &hash)
{
    map<OrderStatus, int> result;
    for (OrderStatus s : ALL_STATUS)
    {
        result[s] = 0; // dam bao trang thai 0 don van xuat hien
    }
    vector<Order *> list = hash.allOrders(); // O(n)
    for (size_t i = 0; i < list.size(); ++i)
    {
        result[list[i]->getStatus()]++; // map co ~4 key => O(1)
    }
    return result;
}

// 2. Tong tien tat ca don - O(n)
long long sumAllAmounts(HashTable &hash)
{
    long long total = 0;
    vector<Order *> list = hash.allOrders();
    for (size_t i = 0; i < list.size(); ++i)
    {
        const long long amount = list[i]->getTotalAmount();
        if (amount > numeric_limits<long long>::max() - total) {
            throw overflow_error("Tong gia tri vuot gioi han so nguyen 64 bit.");
        }
        total += amount;
    }
    return total;
}

// 3. Loc theo trang thai - O(n)
vector<Order *> filterByStatus(HashTable &hash, OrderStatus status)
{
    vector<Order *> result;
    vector<Order *> list = hash.allOrders();
    for (size_t i = 0; i < list.size(); ++i)
    {
        if (list[i]->getStatus() == status)
        {
            result.push_back(list[i]);
        }
    }
    return result;
}

namespace {
class OrderMaxHeap {
public:
    void insert(Order* order) {
        orders.push_back(order);
        size_t index = orders.size() - 1;
        while (index > 0) {
            const size_t parent = (index - 1) / 2;
            if (!higherPriority(orders[index], orders[parent])) break;
            swap(orders[index], orders[parent]);
            index = parent;
        }
    }

    Order* extractMax() {
        if (orders.empty()) return nullptr;
        Order* largest = orders.front();
        orders.front() = orders.back();
        orders.pop_back();
        heapify(0);
        return largest;
    }

private:
    vector<Order*> orders;

    static bool higherPriority(const Order* left, const Order* right) {
        if (left->getTotalAmount() != right->getTotalAmount()) {
            return left->getTotalAmount() > right->getTotalAmount();
        }
        return left->getId() < right->getId();
    }

    void heapify(size_t index) {
        while (index < orders.size() / 2) {
            size_t child = 2 * index + 1;
            if (child + 1 < orders.size() && higherPriority(orders[child + 1], orders[child])) {
                ++child;
            }
            if (!higherPriority(orders[child], orders[index])) break;
            swap(orders[child], orders[index]);
            index = child;
        }
    }
};
} // namespace

vector<Order *> top3Orders(HashTable &hash)
{
    OrderMaxHeap heap;
    for (Order* order : hash.allOrders()) {
        heap.insert(order);
    }

    vector<Order*> result;
    while (result.size() < 3) {
        Order* order = heap.extractMax();
        if (order == nullptr) break;
        result.push_back(order);
    }
    return result;
}

void showFilterByStatus(HashTable &hash, OrderStatus status)
{
    vector<Order *> list = filterByStatus(hash, status);
    if (list.empty())
    {
        cout << "Khong co don nao o trang thai " << statusName(status) << ".\n";
        return;
    }
    cout << "--- DON HANG TRANG THAI " << statusName(status)
         << " (" << list.size() << " don) ---\n";
    for (size_t i = 0; i < list.size(); ++i)
    {
        list[i]->display();
    }
}

void showStatistics(HashTable &hash)
{
    cout << "\n========== THONG KE DON HANG ==========\n";
    cout << "Tong so don: " << hash.size() << "\n";

    cout << "\n-- So don theo trang thai --\n";
    map<OrderStatus, int> counts = countByStatus(hash);
    for (map<OrderStatus, int>::const_iterator it = counts.begin(); it != counts.end(); ++it)
    {
        cout << "  " << statusName(it->first) << ": " << it->second << "\n";
    }

    try {
        const long long total = sumAllAmounts(hash);
        cout << "\nTong gia tri tat ca don: " << total << " VND\n";
    } catch (const overflow_error& error) {
        cout << '\n' << error.what() << '\n';
    }

    cout << "\n-- TOP 3 don co gia tri lon nhat --\n";
    vector<Order *> top = top3Orders(hash);
    for (size_t i = 0; i < top.size(); ++i)
    {
        cout << "Hang " << (i + 1) << ":\n";
        top[i]->display();
    }
}

// ================= TEST =================
static void printStatTest(const char *name, bool pass)
{
    cout << (pass ? "[PASS] " : "[FAIL] ") << name << "\n";
}

void testStatistics()
{
    cout << "\n===== CHAY KIEM THU MODULE THONG KE (KY) =====\n";
    HashTable hash;

    printStatTest("1. Du lieu rong: tong = 0, loc rong, top3 rong",
                  sumAllAmounts(hash) == 0 && filterByStatus(hash, OrderStatus::PENDING).empty() && top3Orders(hash).empty());

    hash.insert(Order("DH0001", "An", "2026-11-01", 150000, OrderStatus::PENDING));
    hash.insert(Order("DH0002", "Binh", "2026-11-02", 900000, OrderStatus::PENDING));
    hash.insert(Order("DH0003", "Chi", "2026-11-03", 400000, OrderStatus::PENDING));
    hash.insert(Order("DH0004", "Dung", "2026-11-04", 700000, OrderStatus::PENDING));
    hash.insert(Order("DH0005", "Em", "2026-11-05", 50000, OrderStatus::PENDING));
    hash.find("DH0002")->setStatus(OrderStatus::APPROVED);
    hash.find("DH0003")->setStatus(OrderStatus::COMPLETED);
    hash.find("DH0004")->setStatus(OrderStatus::COMPLETED);
    hash.find("DH0005")->setStatus(OrderStatus::CANCELLED);

    map<OrderStatus, int> c = countByStatus(hash);
    printStatTest("2. countByStatus dem dung (1 PENDING, 1 APPROVED, 2 COMPLETED, 1 CANCELLED)",
                  c[OrderStatus::PENDING] == 1 && c[OrderStatus::APPROVED] == 1 &&
                      c[OrderStatus::COMPLETED] == 2 && c[OrderStatus::CANCELLED] == 1);

    printStatTest("3. sumAllAmounts = 2.200.000", sumAllAmounts(hash) == 2200000);

    vector<Order *> f = filterByStatus(hash, OrderStatus::COMPLETED);
    printStatTest("4. filterByStatus(COMPLETED) tra ve 2 don",
                  f.size() == 2 && f[0]->getStatus() == OrderStatus::COMPLETED && f[1]->getStatus() == OrderStatus::COMPLETED);

    vector<Order *> t = top3Orders(hash);
    printStatTest("5. top3Orders dung thu tu giam dan: DH0002, DH0004, DH0003",
                  t.size() == 3 && t[0]->getId() == "DH0002" &&
                      t[1]->getId() == "DH0004" && t[2]->getId() == "DH0003");

    hash.erase("DH0001");
    hash.erase("DH0002");
    hash.erase("DH0003");
    vector<Order *> t2 = top3Orders(hash);
    printStatTest("6. top3Orders khi chi con 2 don thi tra ve 2 don (khong loi)",
                  t2.size() == 2 && t2[0]->getId() == "DH0004" && t2[1]->getId() == "DH0005");

    printStatTest("7. Du lieu khong bi thay doi sau khi thong ke", hash.size() == 2);
}
