#include "OrderStatusHandler.h"
#include <iostream>

using namespace std;

bool OrderStatusHandler::pheDuyet(
    Order* don,
    const function<bool(const string&)>& removeFromQueue) {

    if (don == nullptr) {
        cout << "Khong tim thay don hang.\n";
        return false;
    }

    if (don->getStatus() != OrderStatus::PENDING) {
        cout << "Khong the phe duyet: don khong o trang thai Cho phe duyet.\n";
        return false;
    }

    const string id = don->getId();
    if (!removeFromQueue || !removeFromQueue(id)) {
        cout << "Khong the phe duyet: khong go duoc don khoi Queue.\n";
        return false;
    }

    don->setStatus(OrderStatus::APPROVED);
    cout << "Phe duyet don " << id << " thanh cong.\n";
    return true;
}

bool OrderStatusHandler::huyDon(
    Order* don,
    const function<bool(const string&)>& removeFromQueue) {

    if (don == nullptr) {
        cout << "Khong tim thay don hang.\n";
        return false;
    }

    if (don->getStatus() != OrderStatus::PENDING) {
        cout << "Khong the huy: chi huy don dang Cho phe duyet.\n";
        return false;
    }

    const string id = don->getId();
    if (!removeFromQueue || !removeFromQueue(id)) {
        cout << "Khong the huy: khong go duoc don khoi Queue.\n";
        return false;
    }

    don->setStatus(OrderStatus::CANCELLED);
    cout << "Huy don " << id << " thanh cong.\n";
    return true;
}

bool OrderStatusHandler::hoanThanh(Order* don) {
    if (don == nullptr) {
        cout << "Khong tim thay don hang.\n";
        return false;
    }

    if (don->getStatus() != OrderStatus::APPROVED) {
        cout << "Khong the hoan thanh: don phai duoc phe duyet truoc.\n";
        return false;
    }

    const string id = don->getId();
    don->setStatus(OrderStatus::COMPLETED);
    cout << "Don " << id << " da hoan thanh.\n";
    return true;
}
