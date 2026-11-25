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
        delete old;
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
bool OrderQueue::enqueue(Order* order) {
    if (order == nullptr) {
        return false;
    }
    if (order->getStatus() != OrderStatus::PENDING) {
        return false;
    }
    if (contains(order->getId())) {
        return false;
    }
    QueueNode* newNode = new QueueNode(order);
    if (rearNode == nullptr) {
        frontNode = newNode;
        rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
    ++orderCount;
    return true;
}
Order* OrderQueue::dequeue() {
    if (frontNode == nullptr) {
        return nullptr;
    }
    QueueNode* old = frontNode;
    Order* result = old->data;
    frontNode = frontNode->next;
    if (frontNode == nullptr) {
        rearNode = nullptr;
    }
    delete old;
    --orderCount;
    return result;
}
Order* OrderQueue::peek() const {
    if (frontNode == nullptr) {
        return nullptr;
    }
    return frontNode->data;
}
bool OrderQueue::removeById(const string& id) {
    QueueNode* current = frontNode;
    QueueNode* previous = nullptr;
    while (current != nullptr) {
        if (current->data != nullptr && current->data->getId() == id) {
            if (previous == nullptr) {
                frontNode = current->next;
            } else {
                previous->next = current->next;
            }
            if (current == rearNode) {
                rearNode = previous;
            }
            delete current;
            --orderCount;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
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
