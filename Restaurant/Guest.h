#ifndef GUEST_H
#define GUEST_H

#include <map>
#include <memory>
#include <iostream>

#include "MenuItem.h"
#include "Order.h"

class Guest {
public:
    Guest();
    Guest(const std::map<int, std::shared_ptr<MenuItem>>& menuItems);

    void UpdateProductQuantities(std::map<std::string, std::shared_ptr<Product>>& products, const std::shared_ptr<MenuItem>& item, int quantity);
    void RestoreProductQuantities(std::map<std::string, std::shared_ptr<Product>>& products);
    void UpdateProductFile(const std::map<std::string, std::shared_ptr<Product>>& products);
    void DisplayMenu() const;
    void TrackOrderStatus() const;

    void Run();

    void MakeOrder(std::map<std::string, std::shared_ptr<Product>>& products);

private:
    std::map<int, std::shared_ptr<MenuItem>> menuItems;
    Order currentOrder;
};

#endif
