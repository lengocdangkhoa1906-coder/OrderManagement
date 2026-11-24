#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include <iostream>

// Dinh nghia cac trang thai cua don hang truc tuyen bang enum class
enum class OrderStatus {
    PENDING,    // Cho xu ly
    APPROVED,   // Da duyet
    CANCELLED,  // Da huy
    COMPLETED   // Hoan thanh
};

class Order {
private:
    std::string id;           // Ma don hang (VD: "ORD001")
    std::string customerName; // Ten khach hang
    std::string createdAt;    // Ngay tao don (Dinh dang chuan YYYY-MM-DD)
    long long totalAmount;    // Tong tien don hang
    OrderStatus status;       // Trang thai don hang

public:
    // Constructors
    Order();
    Order(const std::string& id, const std::string& customerName,
        const std::string& createdAt, long long totalAmount,
        OrderStatus status = OrderStatus::PENDING);

    // Getters & Setters
    std::string getId() const;
    std::string getCreatedAt() const;
    long long getTotalAmount() const;
    OrderStatus getStatus() const;
    std::string getCustomerName() const;

    void setStatus(OrderStatus newStatus);
    void setCustomerName(const std::string& name);
    void setTotalAmount(long long amount);
    void setCreatedAt(const std::string& date);

    // Tien ich kiem tra va hien thi
    static bool isValidDate(const std::string& date);
    std::string getStatusString() const;
    void display() const;
};

// Ham sap xep danh sach don hang theo ngay (createdAt) su dung thuat toan Merge Sort
// ascending = true: Sap xep tang dan (cu nhat -> moi nhat)
// ascending = false: Sap xep giam dan (moi nhat -> cu nhat)
// Neu trung ngay thi so sanh ma don (id) tang dan
void mergeSortOrdersByDate(std::vector<Order>& orders, bool ascending = true);

#endif // ORDER_H