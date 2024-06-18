#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <iostream>
#include <map>
#include <string>
#include <memory>
#include "Product.h"

class Warehouse {
private:
    std::map<std::string, double> prices;
public:
    static std::map <std::string, int> inventory;

    double GetProductPrice(const std::string& productName) const;
    static void RemoveProduct(const std::string& productName, int quantity);
    void AddProduct(const std::string& productName, int quantity);
    void DisplayInventory() const;
    bool CanSendOrder(const std::map<std::string, int>& order, double balance) const;
    void LoadProducts(const std::string& filename);
    void SaveProducts(const std::string& filename);

    int GetProductIdByName(const std::string& productName);
    double GetProductCostByName(const std::string& productName);
    int GetProductId(const std::string& productName);
    double GetProductCost(const std::string& productName);

    void Run();
    static std::map<std::string, std::shared_ptr<Product>> GetProducts() ;
};

#endif 
