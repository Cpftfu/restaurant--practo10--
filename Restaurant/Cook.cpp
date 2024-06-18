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
    Administrator::AuditIn("���������� ���� �� ������");

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
            cout << "1. ������ ��������\n";
            cout << "��������: ";
            cin >> ch;

            switch (ch) {
            case 1: {
                cout << "���� �����. ������ ��������? (y/n): ";
                char choice;
                cin >> choice;

                if (choice == 'y') {
                    Administrator::AuditIn("����� ����� ��������");
                    ifstream menuItemsFile("menu_items.txt");

                    if (menuItemsFile.is_open()) {
                        map<string, shared_ptr<MenuItem>> menuItems;
                        MenuItem::LoadFromFileForCook("menu_items.txt", menuItems);

                        for (const auto& pair : menuItems) {
                            cout << pair.second->GetName() << " - " << "����������� ��� �������������\n"
                                << MenuItem::GetItemsIngredients(pair.second->GetIngredients()) << endl;
                        }

                        cout << "������� �������� ������� ����, ����� ����������� �� (��������� �������� ���������): ";
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
                                    cout << "����� ���� � ��������� \"" << menuItemName << "\" ����������." << endl;
                                    allMenuItemsAvailable = false;
                                    break;
                                }
                            }
                            else {
                                cout << "����� ���� � ��������� \"" << menuItemName << "\" �� ������." << endl;
                                allMenuItemsAvailable = false;
                                break;
                            }
                        }

                        if (allMenuItemsAvailable) {
                            cout << "������� �������������..." << endl;

                            cout << "��� ������ ������ ������������. ����� ������� ��������� �� ������." << endl;
                            Administrator::AuditIn("����� ��� ����������");
                            status = OrderStatus::READY_FOR_DELIVERY;
                            Order::SaveOrderStatus(status);
                        }

                        MenuItem::SaveToFileForCook("menu_items.txt", menuItems);
                    }
                    else {
                        cout << "������ � �������� �����: menu_items.txt" << endl;
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
            Administrator::AuditIn("������ � �������� �����: receipt.txt");

            cout << "������ � �������� �����: receipt.txt" << endl;
        }
    }
    else {
        cout << "��� ������� ��� ��������." << endl;
    }

    Administrator::AuditIn("����� ������� �������");
}
