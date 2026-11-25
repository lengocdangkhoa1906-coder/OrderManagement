#ifndef STATISTICS_H
#define STATISTICS_H

#include "hashtable.h"
#include <map>
#include <string>
#include <vector>
using namespace std;

// Chi DOC du lieu tu HashTable: khong sua, khong delete Order*.
// (HashTable la noi so huu Order; module nay chi giu con tro tham chieu.)

// Dem so don theo tung trang thai (trang thai 0 don van co mat, gia tri 0).
// Do phuc tap: O(n)
map<OrderStatus, int> countByStatus(HashTable &hash);

// Tong gia tri (VND) cua tat ca don hang.
// Do phuc tap: O(n)
long long sumAllAmounts(HashTable &hash);

// Loc cac don co trang thai = status.
// Do phuc tap: O(n)
vector<Order *> filterByStatus(HashTable &hash, OrderStatus status);

// 3 don co tong tien lon nhat, sap giam dan (it hon 3 don thi tra ve so don hien co).
// Do phuc tap: O(n), bo nho phu O(1)
vector<Order *> top3Orders(HashTable &hash);

// Doi trang thai sang chuoi de in ra man hinh.
string statusName(OrderStatus status);

// In bao cao tong hop: tong so don, so don theo trang thai, tong tien, Top 3 (menu 5).
void showStatistics(HashTable &hash);

// Loc theo trang thai roi in danh sach cac don.
void showFilterByStatus(HashTable &hash, OrderStatus status);

// Bo test doc lap cua module thong ke.
void testStatistics();

#endif