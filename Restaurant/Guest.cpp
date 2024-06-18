#include "Guest.h"
#include "Order.h"
#include <fstream>
#include "Accountant.h"
#include "Administrator.h"

using namespace std;

Guest::Guest() {}

Guest::Guest(const map<int, shared_ptr<MenuItem>>& menuItems) : menuItems(menuItems) {}

void Guest::Run() {
    cout << "����� ����������, �����!" << endl;

    MenuItem::LoadFromFile("menu_items.txt", menuItems);

    map<string, shared_ptr<Product>> products;

    while (true) {
        cout << endl;
        cout << "1. ������� ����" << endl;
        cout << "2. ������� �����" << endl;
        cout << "3. ���������� ������ ������" << endl;
        cout << "4. �����" << endl;

        int choice;
        cout << "����������� ���� �����: ";
        cin >> choice;

        if (choice == 1) {
            Administrator::AuditIn("����������� ���� ������� � �����");
            DisplayMenu();
        }
        else if (choice == 2) {
            Administrator::AuditIn("����� ������ �����");
            MakeOrder(products);
        }
        else if (choice == 3) {
            Administrator::AuditIn("����� ������� ������ ������");
            TrackOrderStatus();
        }
        else if (choice == 4) {
            break;
        }
        else {
            cout << "������������ �����";
        }
    }

    Administrator::AuditIn("����� ����");
    cout << "������� �� �����! �� ��������!" << endl;
}

void Guest::MakeOrder(map<string, shared_ptr<Product>>& products) {
    DisplayMenu();
    int itemId, quantity;

    cout << "������� ID ������, ������� �� ������ �������� (-1 ��� ����������): ";
    cin >> itemId;
    
    Accountant accountant;

    map<int, shared_ptr<MenuItem>> Items;

    while (itemId != -1) {
        if (menuItems.find(itemId) != menuItems.end()) {
            cout << "������� ����������: ";
            cin >> quantity;

            if (quantity > 0 && quantity <= menuItems.at(itemId)->GetQuantity()) {
                shared_ptr<MenuItem> selectedItem = menuItems.at(itemId);
                selectedItem->SetQuantity(selectedItem->GetQuantity() - quantity);

                for (int i = 0; i < quantity; ++i) {
                    currentOrder.AddItem(selectedItem);
                }

                cout << "����� �������� � �����." << endl;
                Administrator::AuditIn("������ ����� " + selectedItem->GetName() + " �� ���������� " + to_string(selectedItem->GetQuantity()));

                UpdateProductQuantities(products, selectedItem, quantity);
                UpdateProductFile(products);

                Items.emplace(itemId, selectedItem);

                accountant.GenerateSalesReport(Items);
                accountant.GetSoldItemsReport();
            }
            else {
                cout << "������������� ����������. ����������, �������� ������� ����������." << endl;
            }
        }
        else {
            cout << "�������� ID ������. ����������, ���������� �����." << endl;
        }

        cout << "������� ID ������, ������� �� ������ �������� (-1 ��� ����������): ";
        cin >> itemId;
    }

    if (!currentOrder.GetBasket().empty()) {
        if (currentOrder.ConfirmOrder()) {
            cout << "����� ����������� � �������. ����� ������� ���� ������� � ������ �������." << endl;
            Administrator::AuditIn("����� ����������� � ������� ������");

            currentOrder.SetStatus(OrderStatus::PAID_AND_SENT_TO_KITCHEN);
            Order::SaveOrderStatus(OrderStatus::PAID_AND_SENT_TO_KITCHEN);

            ofstream outputFile("receipt.txt");
            outputFile.clear();
            if (outputFile.is_open()) {
                outputFile << "�������� ������:" << endl;
                for (const auto& item : currentOrder.GetBasket()) {
                    outputFile << "��������: " << item->GetName() << endl;
                    outputFile << "����: $" << item->GetPrice() << endl;
                    outputFile << "����������: " << item->GetQuantity() << endl;
                    outputFile << "------------------------" << endl;
                }
                outputFile.close();
                cout << "�������� ������ ���� �������� � ���� receipt.txt" << endl;
            }
            else {
                cout << "�� ������� ������� ���� receipt.txt ��� ������ �������� �������." << endl;
            }
        }
        else {
            cout << "����� �������. ������������ ������� �� ����� �������." << endl;

            RestoreProductQuantities(products);
            currentOrder.ClearBasket();
        }
    }
}

void Guest::UpdateProductQuantities(map<string, shared_ptr<Product>>& products, const shared_ptr<MenuItem>& item, int quantity) {
    Administrator::AuditIn("�������� ���������� �������� ���� �� " + to_string(quantity));

    for (const auto& product : products) {
        if (product.second->GetName() == item->GetName()) {
            product.second->SetQuantity(product.second->GetQuantity() - quantity);
            break;
        }
    }
}

void Guest::RestoreProductQuantities(map<string, shared_ptr<Product>>& products) {
    for (const auto& item : currentOrder.GetBasket()) {
        string itemName = item->GetName();
        if (products.find(itemName) != products.end()) {
            products[itemName]->SetQuantity(products[itemName]->GetQuantity() + 1);
        }
    }
}

void Guest::UpdateProductFile(const map<string, shared_ptr<Product>>& products) {
    ofstream outputFile("products.txt");
    if (outputFile.is_open()) {
        for (const auto& product : products) {
            outputFile << product.second->GetId() << " "
                << product.second->GetName() << " "
                << product.second->GetCost() << " "
                << product.second->GetQuantity() << endl;
        }
        outputFile.close();
    }
    else {
        cout << "���� �� ��������" << endl;
    }
}

void Guest::DisplayMenu() const {
    cout << "����:" << endl;
    for (const auto& pair : menuItems) {
        cout << pair.first << ". " << pair.second->GetName() << " - $"
            << pair.second->GetPrice() << " (����������: " << pair.second->GetQuantity()
            << ")" << endl;
    }
    cout << "-------------------------------------" << endl;
}

void Guest::TrackOrderStatus() const {
    cout << "������ ������: " << Order::GetStatusString(Order::LoadOrderStatus()) << endl;
    Order::SaveOrderStatus(currentOrder.GetStatus());
}
