#include "Person.h"

Person::Person(string name, string id, string email) : name(name), id(id), email(email) {}

Person::~Person() {}

string Person::getName() const {
    return name;
}

string Person::getID() const {
    return id;
}

string Person::getEmail() const {
    return email;
}

void Person::displayInfo() const {
    cout << "ID: " << id << "\nName: " << name << "\nEmail: " << email << endl;
}
