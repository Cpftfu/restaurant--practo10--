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
    cout << "��������� ������..." << endl;
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

        cout << "�����: " << productName << ", ����������: " << quantity << " - ���������" << endl;

        warehouse.AddProduct(productName, quantity);

        double productPrice = warehouse.GetProductPrice(productName);
        totalCost += productPrice * quantity;
    }

    balance += totalCost;

    cout << "����� ������� ���������. ����� ���������: $" << totalCost << endl;
    cout << "���������� ������: $" << balance << endl;

    map<string, shared_ptr<Product>> products = warehouse.GetProducts();
    accountant.GenerateFinancialReport(products);
}
map<string, int> Provider::ViewOrder(const string& orderFilename) {
    map<string, int> order;
    ifstream file(orderFilename);
    if (!file) {
        cout << "������ ��� �������� �����: " << orderFilename << endl;
        return order;
    }

    cout << "������ ������ �� ����� \"" << orderFilename << "\":" << endl;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string productName;
        int quantity;
        if (iss >> productName >> quantity) {
            cout << "�����: " << productName << ", ����������: " << quantity << endl;
            order[productName] = quantity;
        }
    }

    file.close();
    return order;
}
void Provider::RunProvider() {
    Administrator::AuditIn("���������� ���� �� ����������");
    map<string, int> order;
    double balance = 10000.00;
    cout << "������� ����������" << endl;
    while (true) {
        cout << endl;
        cout << "1. ����������� �����" << endl;
        cout << "2. ���������� �����" << endl;
        cout << "3. �����" << endl;
        cout << "������� ��� �����: ";
        int choice;
        cin >> choice;
        switch (choice) {
        case 1:
            Administrator::AuditIn("�������� ������ �� �������� �����������");
            order = ViewOrder("OrderProduct.txt");
            break;
        case 2:
            if (!order.empty()) {
                Administrator::AuditIn("��������� ������ �� �������� �����������");
                ProcessOrder(order, balance);
                order.clear();
            }
            else {
                cout << "��� ������ ��� ���������." << endl;
            }
            break;
        case 3:
            Administrator::AuditIn("��������� �����");
            cout << "�����..." << endl;
            return;
        default:
            cout << "������������ �����. ����������, ��������� �������." << endl;
            break;
        }
    }
}