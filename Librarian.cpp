#include "Librarian.h"

Librarian::Librarian(string name, string id, string email) : Person(name, id, email) {}

string Librarian::getRole() const {
    return "Librarian";
}

void Librarian::displayInfo() const {
    Person::displayInfo();
    cout << "Role: Librarian" << endl;
}
