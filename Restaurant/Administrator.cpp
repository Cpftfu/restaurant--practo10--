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
    Administrator::AuditIn("Произведен вход за админа");
    int choice = -1;
    while (choice != 0) {
        cout << "Меню админа:\n";
        cout << "1. Добавить продукт\n";
        cout << "2. Изменить продукт\n";
        cout << "3. Удалить продукт\n";
        cout << "4. Просмотр продуктов\n";
        cout << "5. Добавить сотрудника\n";
        cout << "6. Изменить сотрудника\n";
        cout << "7. Удалить состружника\n";
        cout << "8. Список сотрудников\n";
        cout << "9. Добавить пункт меню\n";
        cout << "10. Изменить пункт меню\n";
        cout << "11. Удалить пункт меню\n";
        cout << "12. Посмотреть файл всех операций\n";
        cout << "0. Разлогиниться\n";
        cout << "Выбор: ";
        cin >> choice;

        int id, quantity;
        string name;
        double cost;
        switch (choice) {
        case 1:
            cout << "Введите ID продукта: ";
            cin >> id;
            cout << "Введите имя продукта: ";
            cin.ignore();
            getline(cin, name);
            cout << "Введите стоимость продукта: ";
            cin >> cost;
            cout << "Введите количество продукта: ";
            cin >> quantity;

            cout << "Продукт успешно добавлен." << endl;

            AddProduct(products, id, name, cost, quantity);
            AuditIn("Добавлен продукт " + name + " " + to_string(quantity));
            break;
        case 2:
            cout << "Введите ID продукта для изменения: ";
            cin >> id;
            UpdateProduct(id);
            AuditIn("Изменен продукт по ID " + id);
            break;
        case 3:
            cout << "Введите ID продукта удаления: ";
            cin >> id;
            RemoveProduct(id);
            AuditIn("Удален продукт по ID " + id);
            break;
        case 4:
            DisplayProducts();
            AuditIn("Вызвана операция просмотра продуктов");
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
            cout << "Выход...\n";
            break;
        default:
            cout << "Неправильный выбор. Попробуйте снова.\n";
            break;
        }
        cout << "\n-------------------------------------\n";
    }
}

void Administrator::AddProduct(map<int, shared_ptr<Product>>& products, int id, const string& name, double cost, int quantity) {
    shared_ptr<Product> product = make_shared<Product>(id, name, cost, quantity);
    products[id] = product;
    Product::SaveToFile("products.txt", products);
    cout << "Продукт успешно добавлен.\n";
    Administrator::AuditIn("Продукт " + name + " успешно добавлен");
}

void Administrator::UpdateProduct(int id) {

    auto it = products.find(id);
    if (it != products.end()) {
        string name;
        double cost;

        cout << "Введите новое имя продукта: ";
        cin.ignore();
        getline(cin, name);
        cout << "Введите новую стоимость продукта: ";
        cin >> cost;

        it->second->SetName(name);
        it->second->SetCost(cost);

        cout << "Продукт успешно изменен.\n";
    }
    else {
        cout << "Продукт не найден.\n";
    }
}

void Administrator::RemoveProduct(int id) {

    auto it = products.find(id);
    if (it != products.end()) {
        products.erase(it);
        cout << "Прдукт успешно удален.\n";
    }
    else {
        cout << "Продукт не найден.\n";
    }
}

void Administrator::DisplayProducts() const {
    if (products.empty()) {
        cout << "Нет доступный продуктов.\n";
    }
    else {
        cout << left << setw(5) << "ID"
            << setw(15) << "Имя"
            << setw(10) << "Цена USD"
            << setw(10) << "Количество" << endl;
        for (const auto& pair : products) {
            cout << setw(5) << pair.first;
            cout << setw(15) << pair.second->GetName();
            cout << setw(10) << pair.second->GetCost();
            cout << setw(10) << pair.second->GetQuantity();
        }
    }
}

void Administrator::ShowProducts(const map<int, shared_ptr<Product>>& products) {
    AuditIn("Вызвана операция ShowProducts для просмотра продуктов");

    cout << left << setw(5) << "ID"
        << setw(15) << "Имя"
        << setw(10) << "Цеца"
        << setw(10) << "Количество" << endl;

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

    cout << right << "Список ролей:\n";
    for (const string& role : roles) {
        cout << right << role << endl;
    }

    cout << "Введите роль сотрудника: ";
    cin.ignore();
    getline(cin, role);
    cout << "Введите фамилию сотрудника: ";
    getline(cin, lastName);
    cout << "Введите имя сотрудника: ";
    getline(cin, firstName);
    cout << "Введите логин сотрудниика: ";
    getline(cin, login);
    cout << "Введите пароль для сотрудника: ";
    getline(cin, password);
    AuditIn("Добавлен сотрудник " + firstName + " с ролью " + role);

    shared_ptr<Employee> newEmployee = make_shared<Employee>(role, lastName, firstName, login, hashFunction(password));
    employees.insert(make_pair(login, newEmployee));

    cout << "Сотрудник успешно добавлен.\n";
}

