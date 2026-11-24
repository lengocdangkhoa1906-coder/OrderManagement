#include "queue.h"
#include <iostream>

OrderQueue::OrderQueue()
    : frontNode(nullptr), rearNode(nullptr), orderCount(0) {}

OrderQueue::~OrderQueue() {
    clear();
}

void OrderQueue::clear() {
    QueueNode* current = frontNode;
    while (current != nullptr) {
        QueueNode* old = current;
        current = current->next;
        delete old;           // Chi delete node cua Queue, khong delete Order*
    }
    frontNode = nullptr;
    rearNode = nullptr;
    orderCount = 0;
}

bool OrderQueue::isEmpty() const {
    return frontNode == nullptr;
}

size_t OrderQueue::size() const {
    return orderCount;
}

bool OrderQueue::contains(const string& id) const {
    QueueNode* current = frontNode;
    while (current != nullptr) {
        if (current->data != nullptr && current->data->getId() == id) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Them vao cuoi (rear) - O(1) nho co con tro rearNode
bool OrderQueue::enqueue(Order* order) {
    if (order == nullptr) {
        return false;
    }
    if (order->getStatus() != OrderStatus::PENDING) {
        return false;         // Queue chi chua don Cho phe duyet
    }
    if (contains(order->getId())) {
        return false;         // Tranh mot don nam hai lan trong Queue
    }

    QueueNode* newNode = new QueueNode(order);
    if (rearNode == nullptr) {
        frontNode = newNode;  // Queue dang rong: node moi la ca dau lan cuoi
        rearNode = newNode;
    }
    else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
    ++orderCount;
    return true;
}

// Lay o dau (front) - O(1)
Order* OrderQueue::dequeue() {
    if (frontNode == nullptr) {
        return nullptr;
    }

    QueueNode* old = frontNode;
    Order* result = old->data;
    frontNode = frontNode->next;
    if (frontNode == nullptr) {
        rearNode = nullptr;   // Vua lay node cuoi cung
    }
    delete old;               // Khong delete Order*
    --orderCount;
    return result;
}

Order* OrderQueue::peek() const {
    if (frontNode == nullptr) {
        return nullptr;
    }
    return frontNode->data;
}

// Xoa theo ma don - O(n): duyet toi da n node, noi lai lien ket,
// giu nguyen thu tu FIFO cua cac node con lai.
bool OrderQueue::removeById(const string& id) {
    QueueNode* current = frontNode;
    QueueNode* previous = nullptr;

    while (current != nullptr) {
        if (current->data != nullptr && current->data->getId() == id) {
            if (previous == nullptr) {
                frontNode = current->next;          // Xoa node dau
            }
            else {
                previous->next = current->next;     // Xoa node giua / cuoi
            }
            if (current == rearNode) {
                rearNode = previous;                // Xoa node cuoi
                // Neu previous == nullptr thi Queue chi co 1 node,
                // rearNode = nullptr la dung (Queue rong).
            }
            delete current;                         // Khong delete Order*
            --orderCount;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;             // Queue rong hoac khong tim thay ma don
}

void OrderQueue::display() const {
    if (frontNode == nullptr) {
        cout << "Queue rong: khong co don nao dang cho phe duyet.\n";
        return;
    }
    QueueNode* current = frontNode;
    while (current != nullptr) {
        if (current->data != nullptr) {
            current->data->display();
        }
        current = current->next;
    }
}
