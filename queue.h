#ifndef QUEUE_H
#define QUEUE_H

#include "order.h"
#include <cstddef>
#include <string>
using namespace std;

// ============================================================
// OrderQueue - Queue cai dat bang Linked List
// Nguoi phu trach: Dang - Queue + Linked List
//
// Nguyen tac:
// - Chi chua cac don dang o trang thai PENDING (Cho phe duyet).
// - Chi giu Order* de tham chieu. Order thuoc so huu cua HashTable
//   (Khoa), Queue TUYET DOI khong delete Order*.
// - FIFO: enqueue() them vao cuoi (rear), dequeue() lay o dau (front).
// ============================================================
class OrderQueue {
private:
    struct QueueNode {
        Order* data;          // Chi tham chieu, khong so huu
        QueueNode* next;

        explicit QueueNode(Order* order, QueueNode* nextNode = nullptr)
            : data(order), next(nextNode) {}
    };

    QueueNode* frontNode;     // Dau hang (lay ra truoc)
    QueueNode* rearNode;      // Cuoi hang (them vao sau)
    size_t orderCount;

public:
    OrderQueue();
    ~OrderQueue();            // Chi delete QueueNode, khong delete Order

    // Tranh copy Queue vi hai Queue cung quan ly mot danh sach node.
    OrderQueue(const OrderQueue&) = delete;
    OrderQueue& operator=(const OrderQueue&) = delete;

    // Them don vao cuoi Queue. Tra ve false neu:
    // - order == nullptr
    // - don khong o trang thai PENDING
    // - ma don da co trong Queue (tranh trung)
    bool enqueue(Order* order);

    // Lay don o dau Queue ra (FIFO) va tra ve Order*.
    // Tra ve nullptr neu Queue rong. Khong delete Order.
    Order* dequeue();

    // Xem don o dau Queue ma khong lay ra. nullptr neu rong.
    Order* peek() const;

    bool isEmpty() const;
    size_t size() const;

    // ---- NANG CAO ----
    // Xoa mot don bat ky khoi Queue theo ma don, giu nguyen thu tu FIFO
    // cua cac don con lai. Xu ly: Queue rong, node dau, node cuoi,
    // node giua, node duy nhat. Tra ve false neu khong tim thay.
    // Ham nay dung lam callback removeFromQueue cho XuLyTrangThai
    // (Thoai): std::function<bool(const std::string&)>
    bool removeById(const string& id);

    // Kiem tra mot ma don co dang nam trong Queue hay khong.
    bool contains(const string& id) const;

    // In danh sach don dang cho phe duyet, theo thu tu FIFO.
    void display() const;

    // Xoa tat ca QueueNode (khong delete Order*).
    void clear();
};

#endif
#pragma once
