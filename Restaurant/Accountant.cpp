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
        cout << "�������� ���������� ��������. ���������� ��� ���." << endl;
        return;
    }
    productRequests[productName] += quantity;
}

void Accountant::ReceiveProductDelivery(const string& productName, int quantity) {
    if (quantity <= 0) {
        cout << "�������� ���������� ��������. ���������� ��� ���." << endl;
        return;
    }
    productDeliveries[productName] += quantity;
}

void Accountant::GenerateFinancialReport(const map<string, shared_ptr<Product>>& products) {
    double totalRevenue = 0.0;
    double totalExpenses = 0.0;

    ofstream outputFile("operations.txt");
    if (outputFile.is_open()) {
        outputFile << "���������� �����:" << endl;
        outputFile << "-----------------" << endl;

        for (const auto& pair : products) {
            double productRevenue = pair.second->GetCost() * pair.second->GetQuantity();
            outputFile << "�������: " << pair.second->GetName() << ", �������: " << pair.second->GetQuantity() << ", �������: $" << productRevenue << endl;

            totalRevenue += productRevenue;
            totalExpenses += Product::GetProductPriceFromFile(pair.second->GetName()) * pair.second->GetQuantity();
        }

        outputFile << "-----------------" << endl;
        outputFile << "����� �������: $" << totalExpenses << endl;
        outputFile << "������: $" << totalRevenue - totalExpenses << endl;
        outputFile.close();

        cout << "���������� ����� �������� � ����� 'operations.txt'." << endl;
        cout << "����� �������: $" << totalExpenses << endl;
        cout << "������: $" << totalRevenue - totalExpenses << endl;
    }
    else {
        cout << "�� ������� ������� ���� ��� ���������� ����������� ������." << endl;
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
        cout << "�� ������� ������� ���� ��� ������ ���������� � �������." << endl;
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
        cout << "�� ������� ������� ���� ��� ������ ���������� � �������." << endl;
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
        cout << "�� ������� ������� ���� � ������� � ��������� ������." << endl;
        return;
    }

    ofstream outputFile("sold_items_report.txt", ios::app);
    if (outputFile.is_open()) {
        outputFile << "����� � ��������� ������:" << endl;
        outputFile << "------------------------" << endl;

        if (!soldItems.empty()) {
            for (const auto& pair : soldItems) {
                outputFile << "�����: " << pair.first << ", �������: " << pair.second << endl;
            }

            outputFile << "------------------------" << endl;
        }
        else {
            outputFile << "��� ������ � ��������� ������." << endl;
        }
        outputFile.close();
        cout << "����� � ��������� ������ �������� � ����� 'sold_items_report.txt'." << endl;
        cout << "����� � ��������� ������:" << endl;

        if (!soldItems.empty()) {
            for (const auto& pair : soldItems) {
                cout << "�����: " << pair.first << ", �������: " << pair.second << endl;
            }
        }
        else {
            cout << "��� ������ � ��������� ������." << endl;
        }
    }
    else {
        cout << "�� ������� ������� ���� ��� ���������� ������ � ��������� ������." << endl;
    }
}

void Accountant::GenerateSalesReport(const map<int, shared_ptr<MenuItem>>& menuItems) {
    double totalSales = 0.0;

    ofstream outputFile("operations.txt", ios::app);
    if (outputFile.is_open()) {
        outputFile << "����� � ��������:" << endl;
        outputFile << "-----------------" << endl;

        for (const auto& pair : menuItems) {
            shared_ptr<MenuItem> menuItem = pair.second;
            string itemName = menuItem->GetName();
            int quantity = menuItem->GetQuantity();
            double itemRevenue = menuItem->GetPrice() * quantity;

            outputFile << "�����: " << itemName << ", �������: " << quantity << ", �������: $" << itemRevenue << endl;

            totalSales += itemRevenue;
        }

        outputFile << "-----------------" << endl;
        outputFile << "����� �������: $" << totalSales << endl;
        outputFile.close();

        cout << "����� � �������� �������� � ����� 'operations.txt'." << endl;
        cout << "����� � ��������:" << endl;

        for (const auto& pair : menuItems) {
            shared_ptr<MenuItem> menuItem = pair.second;
            string itemName = menuItem->GetName();
            int quantity = menuItem->GetQuantity();
            double itemRevenue = menuItem->GetPrice() * quantity;

            cout << "�����: " << itemName << ", �������: " << quantity << ", �������: $" << itemRevenue << endl;
        }

        cout << "����� �������: $" << totalSales << endl;
    }
    else {
        cout << "�� ������� ������� ���� ��� ���������� ������ � ��������." << endl;
    }
}

void Accountant::run() {
    Administrator::AuditIn("���������� ���� �� ����������");
    cout << "����� ����������, ���������!" << endl;

    while (true) {
        cout << endl;
        cout << "�������� ��������:" << endl;
        cout << "1. ������� ������ ������������ ������ �� ��������" << endl;
        cout << "2. ������� ������ �������� �������� ���������" << endl;
        cout << "3. ������������� ���������� �����" << endl;
        cout << "4. ������������� ����� � ��������� ������" << endl;
        cout << "5. ����� �� ���������" << endl;

        int choice;
        cin >> choice;
        if (choice == 1) {
            Administrator::AuditIn("C����� ������������ ������ �� ��������");
            PrintProductRequests();
        }
        else if (choice == 2) {
            Administrator::AuditIn("C����� �������� �������� ���������");
            PrintProductDeliveries();
        }
        else if (choice == 3) {
            map<string, shared_ptr<Product>> products;
            Administrator::AuditIn("������� ���������� �����");
            GenerateFinancialReport(products);
        }
        else if (choice == 4) {
            map<int, shared_ptr<MenuItem>> menuItems;
            Administrator::AuditIn("������� ����� � ��������� ������");
            GenerateSalesReport(menuItems);
        }
        else if (choice == 5) {
            Administrator::AuditIn("�������� �����");
            break;
        }
        else {
            cout << "�������� �����. ���������� ��� ���." << endl;
        }
    }
}