#include "Order.h"
#include <iostream>
#include <fstream>
#include "Administrator.h"


using namespace std;

void Order::AddToBasket(shared_ptr<MenuItem> item) {
    basket.push_back(item);
}

void Order::AddItem(const shared_ptr<MenuItem>& item) {
    basket.push_back(item);
}

double Order::CalculateTotal() const {
    double total = 0.0;
    for (const auto& item : basket) {
        total += item->GetPrice();
    }
    return total;
}

bool Order::ConfirmOrder() const {
    cout << "������������� ������..." << endl;
    cout << "�������� ������!" << endl;
    return true;
}

void Order::TrackOrderStatus() const {
    cout << "������ ������: " << GetStatusString(status) << endl;
}

void Order::GenerateReceipt(const string& filename) const {
    ofstream receiptFile(filename);
    if (receiptFile.is_open()) {
        receiptFile << "Receipt" << endl;
        receiptFile << "-----------------" << endl;
        for (const auto& item : basket) {
            receiptFile << item->GetName() << " - $" << item->GetPrice() << endl;
        }
        receiptFile << "-----------------" << endl;
        receiptFile << "Total: $" << CalculateTotal() << endl;
        receiptFile.close();
        cout << "����� �������� � " << filename << endl;
        Administrator::AuditIn("����� �������� � " + filename);
    }
    else {
        cout << "������ � �������� �����." << endl;
    }
}

const vector<shared_ptr<MenuItem>>& Order::GetBasket() const {
    return basket;
}

void Order::ClearBasket() {
    basket.clear();
}

void Order::SetStatus(OrderStatus status) {
    this->status = status;
}

void Order::SaveOrder(const string& filename) const {
    ofstream file(filename);
    if (file.is_open()) {
        file << basket.size() << endl;
        for (const auto& item : basket) {
            file << item->GetName() << endl;
            file << item->GetQuantity() << endl;
            file << item->GetPrice() << endl;
        }
        file.close();
        cout << "����� �������� � ���� " << filename << endl;
        Administrator::AuditIn("����� �������� � " + filename);
    }
    else {
        cout << "������ � �������� �����." << endl;
    }
}

string Order::GetStatusString(OrderStatus status) {
    switch (status) {
    case OrderStatus::PAID_AND_SENT_TO_KITCHEN:
        return "������� � ��������� �� �����";
    case OrderStatus::IN_PROCESS:
        return "� ��������";
    case OrderStatus::READY_FOR_DELIVERY:
        return "����� � ��������";
    case OrderStatus::COMPLETED:
        return "��������";
    case OrderStatus::UNKNOWN:
        return "����������";
    default:
        return "����������� ������";
    }
}

OrderStatus Order::GetStatus() const {
    return status;
}

void Order::SaveOrderStatus(OrderStatus status) {
    ofstream file("status.txt");
    file.clear();
    if (file.is_open()) {
        file << static_cast<int>(status) << endl;
        file.close();
    }
    else {
        cout << "������ � �������� �����." << endl;
    }
}

OrderStatus Order::LoadOrderStatus() {
    ifstream file("status.txt");
    if (file.is_open()) {
        int status;
        file >> status;
        file.close();
        return static_cast<OrderStatus>(status);
    }
    else {
        cout << "������ � �������� �����." << endl;
        return OrderStatus::UNKNOWN;
    }
}