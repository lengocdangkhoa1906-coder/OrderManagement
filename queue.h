#ifndef QUEUE_H
#define QUEUE_H
#include "order.h"
#include <cstddef>
#include <string>
using namespace std;
class OrderQueue {
private:
    struct QueueNode {
        Order* data;
        QueueNode* next;
        explicit QueueNode(Order* order, QueueNode* nextNode = nullptr)
            : data(order), next(nextNode) {}
    };
    QueueNode* frontNode;
    QueueNode* rearNode;
    size_t orderCount;
public:
    OrderQueue();
    ~OrderQueue();
    OrderQueue(const OrderQueue&) = delete;
    OrderQueue& operator=(const OrderQueue&) = delete;
    bool enqueue(Order* order);
    Order* dequeue();
    Order* peek() const;
    bool isEmpty() const;
    size_t size() const;
    bool removeById(const string& id);
    bool contains(const string& id) const;
    void display() const;
    void clear();
};
#endif
