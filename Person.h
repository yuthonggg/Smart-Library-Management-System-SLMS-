#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>

using namespace std;

class Person {
protected:
    string name;
    string id;
    string email;

public:
    Person(string name, string id, string email);
    virtual ~Person();

    // Getters
    string getName() const;
    string getID() const;
    string getEmail() const;

    // Virtual methods for polymorphism
    virtual void displayInfo() const;
    virtual string getRole() const = 0; // Pure virtual function
};

#endif // PERSON_H
