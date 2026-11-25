#include "XuLyTrangThai.h"
#include <iostream>

using namespace std;

bool XuLyTrangThai::pheDuyet(
    Order* don,
    const function<bool(const string&)>& removeFromQueue) {

    if (don == nullptr) {
        cout << "Khong tim thay don hang.\n";
        return false;
    }

    if (don->trangThai != CHO_PHE_DUYET) {
        cout << "Khong the phe duyet: don khong o trang thai Cho phe duyet.\n";
        return false;
    }

    don->trangThai = DA_PHE_DUYET;

    // Don da duoc phe duyet thi khong con nam trong Queue cho phe duyet.
    if (removeFromQueue) {
        removeFromQueue(don->maDon);
    }

    cout << "Phe duyet don " << don->maDon << " thanh cong.\n";
    return true;
}

bool XuLyTrangThai::huyDon(
    Order* don,
    const function<bool(const string&)>& removeFromQueue) {

    if (don == nullptr) {
        cout << "Khong tim thay don hang.\n";
        return false;
    }

    if (don->trangThai != CHO_PHE_DUYET) {
        cout << "Khong the huy: chi huy don dang Cho phe duyet.\n";
        return false;
    }

    don->trangThai = DA_HUY;

    // Don da huy phai duoc dua ra khoi Queue.
    if (removeFromQueue) {
        removeFromQueue(don->maDon);
    }

    cout << "Huy don " << don->maDon << " thanh cong.\n";
    return true;
}

bool XuLyTrangThai::hoanThanh(Order* don) {
    if (don == nullptr) {
        cout << "Khong tim thay don hang.\n";
        return false;
    }

    if (don->trangThai != DA_PHE_DUYET) {
        cout << "Khong the hoan thanh: don phai duoc phe duyet truoc.\n";
        return false;
    }

    don->trangThai = HOAN_THANH;
    cout << "Don " << don->maDon << " da hoan thanh.\n";
    return true;
}
