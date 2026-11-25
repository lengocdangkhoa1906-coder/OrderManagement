#ifndef BSTDONHANG_H
#define BSTDONHANG_H

#include <string>
#include <vector>
#include "order.h"

class BSTDonHang {
private:
    struct Node {
        std::string ngay;
        std::vector<Order*> danhSachDon;
        Node* left;
        Node* right;

        explicit Node(Order* don);
    };

    Node* root;

    Node* insert(Node* node, Order* don);
    void inorder(Node* node) const;
    void searchRange(Node* node, const std::string& tuNgay,
                     const std::string& denNgay,
                     std::vector<Order*>& ketQua) const;
    void clear(Node* node);

public:
    BSTDonHang();
    ~BSTDonHang();

    // Khong cho copy BST de tranh hai cay cung giai phong mot node.
    BSTDonHang(const BSTDonHang&) = delete;
    BSTDonHang& operator=(const BSTDonHang&) = delete;

    // Cac Order phai con ton tai va giu nguyen ngay tao trong suot vong doi cay.
    void insert(Order* don);
    std::vector<Order*> search(const std::string& ngay) const;
    void inorder() const;
    std::vector<Order*> searchByDateRange(
        const std::string& tuNgay, const std::string& denNgay) const;
};

#endif
