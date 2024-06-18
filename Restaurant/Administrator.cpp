#include <iostream>
#include <fstream>
#include <sstream>
#include "Administrator.h"
#include <memory>
#include "Product.h"
#include "MenuItem.h"
#include "Employee.h"
#include <iomanip>
#include "../LibHash/Header.h"
#include <chrono>
#include <ctime>

using namespace std;

Administrator::Administrator(map<int, shared_ptr <Product>>& _products, map<string, shared_ptr<Employee>>& employees, shared_ptr<MenuItem>& menu) : products(_products), employees(employees), menu(menu) {}

void Administrator::Run() {
    Administrator::AuditIn("���������� ���� �� ������");
    int choice = -1;
    while (choice != 0) {
        cout << "���� ������:\n";
        cout << "1. �������� �������\n";
        cout << "2. �������� �������\n";
        cout << "3. ������� �������\n";
        cout << "4. �������� ���������\n";
        cout << "5. �������� ����������\n";
        cout << "6. �������� ����������\n";
        cout << "7. ������� �����������\n";
        cout << "8. ������ �����������\n";
        cout << "9. �������� ����� ����\n";
        cout << "10. �������� ����� ����\n";
        cout << "11. ������� ����� ����\n";
        cout << "12. ���������� ���� ���� ��������\n";
        cout << "0. �������������\n";
        cout << "�����: ";
        cin >> choice;

        int id, quantity;
        string name;
        double cost;
        switch (choice) {
        case 1:
            cout << "������� ID ��������: ";
            cin >> id;
            cout << "������� ��� ��������: ";
            cin.ignore();
            getline(cin, name);
            cout << "������� ��������� ��������: ";
            cin >> cost;
            cout << "������� ���������� ��������: ";
            cin >> quantity;

            cout << "������� ������� ��������." << endl;

            AddProduct(products, id, name, cost, quantity);
            AuditIn("�������� ������� " + name + " " + to_string(quantity));
            break;
        case 2:
            cout << "������� ID �������� ��� ���������: ";
            cin >> id;
            UpdateProduct(id);
            AuditIn("������� ������� �� ID " + id);
            break;
        case 3:
            cout << "������� ID �������� ��������: ";
            cin >> id;
            RemoveProduct(id);
            AuditIn("������ ������� �� ID " + id);
            break;
        case 4:
            DisplayProducts();
            AuditIn("������� �������� ��������� ���������");
            break;
        case 5:
            AddEmployee();
            break;
        case 6:
            UpdateEmployee();
            break;
        case 7:
            RemoveEmployee();
            break;
        case 8:
            DisplayEmployees();
            break;
        case 9:
            AddMenuItem();
            break;
        case 10:
            UpdateMenuItem();
            break;
        case 11:
            DeleteMenuItem();
            break;
        case 12:
            AuditOut();
            break;
        case 0:
            cout << "�����...\n";
            break;
        default:
            cout << "������������ �����. ���������� �����.\n";
            break;
        }
        cout << "\n-------------------------------------\n";
    }
}

void Administrator::AddProduct(map<int, shared_ptr<Product>>& products, int id, const string& name, double cost, int quantity) {
    shared_ptr<Product> product = make_shared<Product>(id, name, cost, quantity);
    products[id] = product;
    Product::SaveToFile("products.txt", products);
    cout << "������� ������� ��������.\n";
    Administrator::AuditIn("������� " + name + " ������� ��������");
}

void Administrator::UpdateProduct(int id) {

    auto it = products.find(id);
    if (it != products.end()) {
        string name;
        double cost;

        cout << "������� ����� ��� ��������: ";
        cin.ignore();
        getline(cin, name);
        cout << "������� ����� ��������� ��������: ";
        cin >> cost;

        it->second->SetName(name);
        it->second->SetCost(cost);

        cout << "������� ������� �������.\n";
    }
    else {
        cout << "������� �� ������.\n";
    }
}

void Administrator::RemoveProduct(int id) {

    auto it = products.find(id);
    if (it != products.end()) {
        products.erase(it);
        cout << "������ ������� ������.\n";
    }
    else {
        cout << "������� �� ������.\n";
    }
}

void Administrator::DisplayProducts() const {
    if (products.empty()) {
        cout << "��� ��������� ���������.\n";
    }
    else {
        cout << left << setw(5) << "ID"
            << setw(15) << "���"
            << setw(10) << "���� USD"
            << setw(10) << "����������" << endl;
        for (const auto& pair : products) {
            cout << setw(5) << pair.first;
            cout << setw(15) << pair.second->GetName();
            cout << setw(10) << pair.second->GetCost();
            cout << setw(10) << pair.second->GetQuantity();
        }
    }
}

