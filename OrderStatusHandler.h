#ifndef ORDER_STATUS_HANDLER_H
#define ORDER_STATUS_HANDLER_H

#include <functional>
#include <string>
#include "order.h"

class OrderStatusHandler {
public:
    // Callback phai go don khoi Queue thanh cong truoc khi doi trang thai.
    bool pheDuyet(Order* don,
                  const std::function<bool(const std::string&)>& removeFromQueue);

    bool huyDon(Order* don,
                const std::function<bool(const std::string&)>& removeFromQueue);

    bool hoanThanh(Order* don);
};

#endif
