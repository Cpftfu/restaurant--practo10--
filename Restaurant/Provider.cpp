#include "Provider.h"
#include "Warehouse.h"
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>   
#include <iomanip>   
#include <algorithm> 
#include "Accountant.h"
#include "Administrator.h"

using namespace std;

void Provider::ProcessOrder(const map<string, int>& order, double& balance) {
    cout << "Обработка заказа..." << endl;
    double totalCost = 0.0;
    Warehouse warehouse;

    Accountant accountant;

    for (const auto& item : order) {
        const string& productName = item.first;
        int quantity = item.second;
        accountant.ReceiveProductDelivery(productName, quantity);
    }

    for (const auto& item : order) {
        const string& productName = item.first;
        int quantity = item.second;

        cout << "Товар: " << productName << ", Количество: " << quantity << " - Отгружено" << endl;

        warehouse.AddProduct(productName, quantity);

        double productPrice = warehouse.GetProductPrice(productName);
        totalCost += productPrice * quantity;
    }

    balance += totalCost;

    cout << "Заказ успешно обработан. Общая стоимость: $" << totalCost << endl;
    cout << "Оставшийся баланс: $" << balance << endl;

    map<string, shared_ptr<Product>> products = warehouse.GetProducts();
    accountant.GenerateFinancialReport(products);
}
map<string, int> Provider::ViewOrder(const string& orderFilename) {
    map<string, int> order;
    ifstream file(orderFilename);
    if (!file) {
        cout << "Ошибка при открытии файла: " << orderFilename << endl;
        return order;
    }

    cout << "Детали заказа из файла \"" << orderFilename << "\":" << endl;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string productName;
        int quantity;
        if (iss >> productName >> quantity) {
            cout << "Товар: " << productName << ", Количество: " << quantity << endl;
            order[productName] = quantity;
        }
    }

    file.close();
    return order;
}
void Provider::RunProvider() {
    Administrator::AuditIn("Произведен вход за поставщика");
    map<string, int> order;
    double balance = 10000.00;
    cout << "Система поставщика" << endl;
    while (true) {
        cout << endl;
        cout << "1. Просмотреть заказ" << endl;
        cout << "2. Обработать заказ" << endl;
        cout << "3. Выход" << endl;
        cout << "Введите ваш выбор: ";
        int choice;
        cin >> choice;
        switch (choice) {
        case 1:
            Administrator::AuditIn("Просмотр заказа на продукты поставщиком");
            order = ViewOrder("OrderProduct.txt");
            break;
        case 2:
            if (!order.empty()) {
                Administrator::AuditIn("Обработка заказа на продукты поставщиком");
                ProcessOrder(order, balance);
                order.clear();
            }
            else {
                cout << "Нет заказа для обработки." << endl;
            }
            break;
        case 3:
            Administrator::AuditIn("Поставщик вышел");
            cout << "Выход..." << endl;
            return;
        default:
            cout << "Некорректный выбор. Пожалуйста, повторите попытку." << endl;
            break;
        }
    }
}