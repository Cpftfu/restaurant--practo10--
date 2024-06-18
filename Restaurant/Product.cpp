#include "Product.h"
#include <iostream>
#include <fstream>

using namespace std;

Product::Product(int id, const string& name, double cost, int quantity) : id(id), name(name), cost(cost), quantity(quantity) {}

int Product::GetId() const {
    return id;
}

const string& Product::GetName() const {
    return name;
}

double Product::GetCost() const {
    return cost;
}

int Product::GetQuantity() const {
    return quantity;
}

void Product::SetName(const string& newName) {
    name = newName;
}

void Product::SetCost(double newCost) {
    cost = newCost;
}

void Product::SetQuantity(int newQuantity) {
    quantity = newQuantity;
}

void Product::LoadFromFile(const string& filename, map<int, shared_ptr<Product>>& products) {
    ifstream file(filename);
    if (file.is_open()) 
    {
        products.clear();

        int id;
        string name;
        double cost;
        int quantity;

        while (file >> id >> name >> cost >> quantity) 
        {
            shared_ptr<Product> product = make_shared<Product>(id, name, cost, quantity);
            products.emplace(make_pair(id, product));
        }

        file.close();
    }
    else {
        cout << "Ошибка в открытии файла: " << filename << endl;
    }
}

void Product::SaveToFile(const string& filename, const map<int, shared_ptr<Product>>& products) {
    ofstream file(filename);
    if (file.is_open()) 
    {
        file.clear();
        for (const auto& pair : products) {
            file << pair.first << " " << pair.second->GetName() << " " << pair.second->GetCost() << " " << pair.second->GetQuantity() << endl;
        }

        file.close();
    }
    else { cout << "Ошибка в открытии файла: " << filename << endl;}
}

double Product::GetProductPriceFromFile(const string& productName) {
    ifstream file("products.txt");
    if (file.is_open()) {
        string name;
        double cost;
        int quantity, id;

        while (file >> id >> name >> cost >> quantity) {
            if (name == productName) {
                file.close();
                return cost;
            }
        }

        file.close();
    }
    else { cout << "Ошибка в открытии файла: products.txt" << endl; }
    
}

// не знаю, но почему без этого не запускаеться
Product::Product(const string& name, double cost) : name(name), cost(cost), id(0) {}