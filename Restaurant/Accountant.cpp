#include "Accountant.h"
#include "Product.h"
#include "MenuItem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Administrator.h"

using namespace std;

void Accountant::SubmitProductRequest(const string& productName, int quantity) {
    if (quantity <= 0) {
        cout << "Неверное количество продукта. Попробуйте еще раз." << endl;
        return;
    }
    productRequests[productName] += quantity;
}

void Accountant::ReceiveProductDelivery(const string& productName, int quantity) {
    if (quantity <= 0) {
        cout << "Неверное количество продукта. Попробуйте еще раз." << endl;
        return;
    }
    productDeliveries[productName] += quantity;
}

void Accountant::GenerateFinancialReport(const map<string, shared_ptr<Product>>& products) {
    double totalRevenue = 0.0;
    double totalExpenses = 0.0;

    ofstream outputFile("operations.txt");
    if (outputFile.is_open()) {
        outputFile << "Финансовый отчет:" << endl;
        outputFile << "-----------------" << endl;

        for (const auto& pair : products) {
            double productRevenue = pair.second->GetCost() * pair.second->GetQuantity();
            outputFile << "Продукт: " << pair.second->GetName() << ", Продано: " << pair.second->GetQuantity() << ", Выручка: $" << productRevenue << endl;

            totalRevenue += productRevenue;
            totalExpenses += Product::GetProductPriceFromFile(pair.second->GetName()) * pair.second->GetQuantity();
        }

        outputFile << "-----------------" << endl;
        outputFile << "Общая выручка: $" << totalExpenses << endl;
        outputFile << "Баланс: $" << totalRevenue - totalExpenses << endl;
        outputFile.close();

        cout << "Финансовый отчет сохранен в файле 'operations.txt'." << endl;
        cout << "Общие выручка: $" << totalExpenses << endl;
        cout << "Баланс: $" << totalRevenue - totalExpenses << endl;
    }
    else {
        cout << "Не удалось открыть файл для сохранения финансового отчета." << endl;
    }
}

void Accountant::PrintProductRequests() {
    ifstream inputFile("OrderProduct.txt");
    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            cout << line << endl;
        }
        inputFile.close();
    }
    else {
        cout << "Не удалось открыть файл для чтения информации о заявках." << endl;
    }
}

void Accountant::PrintProductDeliveries() {
    ifstream inputFile("OrderProduct.txt");
    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            cout << line << endl;
        }
        inputFile.close();
    }
    else {
        cout << "Не удалось открыть файл для чтения информации о заявках." << endl;
    }
}

void Accountant::GetSoldItemsReport() {
    vector<pair<string, int>> soldItems;

    ifstream inputFile("sold_items_report.txt");
    if (inputFile.is_open()) {
        string line;
        string itemName;
        int quantity;
        while (getline(inputFile, itemName) && getline(inputFile, line)) {
            istringstream iss(line);
            if (iss >> quantity) {
                soldItems.push_back(make_pair(itemName, quantity));
            }
        }
        inputFile.close();
    }
    else {
        cout << "Не удалось открыть файл с отчетом о проданных блюдах." << endl;
        return;
    }

    ofstream outputFile("sold_items_report.txt", ios::app);
    if (outputFile.is_open()) {
        outputFile << "Отчет о проданных блюдах:" << endl;
        outputFile << "------------------------" << endl;

        if (!soldItems.empty()) {
            for (const auto& pair : soldItems) {
                outputFile << "Блюдо: " << pair.first << ", Продано: " << pair.second << endl;
            }

            outputFile << "------------------------" << endl;
        }
        else {
            outputFile << "Нет данных о проданных блюдах." << endl;
        }
        outputFile.close();
        cout << "Отчет о проданных блюдах сохранен в файле 'sold_items_report.txt'." << endl;
        cout << "Отчет о проданных блюдах:" << endl;

        if (!soldItems.empty()) {
            for (const auto& pair : soldItems) {
                cout << "Блюдо: " << pair.first << ", Продано: " << pair.second << endl;
            }
        }
        else {
            cout << "Нет данных о проданных блюдах." << endl;
        }
    }
    else {
        cout << "Не удалось открыть файл для сохранения отчета о проданных блюдах." << endl;
    }
}

void Accountant::GenerateSalesReport(const map<int, shared_ptr<MenuItem>>& menuItems) {
    double totalSales = 0.0;

    ofstream outputFile("operations.txt", ios::app);
    if (outputFile.is_open()) {
        outputFile << "Отчет о продажах:" << endl;
        outputFile << "-----------------" << endl;

        for (const auto& pair : menuItems) {
            shared_ptr<MenuItem> menuItem = pair.second;
            string itemName = menuItem->GetName();
            int quantity = menuItem->GetQuantity();
            double itemRevenue = menuItem->GetPrice() * quantity;

            outputFile << "Блюдо: " << itemName << ", Продано: " << quantity << ", Выручка: $" << itemRevenue << endl;

            totalSales += itemRevenue;
        }

        outputFile << "-----------------" << endl;
        outputFile << "Общая выручка: $" << totalSales << endl;
        outputFile.close();

        cout << "Отчет о продажах сохранен в файле 'operations.txt'." << endl;
        cout << "Отчет о продажах:" << endl;

        for (const auto& pair : menuItems) {
            shared_ptr<MenuItem> menuItem = pair.second;
            string itemName = menuItem->GetName();
            int quantity = menuItem->GetQuantity();
            double itemRevenue = menuItem->GetPrice() * quantity;

            cout << "Блюдо: " << itemName << ", Продано: " << quantity << ", Выручка: $" << itemRevenue << endl;
        }

        cout << "Общая выручка: $" << totalSales << endl;
    }
    else {
        cout << "Не удалось открыть файл для сохранения отчета о продажах." << endl;
    }
}

void Accountant::run() {
    Administrator::AuditIn("Произведен вход за бухгалтера");
    cout << "Добро пожаловать, бухгалтер!" << endl;

    while (true) {
        cout << endl;
        cout << "Выберите действие:" << endl;
        cout << "1. Вывести список отправленных заявок на продукты" << endl;
        cout << "2. Вывести список принятых поставок продуктов" << endl;
        cout << "3. Сгенерировать финансовый отчет" << endl;
        cout << "4. Сгенерировать отчет о проданных блюдах" << endl;
        cout << "5. Выйти из программы" << endl;

        int choice;
        cin >> choice;
        if (choice == 1) {
            Administrator::AuditIn("Cписок отправленных заявок на продукты");
            PrintProductRequests();
        }
        else if (choice == 2) {
            Administrator::AuditIn("Cписок принятых поставок продуктов");
            PrintProductDeliveries();
        }
        else if (choice == 3) {
            map<string, shared_ptr<Product>> products;
            Administrator::AuditIn("Создать финансовый отчет");
            GenerateFinancialReport(products);
        }
        else if (choice == 4) {
            map<int, shared_ptr<MenuItem>> menuItems;
            Administrator::AuditIn("Создать отчет о проданных блюдах");
            GenerateSalesReport(menuItems);
        }
        else if (choice == 5) {
            Administrator::AuditIn("Бугалтер вышел");
            break;
        }
        else {
            cout << "Неверный выбор. Попробуйте еще раз." << endl;
        }
    }
}