#ifndef BOOK_H
#define BOOK_H

#include "Queue.h"
#include <iostream>
#include <string>

using namespace std;

class Book {
private:
  string title;
  string author;
  string genre;
  string bookID;
  bool isBorrowed;
  string borrowedByMemberID;
  time_t dueDate;

  // Queue of Member IDs
  Queue<string> reservationQueue;

public:
  Book(string title, string author, string genre, string bookID);

  string getTitle() const;
  string getAuthor() const;
  string getGenre() const;
  string getBookID() const;
  bool getIsBorrowed() const;
  string getBorrowedByMemberID() const;
  time_t getDueDate() const;

  void borrowBook(string memberID, time_t due);
  void returnBook();

  // Reservation Management
  void addReservation(string memberID);
  string processReservation();
  bool hasReservations() const;
  Queue<string> &getReservationQueue();

  int getReservationCount() const;
  time_t getEstimatedAvailabilityDate() const;

  void displayInfo() const;
};

#endif
