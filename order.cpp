#include "order.h"
#include <iomanip>
#include <stdexcept>
#include <cctype>

// --- HÀM KIỂM TRA TÍNH HỢP LỆ CỦA NGÀY (YYYY-MM-DD) ---

bool Order::isValidDate(const std::string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(static_cast<unsigned char>(date[i]))) return false;
    }

    int year = std::stoi(date.substr(0, 4));
    int month = std::stoi(date.substr(5, 2));
    int day = std::stoi(date.substr(8, 2));

    if (year < 1 || month < 1 || month > 12 || day < 1) return false;

    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Kiem tra nam nhuan
    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (isLeapYear) {
        daysInMonth[2] = 29;
    }

    return day <= daysInMonth[month];
}

// --- CÀI ĐẶT CÁC PHƯƠNG THỨC CỦA LỚP ORDER ---

Order::Order()
    : id(""), customerName(""), createdAt("1970-01-01"), totalAmount(0), status(OrderStatus::PENDING) {
}

Order::Order(const std::string& id, const std::string& customerName,
    const std::string& createdAt, long long totalAmount, OrderStatus status) {
    if (id.empty() || std::isspace(static_cast<unsigned char>(id.front())) ||
        std::isspace(static_cast<unsigned char>(id.back()))) {
        throw std::invalid_argument("Ma don phai co noi dung va khong co khoang trang dau/cuoi.");
    }
    if (customerName.find_first_not_of(" \t\n\r\f\v") == std::string::npos) {
        throw std::invalid_argument("Ten khach hang khong duoc de trong.");
    }
    if (!isValidDate(createdAt)) {
        throw std::invalid_argument("Ngay tao don khong hop le (dinh dang chuan YYYY-MM-DD).");
    }
    if (totalAmount < 0) {
        throw std::invalid_argument("Tong tien don hang khong duoc am.");
    }

    this->id = id;
    this->customerName = customerName;
    this->createdAt = createdAt;
    this->totalAmount = totalAmount;
    this->status = status;
}

std::string Order::getId() const {
    return id;
}

std::string Order::getCreatedAt() const {
    return createdAt;
}

long long Order::getTotalAmount() const {
    return totalAmount;
}

OrderStatus Order::getStatus() const {
    return status;
}

std::string Order::getCustomerName() const {
    return customerName;
}

void Order::setStatus(OrderStatus newStatus) {
    this->status = newStatus;
}

void Order::setCustomerName(const std::string& name) {
    if (name.find_first_not_of(" \t\n\r\f\v") == std::string::npos) {
        throw std::invalid_argument("Ten khach hang khong duoc de trong.");
    }
    this->customerName = name;
}

void Order::setTotalAmount(long long amount) {
    if (amount < 0) {
        throw std::invalid_argument("Tong tien don hang khong duoc am.");
    }
    this->totalAmount = amount;
}

void Order::setCreatedAt(const std::string& date) {
    if (!isValidDate(date)) {
        throw std::invalid_argument("Ngay tao don khong hop le (dinh dang chuan YYYY-MM-DD).");
    }
    this->createdAt = date;
}

std::string Order::getStatusString() const {
    switch (status) {
    case OrderStatus::PENDING:   return "Cho xu ly";
    case OrderStatus::APPROVED:  return "Da duyet";
    case OrderStatus::CANCELLED: return "Da huy";
    case OrderStatus::COMPLETED: return "Hoan thanh";
    default:                     return "Khong xac dinh";
    }
}

void Order::display() const {
    std::cout << std::left
        << std::setw(12) << id
        << std::setw(24) << customerName
        << std::setw(15) << createdAt
        << std::setw(18) << totalAmount
        << std::setw(15) << getStatusString() << "\n";
}

// --- CÀI ĐẶT THUẬT TOÁN MERGE SORT THEO NGÀY ---

namespace {
    // Ham tron hai mang con da sap xep
    void merge(std::vector<Order>& arr, int left, int mid, int right, bool ascending) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        std::vector<Order> L(n1);
        std::vector<Order> R(n2);

        for (int i = 0; i < n1; ++i)
            L[i] = arr[left + i];
        for (int j = 0; j < n2; ++j)
            R[j] = arr[mid + 1 + j];

        int i = 0, j = 0, k = left;

        while (i < n1 && j < n2) {
            bool takeLeft = false;

            if (L[i].getCreatedAt() == R[j].getCreatedAt()) {
                // Neu trung ngay thi so sanh ma don tang dan
                takeLeft = (L[i].getId() <= R[j].getId());
            }
            else {
                takeLeft = ascending
                    ? (L[i].getCreatedAt() < R[j].getCreatedAt())
                    : (L[i].getCreatedAt() > R[j].getCreatedAt());
            }

            if (takeLeft) {
                arr[k] = L[i];
                ++i;
            }
            else {
                arr[k] = R[j];
                ++j;
            }
            ++k;
        }

        // Chep cac phan tu con lai (neu co)
        while (i < n1) {
            arr[k] = L[i];
            ++i;
            ++k;
        }
        while (j < n2) {
            arr[k] = R[j];
            ++j;
            ++k;
        }
    }

    // Ham de quy chia doi mang
    void mergeSortHelper(std::vector<Order>& arr, int left, int right, bool ascending) {
        if (left >= right) return;

        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid, ascending);
        mergeSortHelper(arr, mid + 1, right, ascending);
        merge(arr, left, mid, right, ascending);
    }
}

void mergeSortOrdersByDate(std::vector<Order>& orders, bool ascending) {
    if (orders.size() <= 1) return;
    mergeSortHelper(orders, 0, static_cast<int>(orders.size()) - 1, ascending);
}
