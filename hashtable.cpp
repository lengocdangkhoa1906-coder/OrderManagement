#include "hashtable.h"
using namespace std;

HashTable::HashTable(std::size_t tableSize)
    : buckets(tableSize == 0 ? 101 : tableSize, nullptr), orderCount(0) {}

// Giai phong tat ca cac node va Order trong HashTable khi thoat chuong trinh.
HashTable::~HashTable() {
    for (size_t i = 0; i < buckets.size(); ++i) {
        HashNode* current = buckets[i];
        while (current != nullptr) {
            HashNode* old = current;
            current = current->next;
            delete old;
        }
        buckets[i] = nullptr;
    }
}

// Ham bam don gian: chuyen tung ky tu ID thanh gia tri so.
size_t HashTable::hashFunction(const string& id) const {
    size_t h = 0;
    for (unsigned char ch : id) {
        h = (h * 31 + ch) % buckets.size();
    }
    return h;
}

// Them vao dau danh sach lien ket cua bucket.
bool HashTable::insert(const Order& order) {
    const string id = order.getId();
    if (id.empty() || find(id) != nullptr) {
        return false;
    }

    size_t index = hashFunction(id);
    buckets[index] = new HashNode(order, buckets[index]);
    ++orderCount;
    return true;
}

// Tim kiem trong danh sach lien ket tai bucket da bam.
Order* HashTable::find(const string& id) {
    size_t index = hashFunction(id);
    HashNode* current = buckets[index];

    while (current != nullptr) {
        if (current->data.getId() == id) {
            return &current->data;
        }
        current = current->next;
    }
    return nullptr;
}

// Xoa mot node trong danh sach lien ket; cac node khac van giu nguyen.
bool HashTable::erase(const string& id) {
    size_t index = hashFunction(id);
    HashNode* current = buckets[index];
    HashNode* previous = nullptr;

    while (current != nullptr) {
        if (current->data.getId() == id) {
            if (previous == nullptr) {
                buckets[index] = current->next;  // Node dau
            } else {
                previous->next = current->next; // Node giua/cuoi
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

// Lay tat ca Order* de cho cac module khac doc/loc/sap xep tam.
vector<Order*> HashTable::allOrders() {
    vector<Order*> result;
    result.reserve(orderCount);

    for (size_t i = 0; i < buckets.size(); ++i) {
        HashNode* current = buckets[i];
        while (current != nullptr) {
            result.push_back(&current->data);
            current = current->next;
        }
    }
    return result;
}

size_t HashTable::size() const {
    return orderCount;
}