void Administrator::ShowProducts(const map<int, shared_ptr<Product>>& products) {
    AuditIn("������� �������� ShowProducts ��� ��������� ���������");

    cout << left << setw(5) << "ID"
        << setw(15) << "���"
        << setw(10) << "����"
        << setw(10) << "����������" << endl;

    for (const auto& pair : products) {
        cout << setw(5) << pair.second->GetId()
            << setw(15) << pair.second->GetName()
            << setw(10) << pair.second->GetCost()
            << setw(10) << pair.second->GetQuantity() << endl;
    }
}

void Administrator::AddEmployee() {
    string role, lastName, firstName, login, password;

    vector<string> roles = { "admin", "warehouse", "provider", "waiter", "cook", "accountant" };

    cout << right << "������ �����:\n";
    for (const string& role : roles) {
        cout << right << role << endl;
    }

    cout << "������� ���� ����������: ";
    cin.ignore();
    getline(cin, role);
    cout << "������� ������� ����������: ";
    getline(cin, lastName);
    cout << "������� ��� ����������: ";
    getline(cin, firstName);
    cout << "������� ����� �����������: ";
    getline(cin, login);
    cout << "������� ������ ��� ����������: ";
    getline(cin, password);
    AuditIn("�������� ��������� " + firstName + " � ����� " + role);

    shared_ptr<Employee> newEmployee = make_shared<Employee>(role, lastName, firstName, login, hashFunction(password));
    employees.insert(make_pair(login, newEmployee));

    cout << "��������� ������� ��������.\n";
}

void Administrator::UpdateEmployee() {
    string login;
    cout << "������� ����� ���������� ��� ���������: ";
    cin.ignore();
    getline(cin, login);
    AuditIn("������� ��������� ��� ������� " + login);
    vector<string> roles = { "admin", "warehouse", "provider", "waiter", "cook", "accountant" };
    cout << right << "������ �����:\n";
    for (const string& role : roles) {
        cout << right << role << endl;
    }
    auto it = employees.find(login);
    if (it != employees.end()) {
        string role, lastName, firstName, password;

        cout << "������� ����� ���� ����������: ";
        getline(cin, role);
        cout << "������� ����� ������� ����������: ";
        getline(cin, lastName);
        cout << "������� ����� ��� ����������: ";
        getline(cin, firstName);
        cout << "������� ����� ����� �����������: ";
        getline(cin, login);
        cout << "������� ����� ������ ��� ����������: ";
        getline(cin, password);

        it->second->SetRole(role);
        it->second->SetLastName(lastName);
        it->second->SetFirstName(firstName);
        it->second->SetPassword(hashFunction(password));

        cout << "��������� ������� �������.\n";
    }
    else {
        cout << "��������� �� ������.\n";
    }

}

void Administrator::RemoveEmployee() {
    string login;
    cout << "������� ����� ���������� ��� ��������: ";
    cin.ignore();
    getline(cin, login);
    AuditIn("��������� ������ ��� ����� " + login);
    auto it = employees.find(login);
    if (it != employees.end()) {
        employees.erase(it);
        cout << "��������� ������� ������.\n";
    }
    else {
        cout << "��������� �� ������.\n";
    }
}

void Administrator::DisplayEmployees() const {
    if (employees.empty()) {
        cout << "��� ��������� �����������.\n";
    }
    else {
        for (const auto& pair : employees) {
            cout << "�����: " << pair.first << "\n";
            cout << "����: " << pair.second->GetRole() << "\n";
            cout << "������ ���: " << pair.second->GetFullName() << "\n";
            cout << "-------------------------------------\n";
        }
    }
}

void Administrator::AddMenuItem() {
    string name, description;
    int weight, cookingTime, quantity;
    double price;

    cout << "������� ��� �������� ����: ";
    cin.ignore();
    getline(cin, name);
    cout << "������� ��� �������� ����: ";
    cin >> weight;
    cout << "������� �������� �������� ����: ";
    cin.ignore();
    getline(cin, description);
    cout << "������� ����� ���������� �������� ����: ";
    cin >> cookingTime;
    cout << "������� ���� �������� ����: ";
    cin >> price;
    cout << "������� ���������� ��������� ����: ";
    cin >> quantity;

    AuditIn("�������� ����� ������� ���� " + name);
    cout << "��������� ��������:\n";
    DisplayProducts();

    vector<shared_ptr<Product>> ingredients;
    string productIdStr;
    do {
        cout << "������� ID �������� ��� ���������� � �������� ����������� (-1 ��� ������): ";
        cin >> productIdStr;
        int productId = stoi(productIdStr);

        if (productId == -1) {
            break;
        }

        auto productIt = products.find(productId);
        if (productIt != products.end()) {
            ingredients.push_back(productIt->second);
            cout << "���������� ��������.\n";
        }
        else {
            cout << "�������������� ID ��������. ���������� �����.\n";
        }
    } while (true);

    shared_ptr<MenuItem> newMenuItem = make_shared<MenuItem>(name, weight,
        ingredients, description, cookingTime, price, quantity);
    menu->AddMenuItem(newMenuItem);

    cout << "������� ���� ������� ��������.\n";
}

