#include <iostream>
#include <map>
#include <memory>
#include "Administrator.h"
#include "Guest.h"
#include "Warehouse.h"
#include <Windows.h>
#include <windows.h>
#include "../LibHash/Header.h"
#include "Provider.h"
#include "Accountant.h"
#include "Cook.h"
#include "Waiter.h"

using namespace std;

int main() {
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");

    map< int, shared_ptr<Product>> products;
    map<string, shared_ptr<Employee>> employees;
    shared_ptr<MenuItem> menu = make_shared<MenuItem>();

    Employee::LoadFromFile("employees.txt", employees);
    Product::LoadFromFile("products.txt", products);

    Administrator admin(products, employees, menu);
    Warehouse warehouse;

    Accountant acc;
    Guest run;
    Provider provider;

    Cook cook;
    Waiter waiter;

    int choice = -1;

    while (choice != 0) {
        cout << "Добро пожаловать в систему Ресторана ^_____^ \n";
        cout << "1. Продолжить как сотрудник\n";
        cout << "2. Продолжить как гость\n";
        cout << "0. Выход\n";
        cout << "Выбор: ";

        cin >> choice;
        switch (choice) {
        case 1: {
            system("cls");

            string login, password;
            cout << "Введите логин: ";
            cin.ignore();
            getline(cin, login);

            cout << "Введите пароль: ";
            getline(cin, password);

            auto it = employees.find(login);
            if (it != employees.end() && it->second->GetPassword() == hashFunction(password)) {
                if (it->second->GetRole() == "admin")  admin.Run();
                else if (it->second->GetRole() == "warehouse") warehouse.Run();
                else if (it->second->GetRole() == "provider") provider.RunProvider();
                else if (it->second->GetRole() == "accountant") acc.run();
                else if (it->second->GetRole() == "cook") cook.Run();
                else if (it->second->GetRole() == "waiter") waiter.Run();
            }
            else {
                cout << "Что-то пошло нетак. Попробуйте снова :( \n";
            }
            break;
        }
        case 2:
            system("cls");
            cout << "Вход за гостя.\n";
            Administrator::AuditIn("Вход за гостя");
            run.Run();
            break;
        case 0:
            Administrator::AuditIn("Выход");
            cout << "Выход...\n";
            break;
        default:
            cout << "Что-то пошло нетак. Попробуйте снова :( \n";
            break;
        }
        Product::SaveToFile("products.txt", products);
        Employee::SaveToFile("employees.txt", employees);
    }

    Order::SaveOrderStatus(OrderStatus::UNKNOWN);
}