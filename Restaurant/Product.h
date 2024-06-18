#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <map>
#include <memory>

class Product {
private:
    int id;
    std::string name;
    double cost;
    int quantity;

public:
    Product() = default;
    Product(const std::string& name, double cost);
    Product(int id, const std::string& name, double cost, int quantity);

    int GetId() const;
    const std::string& GetName() const;
    double GetCost() const;
    int GetQuantity() const; 

    void SetName(const std::string& newName);
    void SetCost(double newCost);
    void SetQuantity(int newQuantity);

    static double GetProductPriceFromFile(const std::string& productName);
    static void LoadFromFile(const std::string& filename, std::map<int, std::shared_ptr<Product>>& products);
    static void SaveToFile(const std::string& filename, const std::map<int, std::shared_ptr<Product>>& products);
};
#endif