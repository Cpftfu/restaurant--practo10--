#include "Employee.h"
#include <fstream>
#include <iostream>

using namespace std;

Employee::Employee(const string& role, const string& lastName, const string& firstName, const string& login, int password): role(role), lastName(lastName), firstName(firstName), login(login), password(password) {}

string Employee::GetRole() const {
    return role;
}

string Employee::GetFullName() const {
    return lastName + " " + firstName;
}

string Employee::GetLogin() const {
    return login;
}

int Employee::GetPassword() const {
    return password;
}

void Employee::SetLogin(const string& Login) {
    login = Login;
}

void Employee::SetRole(const string& Role) {
    role = Role;
}

void Employee::SetLastName(const string& LastName) {
    lastName = LastName;
}

void Employee::SetFirstName(const string& FirstName) {
    firstName = FirstName;
}

void Employee::SetPassword(int Password) {
    password = Password;
}

void Employee::LoadFromFile(const string& filename, map<string, shared_ptr<Employee>,
    less<string>, allocator<pair<const string, shared_ptr<Employee>>>>&employees) {
    ifstream file(filename);
    if (file.is_open()) {
        employees.clear();

        string role, lastName, firstName, login;
        int password;

        while (file >> role >> lastName >> firstName >> login >> password) {
            shared_ptr<Employee> employee = make_shared<Employee>(role, lastName, firstName, login, password);
            employees.insert(make_pair(login, employee));
        }

        file.close();
    }
    else {
        cout << "Ошибка в открытии файла: " << filename << endl;
    }
}

void Employee::SaveToFile(const string& filename, const map<string, shared_ptr<Employee>,
    less<string>, allocator<pair<const string, shared_ptr<Employee>>>>&employees) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& pair : employees) {
            file << pair.second->GetRole() << " " << pair.second->GetFullName() << " "
                << pair.second->GetLogin() << " " << pair.second->GetPassword() << endl;
        }

        file.close();
    }
    else {
        cout << "Ошибка в открытии файла: " << filename << endl;
    }
}
