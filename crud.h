#ifndef CRUD_H
#define CRUD_H

#include "hashtable.h"
#include "queue.h"
#include <string>
#include <vector>

bool isValidDate(const std::string& date);

bool createOrder(HashTable& hash, OrderQueue& queue,
    const std::string& id, const std::string& customerName,
    const std::string& createdAt, long long totalAmount);

void inputAndCreateOrder(HashTable& hash, OrderQueue& queue);
bool deleteOrder(HashTable& hash, OrderQueue& queue, const std::string& id);
void viewOrder(HashTable& hash, const std::string& id);
void listOrders(HashTable& hash);

// Thuật toán nâng cao: Binary Search theo mã đơn hàng
Order* binarySearchById(HashTable& hash, const std::string& id);

// Bộ test case kiểm thử độc lập module của Hào
void testCRUD();

#endif
