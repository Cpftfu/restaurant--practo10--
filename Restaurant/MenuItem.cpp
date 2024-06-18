#include "MenuItem.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

MenuItem::MenuItem(): name(""), price(0.0), quantity(0), weight(0), description(""), cookingTime(0) {}

MenuItem::MenuItem(const string& name, double price, int quantity): name(name), price(price), quantity(quantity) {}

MenuItem::MenuItem(const string& name, int weight,const vector<shared_ptr<Product>>& ingredients,const string& description, int cookingTime, double price, int quantity): name(name), weight(weight), ingredients(ingredients), description(description),cookingTime(cookingTime), price(price), quantity(quantity) {}

const string& MenuItem::GetName() const {
    return name;
}

int MenuItem::GetQuantity() const {
    return quantity;
}
void MenuItem::SetName(const string& newName) {
    name = newName;
}
void MenuItem::SetCookingTime(int newCookingTime) {
    cookingTime = newCookingTime;
}

void MenuItem::SetDescription(const string& newDescription) {
    description = newDescription;
}
void MenuItem::SetPrice(double newPrice) {
    price = newPrice;
}

void MenuItem::SetWeight(int newWeight) {
    weight = newWeight;
}

void MenuItem::SetQuantity(int newQuantity) {
    quantity = newQuantity;
}

double MenuItem::GetPrice() const {
    return price;
}

void MenuItem::LoadFromFile(const string& filename, map<int, shared_ptr<MenuItem>>& menuItems) {
    ifstream file(filename);
    if (file.is_open()) {
        menuItems.clear();

        string line;
        int itemId = 1;
        while (getline(file, line)) {
            string name = line;
            int quantity = 0, weight = 0, cookingTime = 0;
            double price = 0.0;
            vector<shared_ptr<Product>> ingredients;
            string description;

            file >> weight;
            file.ignore();
            getline(file, line);
            stringstream ss(line);
            string ingredient;
            while (getline(ss, ingredient, ',')) {
                ingredients.push_back(make_shared<Product>(ingredient, 0));
            }

            getline(file, description);
            file >> cookingTime >> price >> quantity;
            file.ignore();

            shared_ptr<MenuItem> item = make_shared<MenuItem>(name, weight, ingredients, description, cookingTime, price, quantity);

            menuItems[itemId] = item;
            itemId++;
        }

        file.close();
    }
    else {
        cout << "ќшибка в открытии файла: " << filename << "\n";
    }
}

void MenuItem::SaveToFile(const string& filename, const map<int, shared_ptr<MenuItem>>& menuItems) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& pair : menuItems) {
            const shared_ptr<MenuItem>& menuItem = pair.second;

            file << menuItem->GetName() << "\n";
            file << menuItem->GetWeight() << "\n";

            const vector<shared_ptr<Product>>& ingredients = menuItem->GetIngredients();
            for (const auto& ingredient : ingredients) {
                file << ingredient->GetName() << ",";
            }
            file.seekp(-1, ios_base::cur);  
            file << "\n";

            file << menuItem->GetDescription() << "\n";
            file << menuItem->GetCookingTime() << " " << menuItem->GetPrice() << " " << menuItem->GetQuantity() << "\n";
        }
        file.close();
        cout << "Ёлемент меню сохранен в файл: " << filename << "\n";
    }
    else {
        cout << "ќшибка в открытии файла: " << filename << "\n";
    }
}

int MenuItem::GetWeight() const {
    return weight;
}

const vector<shared_ptr<Product>>& MenuItem::GetIngredients() const {
    return ingredients;
}

string MenuItem::GetItemsIngredients(const vector<shared_ptr<Product>>& ingredients) {
    string result;
    for (const auto& ingredient : ingredients) {
        result += ingredient->GetName() + "\n";
    }
    return result;
}

string MenuItem::GetDescription() const {
    return description;
}

int MenuItem::GetCookingTime() const {
    return cookingTime;
}

