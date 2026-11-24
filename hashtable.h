#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "order.h"
#include <cstddef>
#include <string>
#include <vector>
using namespace std;

// HashTable la noi so huu va quan ly du lieu Order chinh.
// Queue, BST va Heap chi giu Order* de tham chieu (khong duoc delete Order*).
class HashTable {
private:
    struct HashNode {
        Order data;
        HashNode* next;

        HashNode(const Order& order, HashNode* nextNode)
            : data(order), next(nextNode) {}
    };

    vector<HashNode*> buckets;
    size_t orderCount;
    size_t hashFunction(const string& id) const;

public:
    explicit HashTable(size_t tableSize = 101);
    ~HashTable();

    // Tranh copy HashTable vi cac bucket chua con tro duoc quan ly thu cong.
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    bool insert(const Order& order);        // false neu trung ma hoac ma rong
    Order* find(const string& id);     // nullptr neu khong tim thay
    bool erase(const string& id);      // CHI goi sau khi go khoi Queue
    vector<Order*> allOrders();        // Order* khong thuoc so huu cua vector
    size_t size() const;
};

#endif
