#ifndef ORDER_H
#define ORDER_H

#include <vector>
#include <memory>
#include <string>
#include "MenuItem.h"

enum class OrderStatus {
    PAID_AND_SENT_TO_KITCHEN,
    IN_PROCESS,
    READY_FOR_DELIVERY,
    COMPLETED,
    UNKNOWN
};

class Order {
public:
    void AddToBasket(std::shared_ptr<MenuItem> item);
    void AddItem(const std::shared_ptr<MenuItem>& item);
    double CalculateTotal() const;
    bool ConfirmOrder() const;
    void TrackOrderStatus() const;
    void GenerateReceipt(const std::string& filename) const;
    const std::vector<std::shared_ptr<MenuItem>>& GetBasket() const;
    void ClearBasket();
    void SetStatus(OrderStatus status);

    void SaveOrder(const std::string& filename) const;

    static std::string GetStatusString(OrderStatus status);

    OrderStatus GetStatus() const;


    static void SaveOrderStatus(OrderStatus status);
    static OrderStatus LoadOrderStatus();

private: std::vector<std::shared_ptr<MenuItem>> basket;
       OrderStatus status;
};

#endif
