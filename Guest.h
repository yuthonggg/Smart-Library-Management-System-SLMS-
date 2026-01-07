#ifndef GUEST_H
#define GUEST_H

#include "Person.h"

class Guest : public Person {
public:
    Guest(string name = "Guest", string id = "G000", string email = "None");
    string getRole() const override;
};

#endif
