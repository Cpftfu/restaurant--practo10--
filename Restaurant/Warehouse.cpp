#include "Warehouse.h"
#include "Administrator.h"
#include <iostream>
#include <iomanip>
#include "Provider.h"
#include "Product.h"
#include "Accountant.h"

using namespace std;

map<string, int> Warehouse::inventory;
double Warehouse::GetProductPrice(const string& productName) const {
    if (prices.find(productName) != prices.end()) {
        return prices.at(productName);
    }
    else {
        return -1.0;
    }
}

void Warehouse::RemoveProduct(const string& productName, int quantity) {
    inventory[productName] += quantity;

    map<int, shared_ptr<Product>> productMap;

    Product::LoadFromFile("products.txt", productMap);

    for (auto& pair : productMap) {
        if (pair.second->GetName() == productName) {
            pair.second->SetQuantity(pair.second->GetQuantity() - quantity);
            break;
        }
    }

    Product::SaveToFile("products.txt", productMap);

}

int Warehouse::GetProductId(const string& productName) {
    for (const auto& pair : inventory) {
        if (pair.first == productName) {
            return pair.second;
        }
    }
    return -1; 
}

double Warehouse::GetProductCost(const string& productName) {
    for (const auto& pair : inventory) {
        if (pair.first == productName) {
            return pair.second;
        }
    }
}

void Warehouse::AddProduct(const string& productName, int quantity) {
    inventory[productName] += quantity;

    map<int, shared_ptr<Product>> productMap;

    Product::LoadFromFile("products.txt", productMap);

    for (auto& pair : productMap) {
        if (pair.second->GetName() == productName) {
            pair.second->SetQuantity(pair.second->GetQuantity() + quantity);
            break;
        }
    }
    Product::SaveToFile("products.txt", productMap);
}

void Warehouse::DisplayInventory() const {
    cout << "Складские запасы:\n";
    for (const auto& item : inventory) {
        cout << item.first << ": " << item.second << endl;
    }
}

bool Warehouse::CanSendOrder(const map<string, int>& order, double balance) const {
    double totalCost = 0.0;
    for (const auto& item : order) {
        const string& productName = item.first;
        int quantity = item.second;

        if (inventory.find(productName) == inventory.end()) {
            cout << "Продукт \"" << productName << "\" отсутствует на складе.\n";
            return false;
        }

        int availableQuantity = inventory.at(productName);
        if (quantity > availableQuantity) {
            cout << "Недостаточное количество продукта \"" << productName << "\" на складе.\n";
            return false;
        }

        if (prices.find(productName) == prices.end()) {
            cout << "Цена продукта \"" << productName << "\" не найдена.\n";
            return false;
        }

        double productPrice = prices.at(productName);
        totalCost += productPrice * quantity;
    }

    if (balance < totalCost) {
        cout << "Недостаточный баланс денежных средств для отправки заявки.\n";
        return false;
    }

    return true;
}

map<string, shared_ptr<Product>> Warehouse::GetProducts() {
    map<string, shared_ptr<Product>> products;

    map<int, shared_ptr<Product>> productMap;
    Product::LoadFromFile("products.txt", productMap);

    for (const auto& pair : productMap) {
        int id = pair.first;
        shared_ptr<Product> product = pair.second;
        products[to_string(id)] = product;
    }

    return products;
}

void Warehouse::LoadProducts(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Ошибка при открытии файла: " << filename << endl;
        return;
    }

    inventory.clear();
    prices.clear();
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        int id, cost, quantity;
        string name;
        if (iss >> id >> name >> cost >> quantity) {
            inventory[name] = quantity;
            prices[name] = cost;
        }
    }

    file.close();
}

void Warehouse::SaveProducts(const string& filename) {
    ofstream file(filename);
    if (!file) {
        cout << "Ошибка при открытии файла для записи: " << filename << endl;
        return;
    }

    for (const auto& item : inventory) {
        const string& productName = item.first;
        int quantity = item.second;
        if (prices.find(productName) != prices.end()) {
            double price = prices[productName];
            file << productName << " " << price << " " << quantity << endl;
        }
    }

    file.close();
}

