#include "Guest.h"

Guest::Guest(string name, string id, string email) : Person(name, id, email) {}

string Guest::getRole() const { return "Guest"; }
