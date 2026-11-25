#ifndef XULYTRANGTHAI_H
#define XULYTRANGTHAI_H

#include <functional>
#include <string>
#include "Order.h"

class XuLyTrangThai {
public:
    // removeFromQueue nhan ma don va goi ham xoa don tu Queue cua ban Dang.
    bool pheDuyet(Order* don,
                  const std::function<bool(const std::string&)>& removeFromQueue);

    bool huyDon(Order* don,
                const std::function<bool(const std::string&)>& removeFromQueue);

    bool hoanThanh(Order* don);
};

#endif
