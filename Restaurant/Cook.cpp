#include "Cook.h"
#include <iostream>
#include <fstream>
#include "Product.h"
#include "MenuItem.h"
#include <sstream>
#include "Warehouse.h"
#include "Administrator.h"

using namespace std;

void Cook::Run() {
    Administrator::AuditIn("Произведен вход за повара");

    OrderStatus status = Order::LoadOrderStatus();

    if (status == OrderStatus::PAID_AND_SENT_TO_KITCHEN) {
        ifstream receiptFile("receipt.txt");

        if (receiptFile.is_open()) {
            string line;

            while (getline(receiptFile, line)) {
                cout << line << endl;
            }

            receiptFile.close();

            int ch;
            cout << "1. Начать готовить\n";
            cout << "Выберите: ";
            cin >> ch;

            switch (ch) {
            case 1: {
                cout << "Есть заказ. Начать готовить? (y/n): ";
                char choice;
                cin >> choice;

                if (choice == 'y') {
                    Administrator::AuditIn("Повар начал готовить");
                    ifstream menuItemsFile("menu_items.txt");

                    if (menuItemsFile.is_open()) {
                        map<string, shared_ptr<MenuItem>> menuItems;
                        MenuItem::LoadFromFileForCook("menu_items.txt", menuItems);

                        for (const auto& pair : menuItems) {
                            cout << pair.second->GetName() << " - " << "Ингредиенты для приготовления\n"
                                << MenuItem::GetItemsIngredients(pair.second->GetIngredients()) << endl;
                        }

                        cout << "Введите названия пунктов меню, чтобы приготовить их (разделите названия пробелами): ";
                        string menuItemsNames;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        getline(cin, menuItemsNames);

                        stringstream ss("menu_items.txt");
                        string menuItemName;
                        bool allMenuItemsAvailable = true;

                        while (ss >> menuItemName) {
                            menuItemName = menuItemsNames;
                            auto it = menuItems.find(menuItemName);

                            if (it != menuItems.end()) {
                                shared_ptr<MenuItem> menuItem = it->second;

                                if (menuItem->GetQuantity() > 0) {
                                    menuItem->SetQuantity(menuItem->GetQuantity() - 1);
                                    vector<shared_ptr<Product>> itemIngredients = menuItem->GetIngredients();

                                    for (const auto& ingredient : itemIngredients) {
                                        Warehouse::RemoveProduct(ingredient->GetName(), 1);
                                    }
                                }
                                else {
                                    cout << "Пункт меню с названием \"" << menuItemName << "\" недоступен." << endl;
                                    allMenuItemsAvailable = false;
                                    break;
                                }
                            }
                            else {
                                cout << "Пункт меню с названием \"" << menuItemName << "\" не найден." << endl;
                                allMenuItemsAvailable = false;
                                break;
                            }
                        }

                        if (allMenuItemsAvailable) {
                            cout << "Процесс приготовления..." << endl;

                            cout << "Все пункты заказа приготовлены. Заказ передан официанту на выдачу." << endl;
                            Administrator::AuditIn("Повар все приготовил");
                            status = OrderStatus::READY_FOR_DELIVERY;
                            Order::SaveOrderStatus(status);
                        }

                        MenuItem::SaveToFileForCook("menu_items.txt", menuItems);
                    }
                    else {
                        cout << "Ошибка в открытии файла: menu_items.txt" << endl;
                    }
                }
                break;
            }
            default:
                status = OrderStatus::IN_PROCESS;
                Order::SaveOrderStatus(status);
                break;
            }
        }
        else {
            Administrator::AuditIn("Ошибка в открытии файла: receipt.txt");

            cout << "Ошибка в открытии файла: receipt.txt" << endl;
        }
    }
    else {
        cout << "Нет заказов для отправки." << endl;
    }

    Administrator::AuditIn("Повар покинул аккаунт");
}
