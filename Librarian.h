#ifndef LIBRARIAN_H
#define LIBRARIAN_H

#include "Person.h"

class Librarian : public Person {
public:
    Librarian(string name, string id, string email);
    string getRole() const override;
    void displayInfo() const override;

};

#endif