void Administrator::UpdateEmployee() {
    string login;
    cout << "Введите логин сотрудника для изменения: ";
    cin.ignore();
    getline(cin, login);
    AuditIn("Изменен сотрудник под логином " + login);
    vector<string> roles = { "admin", "warehouse", "provider", "waiter", "cook", "accountant" };
    cout << right << "Список ролей:\n";
    for (const string& role : roles) {
        cout << right << role << endl;
    }
    auto it = employees.find(login);
    if (it != employees.end()) {
        string role, lastName, firstName, password;

        cout << "Введите новую роль сотрудника: ";
        getline(cin, role);
        cout << "Введите новую фамилию сотрудника: ";
        getline(cin, lastName);
        cout << "Введите новое имя сотрудника: ";
        getline(cin, firstName);
        cout << "Введите новый логин сотрудниика: ";
        getline(cin, login);
        cout << "Введите новый пароль для сотрудника: ";
        getline(cin, password);

        it->second->SetRole(role);
        it->second->SetLastName(lastName);
        it->second->SetFirstName(firstName);
        it->second->SetPassword(hashFunction(password));

        cout << "Сотрудник успешно изменен.\n";
    }
    else {
        cout << "Сотрудник не найден.\n";
    }

}

void Administrator::RemoveEmployee() {
    string login;
    cout << "Введите логин сотрудника для удаления: ";
    cin.ignore();
    getline(cin, login);
    AuditIn("Сотрудник удален под ником " + login);
    auto it = employees.find(login);
    if (it != employees.end()) {
        employees.erase(it);
        cout << "Сотрудник успешно удален.\n";
    }
    else {
        cout << "Сотрудник не найден.\n";
    }
}

void Administrator::DisplayEmployees() const {
    if (employees.empty()) {
        cout << "Нет доступных сотрудников.\n";
    }
    else {
        for (const auto& pair : employees) {
            cout << "Логин: " << pair.first << "\n";
            cout << "Роль: " << pair.second->GetRole() << "\n";
            cout << "Полное имя: " << pair.second->GetFullName() << "\n";
            cout << "-------------------------------------\n";
        }
    }
}

void Administrator::AddMenuItem() {
    string name, description;
    int weight, cookingTime, quantity;
    double price;

    cout << "Введите имя элемента меню: ";
    cin.ignore();
    getline(cin, name);
    cout << "Введите вес элемента меню: ";
    cin >> weight;
    cout << "Введите описание элемента меню: ";
    cin.ignore();
    getline(cin, description);
    cout << "Введите время готовности элемента меню: ";
    cin >> cookingTime;
    cout << "Введите цену элемента меню: ";
    cin >> price;
    cout << "Введите количество элементов меню: ";
    cin >> quantity;

    AuditIn("Добавлен новый элемент меню " + name);
    cout << "Доступные продукты:\n";
    DisplayProducts();

    vector<shared_ptr<Product>> ingredients;
    string productIdStr;
    do {
        cout << "Введите ID продукта для добавления в качестве ингридиента (-1 для выхода): ";
        cin >> productIdStr;
        int productId = stoi(productIdStr);

        if (productId == -1) {
            break;
        }

        auto productIt = products.find(productId);
        if (productIt != products.end()) {
            ingredients.push_back(productIt->second);
            cout << "Ингридиент добавлен.\n";
        }
        else {
            cout << "Несучествующий ID продукта. Попробуйте снова.\n";
        }
    } while (true);

    shared_ptr<MenuItem> newMenuItem = make_shared<MenuItem>(name, weight,
        ingredients, description, cookingTime, price, quantity);
    menu->AddMenuItem(newMenuItem);

    cout << "Элемент меню успешно добавлен.\n";
}

void Administrator::UpdateMenuItem() {
    string name;
    cout << "Введите имя элемента меню для удаления: ";
    cin.ignore();
    getline(cin, name);
    AuditIn("Изменен элемент меню под именем " + name);
    shared_ptr<MenuItem> menuItem = MenuItem::FindMenuItemByName(name);
    if (menuItem != nullptr) {
        string newName, newDescription;
        int newWeight, newCookingTime, newQuantity;
        double newPrice;
        cout << "Введите новое имя для элемента меню: ";
        getline(cin, newName);
        cout << "Введите новую грамовку для элемента меню: ";
        cin >> newWeight;
        cout << "Введите новое описание для элемента меню: ";
        cin.ignore();
        getline(cin, newDescription);
        cout << "Введите новое время готовности для элемента меню: ";
        cin >> newCookingTime;
        cout << "Введите новую цену для элемента меню: ";
        cin >> newPrice;
        cout << "Введите новое количество для элемента меню: ";
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
            cout << "Введите ID продукта для добавления как ингридиент  (-1 выход): ";
            cin >> productIdStr;
            int productId = stoi(productIdStr);
            if (productId == -1) {
                break;
            }
            auto productIt = products.find(productId);
            if (productIt != products.end()) {
                ingredients.push_back(productIt->second);
                cout << "Ингридиент добавлен.\n";
            }
            else {
                cout << "Неправильный ID продукта.\n";
            }
        } while (true);
        if (ingredients.size() > 0) {
            menuItem->SetIngredients(ingredients);
        }
        cout << "Элемент меню успешно обновлен.\n";
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
        cout << "Элемент меню не найден.\n";
    }
}

void Administrator::DeleteMenuItem() {
    string name;
    cout << "Введите имя продукта для удаления: ";
    cin.ignore();
    getline(cin, name);
    AuditIn("Удален элемент меню под именем " + name);

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
        cout << "Ошибка в открытии файла" << endl;
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
        cout << "Ошибка в открытии файла" << endl;
    }
}