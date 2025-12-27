#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include "Guest.h"
#include "Librarian.h"
#include "Member.h"
#include "Person.h"
#include <map>
#include <vector>

// For simplicity, we assume one active session user
class Library {
private:
  vector<Book> books;
  vector<Person *> users;
  Person *currentUser;

  // Helper methods
  Book *findBookByID(string id);
  Person *findUserByID(string id);

  // File I/O
  void saveBooks();
  void loadBooks();
  void saveUsers(); // Saves members and librarians
  void loadUsers();

public:
  Library();
  ~Library();

  void run();

  // Menus
  void mainMenu();
  void librarianMenu();
  void memberMenu();
  void guestMenu();

  // Operations
  void addBook();
  void removeBook();
  void registerMember();
  void removeMember();

  void borrowBook();
  void returnBook();
  void searchBooks();
  void displayAllBooks();
  void viewBookQueue();
  void viewAllBorrowedBooks();

  // Member specific
  void viewMyBorrowedBooks();

  // Utilities
  int calculateFine(time_t dueDate, time_t returnDate);
};

#endif // LIBRARY_H
