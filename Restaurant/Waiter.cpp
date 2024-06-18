#include "Waiter.h"
#include "Order.h"
#include <iostream>
#include "Administrator.h"

using namespace std;

void Waiter::Run() {
    Administrator::AuditIn("Произведен вход за официанта");
    cout << "Добро пожаловать, официант!" << endl;

    OrderStatus status = Order::LoadOrderStatus();
    if (status == OrderStatus::READY_FOR_DELIVERY) {
        cout << "Заказ, готовый для отправки. Пометить его как завершенный? (y/n): ";
        char choice;
        cin >> choice;
        if (choice == 'y') {
            cout << "Потметка заказа как завершенный..." << endl;
            Administrator::AuditIn("Потметка заказа как завершенный официантом");
            status = OrderStatus::COMPLETED;
            Order::SaveOrderStatus(status);
            cout << "Заказ помечен как завершен" << endl;
            Administrator::AuditIn("Заказ помечен как завершен официантом");
        }
    }
    else {
        cout << "Нет готовых заказов для отправки." << endl;
    }
    Administrator::AuditIn("Официант вышел");
}