void MenuItem::AddMenuItem(const shared_ptr<MenuItem>& item) {
    menuItems.push_back(item);
    ofstream file("menu_items.txt", ios::app);
    if (file.is_open()) {
        file << item->GetName() << "\n";
        file << item->GetWeight() << "\n";
        for (const auto& ingredient : item->GetIngredients()) {
            file << ingredient->GetName() << ",";
        }
        file.seekp(-1, ios_base::cur);
        file << "\n";
        file << item->GetDescription() << "\n";
        file << item->GetCookingTime() << " " << item->GetPrice() << " " << item->GetQuantity() << "\n";

        file.close();
    }
    else {
        cout << "ќшибка в открытии файла: menu_items.txt\n";
    }

}

void MenuItem::LoadFromFileForCook(const string& filename, map<string, shared_ptr<MenuItem>>& menuItems) {
    ifstream file(filename);
    if (file.is_open()) {
        menuItems.clear();

        string line;

        while (getline(file, line)) {
            string name = line;
            int quantity = 0, weight = 0, cookingTime = 0;
            double price = 0.0;
            vector<shared_ptr<Product>> ingredients;
            string description;

            file >> weight;
            file.ignore();
            getline(file, line);
            stringstream ss(line);
            string ingredient;
            while (getline(ss, ingredient, ',')) {
                ingredients.push_back(make_shared<Product>(ingredient, 0.0));
            }

            getline(file, description);
            file >> cookingTime >> price >> quantity;
            file.ignore();

            shared_ptr<MenuItem> item = make_shared<MenuItem>(name, weight, ingredients, description, cookingTime, price, quantity);

            menuItems[name] = item;
        }

        file.close();
    }
    else {
        cout << "ќшибка в открытии файла: " << filename << "\n";
    }
}

void MenuItem::SaveToFileForCook(const string& filename, const map<string, shared_ptr<MenuItem>>& menuItems) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& pair : menuItems) {
            const shared_ptr<MenuItem>& item = pair.second;

            file << item->GetName() << "\n";
            file << item->GetWeight() << "\n";
            for (const auto& ingredient : item->GetIngredients()) {
                file << ingredient->GetName() << ",";
            }
            file.seekp(-1, ios_base::cur);
            file << "\n";
            file << item->GetDescription() << "\n";
            file << item->GetCookingTime() << " " << item->GetPrice() << " " << item->GetQuantity() << "\n";
        }

        file.close();
    }
    else {
        cout << "ќшибка в открытии файла: " << filename << "\n";
    }
}

void MenuItem::RemoveMenuItem(const string& itemName, map<int, shared_ptr<MenuItem>>& menuItems) {
    auto it = find_if(menuItems.begin(), menuItems.end(), [&itemName](const auto& pair) {
        return pair.second->GetName() == itemName;
        });

    if (it != menuItems.end()) {
        menuItems.erase(it);
        cout << "Ёлемент меню успешно удален.\n";
    }
    else {
        cout << "Ёлемент меню не найден.\n";
    }
}

shared_ptr<MenuItem> MenuItem::FindMenuItemByName(const string& name) {
    ifstream file("menu_items.txt");

    if (file.is_open()) {
        string line;

        while (getline(file, line)) {
            if (line == name) {
                int weight, cookingTime, quantity;
                double price;
                vector<shared_ptr<Product>> ingredients;
                string description;

                file >> weight;
                file.ignore();
                getline(file, line);
                stringstream ss(line);
                string ingredient;
                while (getline(ss, ingredient, ',')) {
                    ingredients.push_back(make_shared<Product>(ingredient, 0.0));
                }

                getline(file, description);
                file >> cookingTime >> price >> quantity;
                file.ignore();

                shared_ptr<MenuItem> menuItem = make_shared<MenuItem>(name, weight, ingredients, description, cookingTime, price, quantity);

                file.close();
                return menuItem;
            }
            else {
                getline(file, line); 
                getline(file, line); 
                getline(file, line); 
                getline(file, line); 
            }
        }

        file.close();
    }
    else {
        cout << "Ќе найден menu_items.txt\n";
    }
}