void Warehouse::Run() {
    Administrator::AuditIn("Произведен вход за кладовщика");
    map<int, shared_ptr<Product>> products;
    map<string, shared_ptr<Product>> pro;
    map<string, shared_ptr<Employee>> employees;
    shared_ptr<MenuItem> menu = make_shared<MenuItem>();
    Administrator admin{ products, employees, menu };
    Accountant accountant;


    for (const auto& pair : products) {
        int key = pair.first;
        shared_ptr<Product> value = pair.second;
        pro[to_string(key)] = value;
    }
    cout << "Warehouse Management System" << endl;

    while (true) {

        cout << "1. Добавить продукт" << endl;
        cout << "2. Удалить продукт" << endl;
        cout << "3. Изменить количество продуктов" << endl;
        cout << "4. Посмотреть склад" << endl;
        cout << "5. Отправить заявку на продукты" << endl;
        cout << "6. Выход" << endl;
        cout << "Выбор: ";
        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            int id;
            string name;
            double cost;
            int quantity;

            cout << "Введите ID продукта: ";
            cin >> id;
            cout << "Введите имя продукта : ";
            cin.ignore();
            getline(cin, name);
            cout << "Введите стоимость продукта: ";
            cin >> cost;
            cout << "Введите количество продукта: ";
            cin >> quantity;

            admin.AddProduct(products, id, name, cost, quantity);
            break;
        }
        case 2: {
            int id;
            cout << "Введите ID продукта: ";
            cin >> id;

            admin.RemoveProduct(id);
            cout << "Продукт успешно удален." << endl;
            Administrator::AuditIn("Продукт по ID " + to_string(id) + " успешно удален");
            break;
        }
        case 3: {
            int id;
            cout << "Введите ID продукта: ";
            cin >> id;

            admin.UpdateProduct(id);
            cout << "Продукт успешно изменен." << endl;
            Administrator::AuditIn("Продукт по ID " + to_string(id) + " успешно обновлен");
            break;
        }
        case 4: {
            map<int, shared_ptr<Product>> products1;

            Product::LoadFromFile("products.txt", products1);
            admin.ShowProducts(products1);
            break;
        }
        case 5: {
            map<string, int> order;
            double balance;
            LoadProducts("products.txt");

            string orderFilename = "OrderProduct.txt";
            ofstream orderFile(orderFilename);
            if (!orderFile) {
                cout << "Ошибка в открытии order файла." << endl;
                break;
            }

            cout << "Введите детали заказа:" << endl;
            cout << "Имя продукта (-1 to stop): ";
            string productName;
            cin >> productName;

            while (productName != "-1") {
                int quantity;
                cout << "Количество: ";
                cin >> quantity;

                order[productName] = quantity;

                orderFile << setw(15) << left << productName << quantity << endl;

                cout << "Имя продукта (-1 to stop): ";
                cin >> productName;
                Administrator::AuditIn("Заявка на продукт " + productName + " в количесте " + to_string(quantity));
            }

            orderFile.close();

            cout << "Введите балансе: ";
            cin >> balance;
            if (CanSendOrder(order, balance)) {
                cout << "Заявка на продукты может быть отправлена." << endl;
                Administrator::AuditIn("Заявка на продукты может быть отправлена.");

                Provider provider;
                provider.ProcessOrder(order, balance);
                provider.ViewOrder(orderFilename);
            }
            accountant.GenerateFinancialReport(pro);
            break;
        }
        case 6: {
            SaveProducts("products.txt");
            Administrator::AuditIn("Выход кладовщика");
            cout << "Выход..." << endl;
            return;
        }
        default: {
            cout << "Неправильный выбор." << endl;
            break;
        }
        }
    }
}