void Administrator::UpdateMenuItem() {
    string name;
    cout << "������� ��� �������� ���� ��� ��������: ";
    cin.ignore();
    getline(cin, name);
    AuditIn("������� ������� ���� ��� ������ " + name);
    shared_ptr<MenuItem> menuItem = MenuItem::FindMenuItemByName(name);
    if (menuItem != nullptr) {
        string newName, newDescription;
        int newWeight, newCookingTime, newQuantity;
        double newPrice;
        cout << "������� ����� ��� ��� �������� ����: ";
        getline(cin, newName);
        cout << "������� ����� �������� ��� �������� ����: ";
        cin >> newWeight;
        cout << "������� ����� �������� ��� �������� ����: ";
        cin.ignore();
        getline(cin, newDescription);
        cout << "������� ����� ����� ���������� ��� �������� ����: ";
        cin >> newCookingTime;
        cout << "������� ����� ���� ��� �������� ����: ";
        cin >> newPrice;
        cout << "������� ����� ���������� ��� �������� ����: ";
        cin >> newQuantity;
        menuItem->SetName(newName);
        menuItem->SetWeight(newWeight);
        menuItem->SetDescription(newDescription);
        menuItem->SetCookingTime(newCookingTime);
        menuItem->SetPrice(newPrice);
        menuItem->SetQuantity(newQuantity);
        cout << "Available products:\n";
        DisplayProducts();
        vector<shared_ptr<Product>> ingredients;
        string productIdStr;
        do {
            cout << "������� ID �������� ��� ���������� ��� ����������  (-1 �����): ";
            cin >> productIdStr;
            int productId = stoi(productIdStr);
            if (productId == -1) {
                break;
            }
            auto productIt = products.find(productId);
            if (productIt != products.end()) {
                ingredients.push_back(productIt->second);
                cout << "���������� ��������.\n";
            }
            else {
                cout << "������������ ID ��������.\n";
            }
        } while (true);
        if (ingredients.size() > 0) {
            menuItem->SetIngredients(ingredients);
        }
        cout << "������� ���� ������� ��������.\n";
        map<int, shared_ptr<MenuItem>> menuItems;
        MenuItem::LoadFromFile("menu_items.txt", menuItems);
        for (auto& pair : menuItems) {
            if (pair.second->GetName() == name) {
                menuItems[pair.first] = menuItem;
                break;
            }
        }

        MenuItem::SaveToFile("menu_items.txt", menuItems);
    }
    else {
        cout << "������� ���� �� ������.\n";
    }
}

void Administrator::DeleteMenuItem() {
    string name;
    cout << "������� ��� �������� ��� ��������: ";
    cin.ignore();
    getline(cin, name);
    AuditIn("������ ������� ���� ��� ������ " + name);

    shared_ptr<MenuItem> menuItem = MenuItem::FindMenuItemByName(name);
    if (menuItem != nullptr) {
        cout << "Menu item successfully deleted.\n";

        map<int, shared_ptr<MenuItem>> menuItems;
        MenuItem::LoadFromFile("menu_items.txt", menuItems);
        MenuItem::RemoveMenuItem(name, menuItems);
        MenuItem::SaveToFile("menu_items.txt", menuItems);
    }
    else {
        cout << "Menu item not found.\n";
    }
}

void Administrator::AuditIn(const string& audit)
{
    ofstream AuditIn("AuditLog.txt", ios::app);
    if (AuditIn.is_open()) {
        time_t currentTime = time(nullptr);
        char timestamp[26];
        ctime_s(timestamp, sizeof(timestamp), &currentTime);
        string timestampStr(timestamp);
        timestampStr.erase(timestampStr.length() - 1);

        AuditIn << timestampStr << " - " << audit << endl;
        AuditIn.close();
    }
    else {
        cout << "������ � �������� �����" << endl;
    }
}
void Administrator::AuditOut()
{
    string line;
    ifstream AuditOut("AuditLog.txt");
    if (AuditOut.is_open()) {
        while (getline(AuditOut, line))
        {
            cout << line << "\n";
        }
        AuditOut.close();
    }
    else {
        cout << "������ � �������� �����" << endl;
    }
}