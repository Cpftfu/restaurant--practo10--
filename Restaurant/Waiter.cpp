#include "Waiter.h"
#include "Order.h"
#include <iostream>
#include "Administrator.h"

using namespace std;

void Waiter::Run() {
    Administrator::AuditIn("���������� ���� �� ���������");
    cout << "����� ����������, ��������!" << endl;

    OrderStatus status = Order::LoadOrderStatus();
    if (status == OrderStatus::READY_FOR_DELIVERY) {
        cout << "�����, ������� ��� ��������. �������� ��� ��� �����������? (y/n): ";
        char choice;
        cin >> choice;
        if (choice == 'y') {
            cout << "�������� ������ ��� �����������..." << endl;
            Administrator::AuditIn("�������� ������ ��� ����������� ����������");
            status = OrderStatus::COMPLETED;
            Order::SaveOrderStatus(status);
            cout << "����� ������� ��� ��������" << endl;
            Administrator::AuditIn("����� ������� ��� �������� ����������");
        }
    }
    else {
        cout << "��� ������� ������� ��� ��������." << endl;
    }
    Administrator::AuditIn("�������� �����");
}