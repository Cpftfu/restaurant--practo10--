#include "Guest.h"
#include "Order.h"
#include <fstream>
#include "Accountant.h"
#include "Administrator.h"

using namespace std;

Guest::Guest() {}

Guest::Guest(const map<int, shared_ptr<MenuItem>>& menuItems) : menuItems(menuItems) {}

void Guest::Run() {
    cout << "Добро пожаловать, гость!" << endl;

    MenuItem::LoadFromFile("menu_items.txt", menuItems);

    map<string, shared_ptr<Product>> products;

    while (true) {
        cout << endl;
        cout << "1. Открыть меню" << endl;
        cout << "2. Сделать заказ" << endl;
        cout << "3. Посмотреть статус заказа" << endl;
        cout << "4. Выход" << endl;

        int choice;
        cout << "Подтвердите свой выбор: ";
        cin >> choice;

        if (choice == 1) {
            Administrator::AuditIn("Открывается меню заказов у гостя");
            DisplayMenu();
        }
        else if (choice == 2) {
            Administrator::AuditIn("Гость делает заказ");
            MakeOrder(products);
        }
        else if (choice == 3) {
            Administrator::AuditIn("Гость смотрит статус заказа");
            TrackOrderStatus();
        }
        else if (choice == 4) {
            break;
        }
        else {
            cout << "Неправильный выбор";
        }
    }

    Administrator::AuditIn("Гость ушел");
    cout << "Спасибо за визит! До свидания!" << endl;
}

void Guest::MakeOrder(map<string, shared_ptr<Product>>& products) {
    DisplayMenu();
    int itemId, quantity;

    cout << "Введите ID товара, который вы хотите заказать (-1 для завершения): ";
    cin >> itemId;
    
    Accountant accountant;

    map<int, shared_ptr<MenuItem>> Items;

    while (itemId != -1) {
        if (menuItems.find(itemId) != menuItems.end()) {
            cout << "Введите количество: ";
            cin >> quantity;

            if (quantity > 0 && quantity <= menuItems.at(itemId)->GetQuantity()) {
                shared_ptr<MenuItem> selectedItem = menuItems.at(itemId);
                selectedItem->SetQuantity(selectedItem->GetQuantity() - quantity);

                for (int i = 0; i < quantity; ++i) {
                    currentOrder.AddItem(selectedItem);
                }

                cout << "Товар добавлен в заказ." << endl;
                Administrator::AuditIn("Сделан заказ " + selectedItem->GetName() + " на количество " + to_string(selectedItem->GetQuantity()));

                UpdateProductQuantities(products, selectedItem, quantity);
                UpdateProductFile(products);

                Items.emplace(itemId, selectedItem);

                accountant.GenerateSalesReport(Items);
                accountant.GetSoldItemsReport();
            }
            else {
                cout << "Недостаточное количество. Пожалуйста, выберите меньшее количество." << endl;
            }
        }
        else {
            cout << "Неверный ID товара. Пожалуйста, попробуйте снова." << endl;
        }

        cout << "Введите ID товара, который вы хотите заказать (-1 для завершения): ";
        cin >> itemId;
    }

    if (!currentOrder.GetBasket().empty()) {
        if (currentOrder.ConfirmOrder()) {
            cout << "Заказ подтвержден и оплачен. Сумма платежа была списана с вашего баланса." << endl;
            Administrator::AuditIn("Заказ подтвержден и оплачен гостем");

            currentOrder.SetStatus(OrderStatus::PAID_AND_SENT_TO_KITCHEN);
            Order::SaveOrderStatus(OrderStatus::PAID_AND_SENT_TO_KITCHEN);

            ofstream outputFile("receipt.txt");
            outputFile.clear();
            if (outputFile.is_open()) {
                outputFile << "Заказные детали:" << endl;
                for (const auto& item : currentOrder.GetBasket()) {
                    outputFile << "Название: " << item->GetName() << endl;
                    outputFile << "Цена: $" << item->GetPrice() << endl;
                    outputFile << "Количество: " << item->GetQuantity() << endl;
                    outputFile << "------------------------" << endl;
                }
                outputFile.close();
                cout << "Заказные детали были записаны в файл receipt.txt" << endl;
            }
            else {
                cout << "Не удалось открыть файл receipt.txt для записи заказных деталей." << endl;
            }
        }
        else {
            cout << "Заказ отменен. Недостаточно средств на вашем балансе." << endl;

            RestoreProductQuantities(products);
            currentOrder.ClearBasket();
        }
    }
}

void Guest::UpdateProductQuantities(map<string, shared_ptr<Product>>& products, const shared_ptr<MenuItem>& item, int quantity) {
    Administrator::AuditIn("Изменено количество элемента меню на " + to_string(quantity));

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
        cout << "Файл не доступен" << endl;
    }
}

void Guest::DisplayMenu() const {
    cout << "Меню:" << endl;
    for (const auto& pair : menuItems) {
        cout << pair.first << ". " << pair.second->GetName() << " - $"
            << pair.second->GetPrice() << " (Количество: " << pair.second->GetQuantity()
            << ")" << endl;
    }
    cout << "-------------------------------------" << endl;
}

void Guest::TrackOrderStatus() const {
    cout << "Статус заказа: " << Order::GetStatusString(Order::LoadOrderStatus()) << endl;
    Order::SaveOrderStatus(currentOrder.GetStatus());
}
