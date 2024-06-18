#include "Warehouse.h"
#include "Administrator.h"
#include <iostream>
#include <iomanip>
#include "Provider.h"
#include "Product.h"
#include "Accountant.h"

using namespace std;

map<string, int> Warehouse::inventory;
double Warehouse::GetProductPrice(const string& productName) const {
    if (prices.find(productName) != prices.end()) {
        return prices.at(productName);
    }
    else {
        return -1.0;
    }
}

void Warehouse::RemoveProduct(const string& productName, int quantity) {
    inventory[productName] += quantity;

    map<int, shared_ptr<Product>> productMap;

    Product::LoadFromFile("products.txt", productMap);

    for (auto& pair : productMap) {
        if (pair.second->GetName() == productName) {
            pair.second->SetQuantity(pair.second->GetQuantity() - quantity);
            break;
        }
    }

    Product::SaveToFile("products.txt", productMap);

}

int Warehouse::GetProductId(const string& productName) {
    for (const auto& pair : inventory) {
        if (pair.first == productName) {
            return pair.second;
        }
    }
    return -1; 
}

double Warehouse::GetProductCost(const string& productName) {
    for (const auto& pair : inventory) {
        if (pair.first == productName) {
            return pair.second;
        }
    }
}

void Warehouse::AddProduct(const string& productName, int quantity) {
    inventory[productName] += quantity;

    map<int, shared_ptr<Product>> productMap;

    Product::LoadFromFile("products.txt", productMap);

    for (auto& pair : productMap) {
        if (pair.second->GetName() == productName) {
            pair.second->SetQuantity(pair.second->GetQuantity() + quantity);
            break;
        }
    }
    Product::SaveToFile("products.txt", productMap);
}

void Warehouse::DisplayInventory() const {
    cout << "��������� ������:\n";
    for (const auto& item : inventory) {
        cout << item.first << ": " << item.second << endl;
    }
}

bool Warehouse::CanSendOrder(const map<string, int>& order, double balance) const {
    double totalCost = 0.0;
    for (const auto& item : order) {
        const string& productName = item.first;
        int quantity = item.second;

        if (inventory.find(productName) == inventory.end()) {
            cout << "������� \"" << productName << "\" ����������� �� ������.\n";
            return false;
        }

        int availableQuantity = inventory.at(productName);
        if (quantity > availableQuantity) {
            cout << "������������� ���������� �������� \"" << productName << "\" �� ������.\n";
            return false;
        }

        if (prices.find(productName) == prices.end()) {
            cout << "���� �������� \"" << productName << "\" �� �������.\n";
            return false;
        }

        double productPrice = prices.at(productName);
        totalCost += productPrice * quantity;
    }

    if (balance < totalCost) {
        cout << "������������� ������ �������� ������� ��� �������� ������.\n";
        return false;
    }

    return true;
}

map<string, shared_ptr<Product>> Warehouse::GetProducts() {
    map<string, shared_ptr<Product>> products;

    map<int, shared_ptr<Product>> productMap;
    Product::LoadFromFile("products.txt", productMap);

    for (const auto& pair : productMap) {
        int id = pair.first;
        shared_ptr<Product> product = pair.second;
        products[to_string(id)] = product;
    }

    return products;
}

void Warehouse::LoadProducts(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "������ ��� �������� �����: " << filename << endl;
        return;
    }

    inventory.clear();
    prices.clear();
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        int id, cost, quantity;
        string name;
        if (iss >> id >> name >> cost >> quantity) {
            inventory[name] = quantity;
            prices[name] = cost;
        }
    }

    file.close();
}

void Warehouse::SaveProducts(const string& filename) {
    ofstream file(filename);
    if (!file) {
        cout << "������ ��� �������� ����� ��� ������: " << filename << endl;
        return;
    }

    for (const auto& item : inventory) {
        const string& productName = item.first;
        int quantity = item.second;
        if (prices.find(productName) != prices.end()) {
            double price = prices[productName];
            file << productName << " " << price << " " << quantity << endl;
        }
    }

    file.close();
}

void Warehouse::Run() {
    Administrator::AuditIn("���������� ���� �� ����������");
    map<int, shared_ptr<Product>> products;
    map<string, shared_ptr<Product>> pro;
    map<string, shared_ptr<Employee>> employees;
    shared_ptr<MenuItem> menu = make_shared<MenuItem>();
    Administrator admin{ products, employees, menu };
    Accountant accountant;


    for (const auto& pair : products) {
        int key = pair.first;
        shared_ptr<Product> value = pair.second;
        pro[to_string(key)] = value;
    }
    cout << "Warehouse Management System" << endl;

    while (true) {

        cout << "1. �������� �������" << endl;
        cout << "2. ������� �������" << endl;
        cout << "3. �������� ���������� ���������" << endl;
        cout << "4. ���������� �����" << endl;
        cout << "5. ��������� ������ �� ��������" << endl;
        cout << "6. �����" << endl;
        cout << "�����: ";
        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            int id;
            string name;
            double cost;
            int quantity;

            cout << "������� ID ��������: ";
            cin >> id;
            cout << "������� ��� �������� : ";
            cin.ignore();
            getline(cin, name);
            cout << "������� ��������� ��������: ";
            cin >> cost;
            cout << "������� ���������� ��������: ";
            cin >> quantity;

            admin.AddProduct(products, id, name, cost, quantity);
            break;
        }
        case 2: {
            int id;
            cout << "������� ID ��������: ";
            cin >> id;

            admin.RemoveProduct(id);
            cout << "������� ������� ������." << endl;
            Administrator::AuditIn("������� �� ID " + to_string(id) + " ������� ������");
            break;
        }
        case 3: {
            int id;
            cout << "������� ID ��������: ";
            cin >> id;

            admin.UpdateProduct(id);
            cout << "������� ������� �������." << endl;
            Administrator::AuditIn("������� �� ID " + to_string(id) + " ������� ��������");
            break;
        }
        case 4: {
            map<int, shared_ptr<Product>> products1;

            Product::LoadFromFile("products.txt", products1);
            admin.ShowProducts(products1);
            break;
        }
        case 5: {
            map<string, int> order;
            double balance;
            LoadProducts("products.txt");

            string orderFilename = "OrderProduct.txt";
            ofstream orderFile(orderFilename);
            if (!orderFile) {
                cout << "������ � �������� order �����." << endl;
                break;
            }

            cout << "������� ������ ������:" << endl;
            cout << "��� �������� (-1 to stop): ";
            string productName;
            cin >> productName;

            while (productName != "-1") {
                int quantity;
                cout << "����������: ";
                cin >> quantity;

                order[productName] = quantity;

                orderFile << setw(15) << left << productName << quantity << endl;

                cout << "��� �������� (-1 to stop): ";
                cin >> productName;
                Administrator::AuditIn("������ �� ������� " + productName + " � ��������� " + to_string(quantity));
            }

            orderFile.close();

            cout << "������� �������: ";
            cin >> balance;
            if (CanSendOrder(order, balance)) {
                cout << "������ �� �������� ����� ���� ����������." << endl;
                Administrator::AuditIn("������ �� �������� ����� ���� ����������.");

                Provider provider;
                provider.ProcessOrder(order, balance);
                provider.ViewOrder(orderFilename);
            }
            accountant.GenerateFinancialReport(pro);
            break;
        }
        case 6: {
            SaveProducts("products.txt");
            Administrator::AuditIn("����� ����������");
            cout << "�����..." << endl;
            return;
        }
        default: {
            cout << "������������ �����." << endl;
            break;
        }
        }
    }
}