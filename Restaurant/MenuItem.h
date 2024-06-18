#ifndef MENUITEM_H
#define MENUITEM_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Product.h"
#include <fstream>
#include <sstream>
#include <iostream>

class MenuItem {
public:
    MenuItem();
    MenuItem(const std::string& name, int weight,const std::vector<std::shared_ptr<Product>>& ingredients,
        const std::string& description, int cookingTime, double price, int quantity);
    MenuItem(const std::string& name, double price, int quantity);

    const std::string& GetName() const;
    int GetQuantity() const;
    void SetQuantity(int newQuantity);
    double GetPrice() const;

    static void LoadFromFile(const std::string& filename, std::map<int, std::shared_ptr<MenuItem>>& menuItems);
    static void SaveToFile(const std::string& filename, const std::map<int, std::shared_ptr<MenuItem>>& menuItems);

    static void SaveToFileForCook(const std::string& filename, const std::map<std::string, std::shared_ptr<MenuItem>>& menuItems);
    static void LoadFromFileForCook(const std::string& filename, std::map<std::string, std::shared_ptr<MenuItem>>& menuItems);

    static std::string GetItemsIngredients(const std::vector<std::shared_ptr<Product>>& ingredients);
    int GetRemainingQuantity() const { return quantity - soldQuantity;}
    int GetWeight() const;
    const std::vector<std::shared_ptr<Product>>& GetIngredients() const;
    std::string GetDescription() const;
    int GetCookingTime() const;
    void AddMenuItem(const std::shared_ptr<MenuItem>& item);
    void SetName(const std::string& newName);
    void SetWeight(int newWeight);
    void SetCookingTime(int newCookingTime);
    void SetDescription(const std::string& newDescription);
    void SetPrice(double newPrice);
    void SetIngredients(const std::vector<std::shared_ptr<Product>>& newIngredients) {ingredients = newIngredients;}

    static void RemoveMenuItem(const std::string& itemName, std::map<int, std::shared_ptr<MenuItem>>& menuItems);
    static std::shared_ptr<MenuItem> FindMenuItemByName(const std::string& name); 

private:
    std::vector<std::shared_ptr<MenuItem>> menuItems;
    std::string name;
    int quantity;
    double price;
    int weight;
    std::vector<std::shared_ptr<Product>> ingredients;
    std::string description;
    int cookingTime;
    int soldQuantity = 0;
};

#endif 
