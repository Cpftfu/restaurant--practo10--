#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <memory>
#include "Product.h"
#include "Employee.h"
#include "MenuItem.h"

class Administrator {
private:
    std::map<int, std::shared_ptr<Product>>& products;
    std::map<std::string, std::shared_ptr<Employee>>& employees;
    std::shared_ptr<MenuItem>& menu;

public:
    Administrator() = default;
    Administrator(std::map< int,std::shared_ptr< Product>>& products,
        std::map<std::string, std::shared_ptr<Employee>>& employees,
        std::shared_ptr<MenuItem>& menu);

    void Run();
    void AddProduct(std::map<int, std::shared_ptr<Product>>& products, int id, const std::string& name, double cost, int quantity);
    void UpdateProduct(int id);
    void RemoveProduct( int id);
    void DisplayProducts() const;
    void AddEmployee();
    void UpdateEmployee();
    void RemoveEmployee();
    void DisplayEmployees() const;
    void AddMenuItem();

    void ShowProducts(const std::map<int, std::shared_ptr<Product>>& products);
    void CreateOrder(std::map<int, std::shared_ptr<Product>>& products, const std::string& filename);
    void UpdateMenuItem();
    void DeleteMenuItem();



    static void AuditIn(const std::string& audit);
    static void AuditOut();

};

#endif
