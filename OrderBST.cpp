#include "OrderBST.h"
#include <iostream>
#include <iomanip>

using namespace std;

BSTDonHang::Node::Node(Order* don) {
    ngay = don->getCreatedAt();
    danhSachDon.push_back(don);
    left = nullptr;
    right = nullptr;
}

BSTDonHang::BSTDonHang() {
    root = nullptr;
}

BSTDonHang::~BSTDonHang() {
    clear(root);
}

BSTDonHang::Node* BSTDonHang::insert(Node* node, Order* don) {
    if (node == nullptr) {
        return new Node(don);
    }

    const string ngayTao = don->getCreatedAt();
    if (ngayTao < node->ngay) {
        node->left = insert(node->left, don);
    } else if (ngayTao > node->ngay) {
        node->right = insert(node->right, don);
    } else {
        // Nhieu don co cung ngay tao se duoc luu chung trong vector.
        node->danhSachDon.push_back(don);
    }

    return node;
}

void BSTDonHang::insert(Order* don) {
    if (don == nullptr) {
        cout << "Khong the them don hang rong vao BST.\n";
        return;
    }

    // Ngay phai dung dinh dang YYYY-MM-DD de so sanh chuoi dung.
    root = insert(root, don);
}

vector<Order*> BSTDonHang::search(const string& ngay) const {
    if (!Order::isValidDate(ngay)) {
        return {};
    }

    Node* current = root;

    while (current != nullptr) {
        if (ngay == current->ngay) {
            return current->danhSachDon;
        }

        if (ngay < current->ngay) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return {};
}

void BSTDonHang::inorder(Node* node) const {
    if (node == nullptr) return;

    inorder(node->left);

    for (Order* don : node->danhSachDon) {
        don->display();
    }

    inorder(node->right);
}

void BSTDonHang::inorder() const {
    cout << "\nDanh sach don hang theo ngay tang dan:\n";
    cout << left << setw(12) << "Ma don"
         << setw(24) << "Khach hang"
         << setw(15) << "Ngay tao"
         << setw(18) << "Tong tien"
         << "Trang thai\n";
    inorder(root);
}

void BSTDonHang::searchRange(
    Node* node, const string& tuNgay, const string& denNgay,
    vector<Order*>& ketQua) const {

    if (node == nullptr) return;

    if (node->ngay > tuNgay) {
        searchRange(node->left, tuNgay, denNgay, ketQua);
    }

    if (node->ngay >= tuNgay && node->ngay <= denNgay) {
        for (Order* don : node->danhSachDon) {
            ketQua.push_back(don);
        }
    }

    if (node->ngay < denNgay) {
        searchRange(node->right, tuNgay, denNgay, ketQua);
    }
}

vector<Order*> BSTDonHang::searchByDateRange(
    const string& tuNgay, const string& denNgay) const {

    vector<Order*> ketQua;

    if (!Order::isValidDate(tuNgay) || !Order::isValidDate(denNgay) || tuNgay > denNgay) {
        return ketQua;
    }

    searchRange(root, tuNgay, denNgay, ketQua);
    return ketQua;
}

void BSTDonHang::clear(Node* node) {
    if (node == nullptr) return;

    clear(node->left);
    clear(node->right);

    // Chi xoa node BST, khong xoa Order* vi Order do module quan ly du lieu goc.
    delete node;
}
